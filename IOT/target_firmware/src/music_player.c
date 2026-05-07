#include "tone.h"
#include <util/delay.h>

void play_karry()
{
    int e3 = 165;
    int g3 = 196;
    int b3 = 247;
    int c4 = 261;
    // Første del
    tone_play(e3, 200);
    _delay_ms(200);
    tone_play(e3, 100);
    _delay_ms(200);
    tone_play(e3, 200);
    _delay_ms(200);
    tone_play(e3, 100);
    _delay_ms(200);
    tone_play(e3, 200);
    _delay_ms(200);
    tone_play(e3, 200);
    _delay_ms(200);
    tone_play(e3, 100);
    _delay_ms(200);
    tone_play(e3, 200);
    _delay_ms(200);
    tone_play(e3, 100);
    _delay_ms(200);
    tone_play(g3, 400);

    // Anden del
    tone_play(e3, 200);
    _delay_ms(200);
    tone_play(e3, 100);
    _delay_ms(200);
    tone_play(e3, 200);
    _delay_ms(200);
    tone_play(e3, 100);
    _delay_ms(200);
    tone_play(e3, 200);
    _delay_ms(200);

    tone_play(b3, 500);
    tone_play(c4, 500);
    tone_play(b3, 300);
}