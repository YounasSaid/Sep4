#include "task_handle_plant.h"
#include <stdint.h>
#include <eeprom_wrapper.h>
#include "server_connector.h"
#include <stdio.h>
#include "button.h"
#include "display.h"

#define MAX_PLANT_ID 254

uint8_t plant_id;

uint8_t savedValue EEMEM;

bool plant_id_changed;

void task_handle_plant_init(void)
{
    uint8_t read_value = eeprom_wrapper_read_byte(&savedValue);

    if (read_value == 0xFF)
    {
        plant_id = 0; // default værdi
        eeprom_wrapper_update_byte(&savedValue, plant_id);
    }
    else
    {
        plant_id = read_value;
    }
}

void task_handle_plant_run(void)
{
    plant_id_changed = false;
    if (button_get(1))
    {
        if (plant_id == MIN_PLANT_ID)
        {
            plant_id = MAX_PLANT_ID; // wrap rundt til max=254
        }
        else
        {
            plant_id--;
        }
        plant_id_changed = true;
    }
    if (button_get(2))
    {
        if (plant_id == MAX_PLANT_ID)
        {
            plant_id = MIN_PLANT_ID; // wrap rundt til min=0
        }
        else
        {
            plant_id++;
        }
        plant_id_changed = true;
    }

    if (plant_id_changed)
    {
        display_int(plant_id);
    }

    // Gem plante id'et på EEP-ROM
    if (button_get(3))
    {
        eeprom_wrapper_update_byte(&savedValue, plant_id);

        server_connector_send_plant_id(plant_id);
        display_int(1000); // Indikerer at id'et er sendt
    }
}
