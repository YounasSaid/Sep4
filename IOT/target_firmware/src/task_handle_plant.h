#pragma once
#include <stdbool.h>
#include <stdint.h>

#if defined(__AVR__)
#include <avr/eeprom.h>
#else
#if !defined(EEMEM)
#define EEMEM
#endif
#endif


void task_handle_plant_init(void);

void task_handle_plant_run(void);

int task_handle_plant_get_plant_id(void);

void task_handle_plant_set_plant_id(int id); // Bruges i testen