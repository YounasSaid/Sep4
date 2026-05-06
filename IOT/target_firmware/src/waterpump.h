#pragma once

// Initialisere pump driver
void pump_init();
// Tænder pumpen
void pump_turn_on();
// Slukker pumpen
void pump_turn_off();
// Tænder for et angivet antal milisekunder, slukker derefter
void pump_turn_on_duration(int ms);
// Tænder for pumpen, og vandet et bestemt antal ml
void pump_turn_on_amount(int ml);