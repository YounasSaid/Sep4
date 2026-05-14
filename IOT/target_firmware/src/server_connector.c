#include "server_connector.h"
#include "wifi.h"
#include "uart_stdio.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"
#include <stdlib.h>

static char _tmp_buff1[MAX_STRING_LENGTH] = {0};
static char _tmp_buff2[MAX_STRING_LENGTH] = {0};

static char string_received[MAX_STRING_LENGTH] = {0};

static queue_t queue;

static void wifi_line_callback(const char *line)
{
    uint8_t _index;
    _index = strlen(string_received);
    string_received[_index] = '\0';

    // Kopier string_received over i queue
    int string_received_length = strlen(string_received);
    char* str_copy = malloc(string_received_length * sizeof(char) + 1);
    strcpy(str_copy, string_received);
    queue_enqueue(queue, str_copy);

    string_received[0] = '\0';
}

int server_connector_init(uint8_t id)
{
    queue = queue_create_queue(MESSAGE_QUEUE_SIZE);

    strcpy(_tmp_buff1, WIFI_SSID);     // SSID
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
    char auth[] = "auth," API_KEY ";";
    int len = strlen(auth);
    WIFI_ERROR_MESSAGE_t auth_status = wifi_command_TCP_transmit((uint8_t *)auth, len);

    if (auth_status != WIFI_OK)
    {
        printf("Fejl under auth\n");
        return 0;
    }

    _delay_ms(1000);

    WIFI_ERROR_MESSAGE_t id_status = server_connector_send_plant_id(id);

    if (id_status != WIFI_OK)
    {
        printf("Fejl under sending af id\n");
        return 0;
    }
    printf("Succesfully joined TCP server\n");

    return 1;
}

WIFI_ERROR_MESSAGE_t server_connector_send_plant_id(uint8_t id_to_send)
{

    char id_message[8];
    int len = sprintf(id_message, "id,%u;", id_to_send);
    WIFI_ERROR_MESSAGE_t id_status = wifi_command_TCP_transmit((uint8_t *)id_message, len);

    return id_status;
}

bool server_connector_has_received_message() {
    return !queue_isEmpty(queue);
}

void server_connector_get_received_message(char* received_buffer, size_t size) {
    if (queue_isEmpty(queue)) {
        printf("Ingen beskeder at tage fra");
        return;
    }
    char* message = (char*) queue_dequeue(queue);

    strncpy(received_buffer, message, size - 1);
    
    free(message);
}