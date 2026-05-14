#include <stdio.h>
#include "task_read_sensors.h"
#include "soil.h"
#include "wifi.h"
#include "dht11.h"
#include "light.h"
#include "music_player.h"

static char wifi_measure_data[64];

void task_read_sensors_init()
{
    ADC_Error_t error = soil_init(ADC_PK0);

    if (error != ADC_OK)
    {
        printf("Failed to initialize soil moisture sensor");
        play_karry();
    }

    error = light_init();

    if (error != ADC_OK)
    {
        printf("Failed to initialize light sensor");
        play_karry();
    }
}

static void read_soil_sensor(uint16_t *soil_value)
{
    *soil_value = soil_measure_raw(ADC_PK0);

    if (*soil_value == UINT16_MAX)
    {
        printf("Failed to read from soil moisture sensor. Make sure it is initialized correctly.\n");
    }
    else
    {
        printf("Soil moisture:%u\n", *soil_value);
    }
}

static DHT11_ERROR_MESSAGE_t read_temperature_humidity_sensor(uint8_t *humidity_integer, uint8_t *humidity_decimal, uint8_t *temperature_integer, uint8_t *temperature_decimal)
{
    DHT11_ERROR_MESSAGE_t error = dht11_get(humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
    if (error == DHT11_OK)
    {
        printf("Temperature: %hhu.%hhu°C, Humidity: %hhu.%hhu%%\n", *temperature_integer, *temperature_decimal, *humidity_integer, *humidity_decimal);
    }
    else
    {
        printf("Failed to read DHT11 sensor data\n");
        play_karry();
    }
    return error;
}

static void read_light_sensor(uint16_t *light_level)
{
    *light_level = light_measure_raw();
    if (*light_level == UINT16_MAX)
    {
        printf("Failed to read from light sensor. Make sure it is initialized correctly.\n");
    }
    else
    {
        printf("Light level: %u (0-1023)\n", *light_level);
    }
}

void task_read_sensors_run()
{
    uint16_t soil_value;
    read_soil_sensor(&soil_value);

    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    DHT11_ERROR_MESSAGE_t error = read_temperature_humidity_sensor(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

    uint16_t light_level;
    read_light_sensor(&light_level);

    int length;

    if (error != DHT11_FAIL)
    {
        length = snprintf(wifi_measure_data, sizeof(wifi_measure_data), "soil,%u;temp,%u.%u;hum,%u.%u;light,%u;", soil_value, temperature_integer, temperature_decimal, humidity_integer, humidity_decimal, light_level);
    }
    else
    {
        length = snprintf(wifi_measure_data, sizeof(wifi_measure_data), "soil,%u;error,dht11_read_failed;light,%u;", soil_value, light_level);
    }
    printf("Sender sensordata til server: %s\n", wifi_measure_data);

    WIFI_ERROR_MESSAGE_t status = wifi_command_TCP_transmit((uint8_t *)wifi_measure_data, length);

    if (status != WIFI_OK)
    {
        printf("Fejl ved transmitering af sensordata\n");
    }
}
