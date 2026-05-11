#include "handle_plant.h"
#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "server_connector.h"

uint8_t savedValue EEMEM;

void task_handle_plant_init(void)
{
    uint8_t read_value = eeprom_read_byte(&savedValue);

    printf("Start værdi: %u", read_value);

    if (read_value == 0xFF)
    {
        plant_id = 0; // default værdi
    }
    else
    {
        plant_id = read_value;
    }

    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC11);
    EIMSK |= (1 << INT1);

    EICRA |= (1 << ISC21);
    EIMSK |= (1 << INT2);
}

void task_handle_plant_run(void)
{
    if (button_get(3))
    {
        eeprom_update_byte(&savedValue, plant_id);
        
        server_connector_send_plant_id();
    }
    display_int(plant_id);

    printf("%u", plant_id);
}

ISR(INT0_vect)
{
    plant_id--;
}

ISR(INT1_vect)
{
    plant_id++;
}
