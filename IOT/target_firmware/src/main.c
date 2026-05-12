#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
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
#include "waterpump.h"
#include "display.h"
#include "task_connection_timeout.h"
#include <avr/wdt.h>
#include "reboot.h"
#include "task_handle_plant.h"

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
        {.period = 267, .task_p = task_read_server_run, .ticks = 0},
        {.period = 1000, .task_p = task_connection_timeout_run, .ticks = 0},
        {.period = 200, .task_p = task_handle_plant_run, .ticks = 0}
    }; // 67
uint8_t task_count = sizeof(task_list) / sizeof(task_t);

int main(void)
{
    wdt_disable();

    led_init();

    if (UART_OK != uart_stdio_init(115200))
    {
        led_on(4); // Turn on LED4 to indicate error
        printf("UART ikke OK :(");
        reboot();
    }

    button_init();
    wifi_init();
    servo_init(PWM_NORMAL);
    display_init();
    task_read_sensors_init();
    task_read_server_init();
    task_handle_plant_init();
    scheduler_init(task_list, task_count);
    pump_init();

    sei(); // Enable global interrupts

    printf("DRIVHUS MÅLER 2000\n");
    display_int(get_plant_id());

    if (server_connector_init(get_plant_id()) == 0)
    {
        // Kunne ikke oprette forbindelse til wifi eller server
        reboot();
    }

    printf("Klar til at gøre ting\n");

    while (1)
    {
        dispatcher();
    }

    return 1;
}