#include "tone.h"
#include "music_player.h"
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

void play_confirm() {
    tone_play(780, 50);
    tone_play(880, 50);
    tone_play(987, 50);
}

void play_up() {
    tone_play(987, 100);
}

void play_down() {
    tone_play(780, 100);
}