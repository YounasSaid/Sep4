#include <avr/wdt.h>


void reboot() {
    wdt_enable(WDTO_15MS);
    while(1) {

    }
}