#include "task_read_server.h"
#include "server_connector.h"
#include "waterpump.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void task_read_server_init(){

}
void task_read_server_run(){
    // Hvis ingen beskeder, så forsæt ikke
    if (!_tcp_string_received) {
        return;
    }

    printf("Modtog besked fra server: %s\n", string_received);

    char *type = strtok(string_received, ",");
    char *value_str = strtok(NULL, ";");

    if (type == NULL || value_str == NULL) {
        printf("ugyldig besked modtaget, kan ikke læse\n");
        return;
    } 

    if (strcmp(type, "water") == 0) {
        int value = atoi(value_str);
        pump_turn_on_amount(value);
    } else {
        printf("Ukendt kommando\n");
    }

    // Clear besked modtaget, og gør klar til næste
    _tcp_string_received = false;
    string_received[0] = '\0';
}