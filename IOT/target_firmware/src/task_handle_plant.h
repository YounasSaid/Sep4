#pragma once

#include <stdbool.h>
#include <stdint.h>

extern uint8_t plant_id;

void task_handle_plant_init(void);

void task_handle_plant_run(void);