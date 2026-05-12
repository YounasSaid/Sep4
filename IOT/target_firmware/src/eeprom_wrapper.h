#pragma once
#include <stdint.h>

uint8_t eeprom_wrapper_read_byte(const uint8_t * address);
void eeprom_wrapper_update_byte(const uint8_t * address, uint8_t value);