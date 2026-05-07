#include <stdbool.h>
#include <stdint.h>
#pragma once
#include <stdbool.h>

#define MAX_STRING_LENGTH 100

extern bool _tcp_string_received;
extern char string_received[MAX_STRING_LENGTH];

int server_connector_init(uint8_t id); // Forsøg at forbinde til wifi og derefter server
