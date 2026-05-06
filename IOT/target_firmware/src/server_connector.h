#pragma once

#include <stdint.h>
#define MAX_STRING_LENGTH 100

int server_connector_init(); // Forsøg at forbinde til wifi og derefter server

void server_connector_prepare_wifi_line_buffer(char* buffer,uint8_t max_length);
