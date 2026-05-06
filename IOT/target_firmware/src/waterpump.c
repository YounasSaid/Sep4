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

    // Kinda cursed, men delay metoden skal have en fast værdi, og kan ikke tage et variabel
    for (int i = 0; i < ms; i++) {
        _delay_ms(1);
    }

    pump_turn_off();
}

void pump_turn_on_amount(int ml) {
    // TODO: Find ud af ml/ms
    int ms = ml * 1;
    pump_turn_on_duration(ms);
}