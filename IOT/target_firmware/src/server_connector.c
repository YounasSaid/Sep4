#include "server_connector.h"
#include "wifi.h"
#include "uart_stdio.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define MAX_STRING_LENGTH 100

static char _tmp_buff1[MAX_STRING_LENGTH] = {0};
static char _tmp_buff2[MAX_STRING_LENGTH] = {0};

bool _tcp_string_received = false;
char string_received[MAX_STRING_LENGTH] = {0};

void wifi_line_callback(const char *line)
{
    uint8_t _index;
    _index = strlen(string_received);
    string_received[_index] = '\0';
    _tcp_string_received = true;
}

int server_connector_init()
{
    strcpy(_tmp_buff1, WIFI_SSID); // SSID
    strcpy(_tmp_buff2, WIFI_PASSWORD); // PASSWORD
    printf("Forbinder til SSID: <%s> PASSWORD: <%s>\n", _tmp_buff1, _tmp_buff2);
    if (wifi_command_join_AP(_tmp_buff1, _tmp_buff2) != WIFI_OK)
    {
        printf("Failed to join WiFi network. Terminating.\n");
        return 0;
    }
    else
    {
        printf("Successfully joined WiFi network.\n");
    }

    char serverIpAddress[] = SERVER_IP;
    WIFI_ERROR_MESSAGE_t message = wifi_command_create_TCP_connection(serverIpAddress, 23, wifi_line_callback, string_received);
    if (message != WIFI_OK)
    {
        printf("Failed to connect to server. Terminating\n");
        return 0;
    }

    // Auth
    char auth[] = "auth:" API_KEY ";";
    int len = strlen(auth);
    WIFI_ERROR_MESSAGE_t status = wifi_command_TCP_transmit((uint8_t *)auth, len);

    if (status != WIFI_OK)
    {
        printf("Fejl under auth\n");
        return 0;
    }
    printf("Succesfully joined TCP server\n");
    
    return 1;
}
