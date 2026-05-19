#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "wifi.h"

#define MAX_STRING_LENGTH 100

#define MESSAGE_QUEUE_SIZE 10

int server_connector_init(uint8_t id); // Forsøg at forbinde til wifi og derefter server

WIFI_ERROR_MESSAGE_t server_connector_send_plant_id(uint8_t id_to_send);

bool server_connector_has_received_message();

void server_connector_get_received_message(char* received_buffer, size_t size);