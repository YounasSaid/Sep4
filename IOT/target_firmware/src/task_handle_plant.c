#include "task_handle_plant.h"
#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "server_connector.h"
#include <stdio.h>
#include "button.h"
#include "display.h"


uint8_t plant_id;
uint8_t savedValue EEMEM;

void task_handle_plant_init(void)
{
    uint8_t read_value = eeprom_read_byte(&savedValue);

    if (read_value == 0xFF)
    {
        plant_id = 0; // default værdi
        eeprom_update_byte(&savedValue, plant_id);
    }
    else
    {
        plant_id = read_value;
    }
}

void task_handle_plant_run(void)
{
    if (button_get(1)) {
        plant_id--;
    }
    if (button_get(2)) {
        plant_id++;
    }
    
    display_int(plant_id);

    // Gem plante id'et på EEP-ROM
    if (button_get(3))
    {
        eeprom_update_byte(&savedValue, plant_id);

        server_connector_send_plant_id(plant_id);
        display_int(1000); // Indikerer at id'et er sendt
    }
}
