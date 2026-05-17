#include "eeprom_wrapper.h"
#include <avr/eeprom.h>
#include <stdint.h>

uint8_t eeprom_wrapper_read_byte(const uint8_t * address)
{
    return eeprom_read_byte((uint8_t *)address);
}
void eeprom_wrapper_update_byte(const uint8_t * address, uint8_t value)
{
    eeprom_update_byte((uint8_t *)address, value);
}
