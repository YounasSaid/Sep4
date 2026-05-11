#include "unity.h"
#include "server_connector.h"
#include "mocks/Mockwifi.h"
#include <string.h>

// Docs: https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md

WIFI_ERROR_MESSAGE_t check_id_message_callback(uint8_t* data, uint16_t length, int num_calls)
{

    return WIFI_OK;
}

// Happy path
/*void test_server_connector_init_SuccessfullySendAuthAndId_WhenEverythingIsSuccessful_Returns_1(void)
{
    // Det der skal mockes:
    // WIFI_ERROR_MESSAGE_t wifi_command_join_AP(char *ssid, char *password);
    // WIFI_ERROR_MESSAGE_t wifi_command_create_TCP_connection(char *IP, uint16_t port, WIFI_TCP_Callback_t callback_when_message_received, char *received_message_buffer);
    // WIFI_ERROR_MESSAGE_t wifi_command_TCP_transmit(uint8_t * data, uint16_t length)
    char expected_auth[] = "auth," API_KEY ";";
    uint8_t test_id = 67;
    char expected_id_message[] = "id,67;";

    wifi_command_join_AP_ExpectAndReturn(WIFI_SSID, WIFI_PASSWORD, WIFI_OK);
    wifi_command_create_TCP_connection_ExpectAndReturn(SERVER_IP, 23, NULL, NULL, WIFI_OK);

    wifi_command_create_TCP_connection_IgnoreArg_callback_when_message_received();
    wifi_command_create_TCP_connection_IgnoreArg_received_message_buffer();

    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_auth, strlen(expected_auth), WIFI_OK);
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_id_message, strlen(expected_id_message), WIFI_OK);
    wifi_command_TCP_transmit_IgnoreArg_data();
    int returnResult = server_connector_init(test_id);

    TEST_ASSERT_EQUAL(1, returnResult);
}
*/
void test_server_connector_init_SuccesfullyJoinWIFI_ButFailsTCPConnection_returns_0(void)
{
    uint8_t test_id = 67;

    wifi_command_join_AP_ExpectAndReturn(WIFI_SSID, WIFI_PASSWORD, WIFI_OK);

    wifi_command_create_TCP_connection_ExpectAndReturn(SERVER_IP, 23, NULL, NULL, WIFI_FAIL);
    wifi_command_create_TCP_connection_IgnoreArg_callback_when_message_received();
    wifi_command_create_TCP_connection_IgnoreArg_received_message_buffer();

    int returnResult = server_connector_init(test_id);

    TEST_ASSERT_EQUAL(0, returnResult);
}

void test_server_connector_init_FailToJoinWIFI_returns_0(void)
{
    uint8_t test_id = 67;

    wifi_command_join_AP_ExpectAndReturn(WIFI_SSID, WIFI_PASSWORD, WIFI_FAIL);

    int returnResult = server_connector_init(test_id);

    TEST_ASSERT_EQUAL(0, returnResult);
}

void test_server_connector_init_SuccesfullyJoinAndConnectWIFIAndTCP_ButAuthFails_returns_0(void)
{
    char expected_auth[] = "auth," API_KEY ";";
    uint8_t test_id = 67;

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK);

    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_auth, strlen(expected_auth), WIFI_FAIL);

    int returnResult = server_connector_init(test_id);

    TEST_ASSERT_EQUAL(0, returnResult);
}

void test_server_connector_init_ShouldSendCorrectId_WhenIdIsZero(void)
{
    uint8_t test_id = 0;
    char expected_id_message[] = "id,0;";
    char expected_auth[] = "auth," API_KEY ";";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK);

    // Auth
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_auth, strlen(expected_auth), WIFI_OK);

    // ID
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_id_message, strlen(expected_id_message), WIFI_OK);

    server_connector_init(test_id);
}
/* TODO: fix tests
void test_server_connector_init_ShouldSendCorrectId_WhenIdIsMaxValue(void)
{
    uint8_t test_id = 255;
    char expected_id_message[] = "id,255;";
    char expected_auth[] = "auth," API_KEY ";";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK);

    // Auth
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_auth, strlen(expected_auth), WIFI_OK);

    // ID
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_id_message, strlen(expected_id_message), WIFI_OK);

    server_connector_init(test_id);
}


void test_server_connector_init_IdMessageTransmissionFails_Returns_0(void)
{
    uint8_t test_id = 67;
    char expected_auth[] = "auth," API_KEY ";";
    char expected_id_msg[] = "id,67;";

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_IgnoreAndReturn(WIFI_OK);

    // Auth
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_auth, strlen(expected_auth), WIFI_OK);

    // ID sendes men status WIFI_FAIL
    wifi_command_TCP_transmit_ExpectAndReturn((uint8_t *)expected_id_msg, strlen(expected_id_msg), WIFI_FAIL);

    int returnResult = server_connector_init(test_id);

    TEST_ASSERT_EQUAL(0, returnResult);
}
*/