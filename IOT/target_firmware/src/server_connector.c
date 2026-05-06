#include "server_connector.h"
#include "wifi.h"
#include "uart_stdio.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

static char _tmp_buff1[MAX_STRING_LENGTH] = {0};
static char _tmp_buff2[MAX_STRING_LENGTH] = {0};
static bool _tcp_string_received = false;

void server_connector_prepare_wifi_line_buffer(char *buffer, uint8_t max_length)
{
    uint8_t _index;
    _index = strlen(buffer);

    if (_index < (max_length - 3)) // En tilovers som sikkerhed
    {
        buffer[_index] = '\r';
        buffer[_index + 1] = '\n';
        buffer[_index + 2] = '\0';
    }
}

static void wifi_line_callback(const char *line)
{
    server_connector_prepare_wifi_line_buffer(_tmp_buff1, MAX_STRING_LENGTH);
    _tcp_string_received = true;
}

int server_connector_init()
{
    strcpy(_tmp_buff1, "\0"); // SSID
    strcpy(_tmp_buff2, "\0"); // PASSWORD
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

    char serverIpAddress[] = "98.71.68.49";
    WIFI_ERROR_MESSAGE_t message = wifi_command_create_TCP_connection(serverIpAddress, 23, wifi_line_callback, _tmp_buff1);
    if (message != WIFI_OK)
    {
        printf("Failed to connect to server. Terminating\n");
        return 0;
    }
    else
    {
        printf("Succesfully joined TCP server\n");
    }

    return 1;
}
