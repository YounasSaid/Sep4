/*****************************************************************************
 * main.c
 *  Main application file for the IoT hardware drivers demo.
 *  This file initializes all the hardware drivers and demonstrates their
 *  functionality.
 *  Push button 2 on the shield during reset to enter continious sensor
 *  reading mode. Otherwise the program will run an interactive demo that
 *  allows you to test each driver individually by sending commands over UART.
 *  See interactive.c for details.
 *
 *  Author:  Erland Larsen
 *  Date:    2026-03-17
 *  Project: SPE4_API
 *****************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "interactive.h"
#include "button.h"
#include "uart_stdio.h"
#include "led.h"
#include "wifi.h"
#include "button.h"
#include "buzzer.h"
#include "dht11.h"
#include "servo.h"
#include "adc.h"
#include "light.h"
#include "soil.h"
#include "tone.h"
#include "timer.h"
#include "server_connector.h"
#include <stdint.h>
#include "scheduler.h"
#include "task_read_sensors.h"
#include "task_read_server.h"

#define MAX_STRING_LENGTH 100

#define DATA_TRANSMIT_TIMEOUT_MS 5000

uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
static int8_t _led_no = 0;
// static int16_t _x, _y, _z;

void timer_callback(uint8_t id)
{
    led_toggle((_led_no % 4) + 1); // Toggle LEDs in sequence 1-4
    _led_no++;
}

task_t task_list[] =
    {
        // period in ms, task to run, ready? (to run)
        {.period = 5000, .task_p = task_read_sensors_run, .ticks = 0},
        {.period = 6767, .task_p = task_read_server_run, .ticks = 0}}; // 67
uint8_t task_count = sizeof(task_list) / sizeof(task_t);

int main(void)
{

    if (UART_OK != uart_stdio_init(115200))
    {
        led_on(4); // Turn on LED4 to indicate error
        while (1)
            ;
    }

    led_init();
    button_init();
    light_init();
    wifi_init();
    servo_init(PWM_NORMAL);

        //    tone_init();

    task_read_sensors_init();
    task_read_server_init(); // TO-DO
    scheduler_init(task_list, task_count);



    sei(); // Enable global interrupts
    printf("DRIVHUS MÅLER 2000\n");

    if (server_connector_init() == 0)
    {
        // Kunne ikke oprette forbindelse til wifi eller server
        return 1;
    }

    while (1)
    {
        dispatcher();
    }

    return 1;
}