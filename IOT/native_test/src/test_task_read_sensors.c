#include "unity.h"
#include "task_read_sensors.h"
#include "adc.h"
#include "mocks/Mockwifi.h"
#include "mocks/Mocksoil.h"
#include "mocks/Mockdht11.h"
#include "mocks/Mocklight.h"
#include "mocks/Mockkarry_player.h"
#include <string.h>

static uint8_t fake_h_i, fake_h_d, fake_t_i, fake_t_d;

DHT11_ERROR_MESSAGE_t my_dht11_get_stub(uint8_t *h_i, uint8_t *h_d, uint8_t *t_i, uint8_t *t_d, int num_calls)
{
    *h_i = fake_h_i;
    *h_d = fake_h_d;
    *t_i = fake_t_i;
    *t_d = fake_t_d;
    return DHT11_OK;
}

DHT11_ERROR_MESSAGE_t dht11_fail_stub(uint8_t *h_i, uint8_t *h_d, uint8_t *t_i, uint8_t *t_d, int num_calls)
{
    return DHT11_FAIL;
}

void setUp(void)
{
    fake_h_i = 0;
    fake_h_d = 0;
    fake_t_i = 0;
    fake_t_d = 0;
    dht11_get_Stub(my_dht11_get_stub);
}

void test_task_read_sensors_run_Should_call_all_sensors_and_transmit_Once(void)
{

    fake_t_i = 6;
    fake_t_d = 7;
    fake_h_i = 6;
    fake_h_d = 7;

    soil_measure_raw_ExpectAndReturn(ADC_PK0, 67);
    light_measure_raw_ExpectAndReturn(67);

    const char *expected = "soil,67;temp,6.7;hum,6.7;light,67;";
    int length = strlen(expected);

    wifi_command_TCP_transmit_ExpectWithArrayAndReturn((uint8_t *)expected, length, length, WIFI_OK);

    task_read_sensors_run();
}

void test_task_read_sensors_init_Should_InitializeSoilSensor_When_Called(void)
{
    soil_init_ExpectAndReturn(ADC_PK0, ADC_OK); // skal kaldes med korrekt ADC pin: ADC_PK0

    light_init_ExpectAndReturn(ADC_OK);

    task_read_sensors_init();
}

void test_task_read_sensors_init_Should_PlayKarry_When_SoilInitFails(void)
{
    soil_init_ExpectAndReturn(ADC_PK0, ADC_ERROR_INVALID_CHANNEL);

    play_karry_Expect();

    light_init_ExpectAndReturn(ADC_OK);

    task_read_sensors_init();
}

void test_task_read_sensors_init_Should_PlayKarry_When_LightInitFails(void)
{
    soil_init_ExpectAndReturn(ADC_PK0, ADC_OK);

    light_init_ExpectAndReturn(ADC_ERROR_INVALID_REFERENCE);

    play_karry_Expect();

    task_read_sensors_init();
}

void test_task_read_sensors_run_Should_TransmitZeroValues_When_SensorsReturnZero(void)
{
    soil_measure_raw_ExpectAndReturn(ADC_PK0, 0);
    light_measure_raw_ExpectAndReturn(0);

    const char *expected = "soil,0;temp,0.0;hum,0.0;light,0;";
    int length = strlen(expected);

    wifi_command_TCP_transmit_ExpectWithArrayAndReturn((uint8_t *)expected, length, length, WIFI_OK);

    task_read_sensors_run();
}

void test_task_read_sensors_run_Should_HandleDHT11Error(void)
{
    dht11_get_Stub(dht11_fail_stub);

    soil_measure_raw_ExpectAndReturn(ADC_PK0, 50);
    light_measure_raw_ExpectAndReturn(100);
    
    play_karry_Expect();

    const char *expected = "soil,50;error,dht11_read_failed;light,100;";
    int length = strlen(expected);

    wifi_command_TCP_transmit_ExpectWithArrayAndReturn((uint8_t *)expected, length, length, WIFI_OK);

    task_read_sensors_run();
}