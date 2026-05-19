#include "task_read_server.h"
#include "server_connector.h"
#include "waterpump.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task_connection_timeout.h"
#include "servo.h"

void task_read_server_init()
{
    pump_init();
}
void task_read_server_run()
{
    // Hvis ingen beskeder, så forsæt ikke
    if (!server_connector_has_received_message())
    {
        return;
    }

    char string_received[MAX_STRING_LENGTH] = {0};
    server_connector_get_received_message(string_received, sizeof(string_received));

    printf("Modtog besked fra server: %s\n", string_received);

    char *type = strtok(string_received, ",");
    char *value_str = strtok(NULL, ";");

    if (type == NULL || value_str == NULL)
    {
        printf("ugyldig besked modtaget, kan ikke læse\n");
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
        task_connection_timeout_set_seconds_to_timeout(atoi(value_str) + TIME_SERVER_PING_SENSITIVTY_SECONDS);
    }
    else if (strcmp(type, "window") == 0) {
        int value = atoi(value_str);
        if (value >= -90 && value <= 90) {
            servo_setAngle(PWM_A, atoi(value_str));
        } else {
            printf("Kan ikke flytte vindue til vinkel: %d", value);
        }
    }
    else
    {
        printf("Ukendt kommando\n");
    }
}