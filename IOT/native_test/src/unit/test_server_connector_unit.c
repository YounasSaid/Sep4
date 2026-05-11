#include "unity.h"
#include "server_connector.h"
#include "mocks/Mockwifi.h"
#include <string.h>

// Docs: https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md

void setUp(void) {}
void tearDown(void) {}

void test_server_connector_init_Returns_1_WhenEverythingSucceeds(void)
{
    // Det der skal mockes:
    // WIFI_ERROR_MESSAGE_t wifi_command_join_AP(char *ssid, char *password);
    // WIFI_ERROR_MESSAGE_t wifi_command_create_TCP_connection(char *IP, uint16_t port, WIFI_TCP_Callback_t callback_when_message_received, char *received_message_buffer);
    // WIFI_ERROR_MESSAGE_t wifi_command_TCP_transmit(uint8_t * data, uint16_t length)

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);          // Auth
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);          // Plant ID

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_connector_init_SuccesfullyJoinWIFIWithCorrectParameters(void)
{

    wifi_command_join_AP_ExpectAndReturn(WIFI_SSID, WIFI_PASSWORD, WIFI_OK); // AP

    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);          // Auth
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);          // Plant ID

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_connector_init_SuccesfullySetupTCPWithCorrectParameters(void)
{

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK); // AP

    wifi_command_create_TCP_connection_ExpectAndReturn(SERVER_IP, 23, NULL, NULL, WIFI_OK); // TCP
    wifi_command_create_TCP_connection_IgnoreArg_callback_when_message_received();
    wifi_command_create_TCP_connection_IgnoreArg_received_message_buffer();

    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK); // Auth
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK); // Plant ID

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_connector_init_SuccesfullySendAuthWithCorrectParameters(void)
{
    char expected_auth[] = "auth," API_KEY ";";
    char expected_plant_id_message[] = "id,67;";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP

    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_auth, strlen(expected_auth), WIFI_OK); // Auth

    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_plant_id_message, strlen(expected_plant_id_message), WIFI_OK); // Plant ID

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_connector_init_SuccesfullySendPlantIdWithCorrectParameters(void)
{
    char expected_plant_id_message[] = "id,67;";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP
    wifi_command_TCP_transmit_ExpectAndReturn(NULL, 0, WIFI_OK); // Auth
    wifi_command_TCP_transmit_IgnoreArg_data();
    wifi_command_TCP_transmit_IgnoreArg_length();

    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_plant_id_message, strlen(expected_plant_id_message), WIFI_OK); // Plant ID

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_connector_init__Returns_0_WhenWifiFails(void)
{
    wifi_command_join_AP_ExpectAndReturn(WIFI_SSID, WIFI_PASSWORD, WIFI_FAIL);

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(0, result);
}

void test_server_connector_init_Returns_0_WhenTcpFails(void)
{
    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);

    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_FAIL);

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(0, result);
}

void test_server_connector_init_Returns_0_WhenAuthTransmitFails(void)
{

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP

    wifi_command_TCP_transmit_ExpectAndReturn(NULL, 0, WIFI_FAIL); // Auth
    wifi_command_TCP_transmit_IgnoreArg_data();
    wifi_command_TCP_transmit_IgnoreArg_length();

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(0, result);
}

void test_server_connector_init_Returns_0_WhenPlantIdTransmitFails(void)
{

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP
    wifi_command_TCP_transmit_ExpectAndReturn(NULL, 0, WIFI_OK); // Auth
    wifi_command_TCP_transmit_IgnoreArg_data();
    wifi_command_TCP_transmit_IgnoreArg_length();

    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_FAIL); // Plant ID

    int result = server_connector_init(67);

    TEST_ASSERT_EQUAL(0, result);
}

void test_server_connector_init_ShouldSendCorrectId_WhenIdIsZero(void)
{
    char expected_plant_id_message[] = "id,0;";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP

    wifi_command_TCP_transmit_ExpectAndReturn(NULL, 0, WIFI_OK); // Auth
    wifi_command_TCP_transmit_IgnoreArg_data();
    wifi_command_TCP_transmit_IgnoreArg_length();

    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_plant_id_message, strlen(expected_plant_id_message), WIFI_OK); // Plant ID

    int result = server_connector_init(0);

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_connector_init_ShouldSendCorrectId_WhenIdIsMaxValue(void)
{
    char expected_id_message[] = "id,255;";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP
    wifi_command_TCP_transmit_ExpectAndReturn(NULL, 0, WIFI_OK); // Auth
    wifi_command_TCP_transmit_IgnoreArg_data();
    wifi_command_TCP_transmit_IgnoreArg_length();
    // ID
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_id_message, strlen(expected_id_message), WIFI_OK);

    int result = server_connector_init(255);

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_connector_init_ShouldSend11_WhenIdIs267(void)
{
    // 267 - 256 = 11
    char expected_id_message[] = "id,11;";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);               // AP
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK); // TCP
    wifi_command_TCP_transmit_ExpectAndReturn(NULL, 0, WIFI_OK); // Auth
    wifi_command_TCP_transmit_IgnoreArg_data();
    wifi_command_TCP_transmit_IgnoreArg_length();
    // ID
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_id_message, strlen(expected_id_message), WIFI_OK);

    int result = server_connector_init(267); // Compiler warning

    TEST_ASSERT_EQUAL(1, result);
}

void test_server_server_connector_send_plant_id_SendsCorrectData(void)
{
    char expected_id_message[] = "id,67;";

    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_id_message, strlen(expected_id_message), WIFI_OK);
    WIFI_ERROR_MESSAGE_t result = server_connector_send_plant_id(67);

    TEST_ASSERT_EQUAL(WIFI_OK, result);
}

void test_server_server_connector_send_plant_id_Returns_WifiFail_WhenTransmitFails(void)
{

    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_FAIL);

    WIFI_ERROR_MESSAGE_t result = server_connector_send_plant_id(67);

    TEST_ASSERT_EQUAL(WIFI_FAIL, result);
}


