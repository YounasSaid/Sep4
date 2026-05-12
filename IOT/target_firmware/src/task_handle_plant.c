#include "task_handle_plant.h"
#include <stdint.h>
#include <eeprom_wrapper.h>
#include "server_connector.h"
#include <stdio.h>
#include "button.h"
#include "display.h"

static uint8_t plant_id;

static uint8_t saved_value EEMEM;

void task_handle_plant_init(void)
{
    uint8_t read_value = eeprom_wrapper_read_byte(&saved_value);
    plant_id = read_value;
}

void task_handle_plant_run(void)
{
    if (button_get(1))
    {
        plant_id--;
    }
    if (button_get(2))
    {
        plant_id++;
    }

    display_int(plant_id);

    // Gem plante id'et på EEP-ROM
    if (button_get(3))
    {
        eeprom_wrapper_update_byte(&saved_value, plant_id); // Gem på eeprom

        server_connector_send_plant_id(plant_id); // Send til server, at der er blevet opdateret id

        display_int(1000); // Indikerer at id'et er sendt
    }
}

int get_plant_id(void) {
    return plant_id;
}