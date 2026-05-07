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

    // Det tid det tager at skyde vand igennem pumpen - det kommer først ud af den anden ende efter ~1 sekund
    _delay_ms(1000);
    // Kinda cursed, men delay metoden skal have en fast værdi, og kan ikke tage et variabel
    for (int i = 0; i < ms; i++) {
        _delay_ms(1);
    }

    pump_turn_off();
}

void pump_turn_on_amount(int ml) {
    int ms = ml * 20; // Den tager ~1dl +-4% på 2 sekunder
    pump_turn_on_duration(ms);
}