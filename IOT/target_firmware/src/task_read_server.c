#include "task_read_server.h"
#include "server_connector.h"
#include "waterpump.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task_connection_timeout.h"

void task_read_server_init()
{
    pump_init();
}
void task_read_server_run()
{
    // Hvis ingen beskeder, så forsæt ikke
    if (!_tcp_string_received)
    {
        return;
    }

    _tcp_string_received = false;

    printf("Modtog besked fra server: %s\n", string_received);

    char *type = strtok(string_received, ",");
    char *value_str = strtok(NULL, ";");

    if (type == NULL || value_str == NULL)
    {
        printf("ugyldig besked modtaget, kan ikke læse\n");
        string_received[0] = '\0'; // Ryd buffer ved ugyldig besked
        return;
    }

    if (strcmp(type, "water") == 0)
    {
        int value = atoi(value_str);
        if (value > 0 && value <= 100) // mellem 1 og 100
        {
            pump_turn_on_amount(value);
        }
    }
    else if (strcmp(type, "ping") == 0) 
    {
        seconds_to_timeout = atoi(value_str) + TIME_SERVER_PING_SENSITIVTY_SECONDS;
    }
    else
    {
        printf("Ukendt kommando\n");
    }

    // Ryd buffer for at gøre klar til næste
    string_received[0] = '\0';
}