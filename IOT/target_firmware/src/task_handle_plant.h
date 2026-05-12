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


extern bool plant_id_changed;

extern uint8_t plant_id;

extern uint8_t savedValue EEMEM;

void task_handle_plant_init(void);

void task_handle_plant_run(void);