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

#define MAX_PLANT_ID 254
#define MIN_PLANT_ID 0

void task_handle_plant_init(void);

void task_handle_plant_run(void);

int get_plant_id(void);