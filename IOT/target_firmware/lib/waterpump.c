#include "waterpump.h"
#include <avr/io.h>
#include <util/delay.h>


void pump_init() {
    DDRC |= (1 << PC7);
    PORTC &= ~(1 << PC7);
}

void pump_turn_on() {
    PORTC |= (1 << PC7);
}

void pump_turn_off() {
    PORTC &= ~(1 << PC7);
}

void pump_turn_on_duration(int ms) {
    pump_turn_on();
    delay(ms);
    pump_turn_off();
}