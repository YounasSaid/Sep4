#include "unity.h"
#include "server_connector.h"
#include "mocks/Mockwifi.h"
#include <string.h>

// Docs: https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md

void setUp(void)
{
    _tcp_string_received = false;
    memset(string_received, 0, MAX_STRING_LENGTH);
}

WIFI_TCP_Callback_t captured_callback_when_message_received;

// Hjælpefunktion
WIFI_ERROR_MESSAGE_t init_callback_stub(char *IP, uint16_t port, WIFI_TCP_Callback_t callback_when_message_received, char *received_message_buffer, int num_calls)
{
    captured_callback_when_message_received = callback_when_message_received;
    return WIFI_OK;
}

// Happy path
void test_server_connector_init_SuccessfullySendAuthAndId_WhenEverythingIsSuccessful_Returns_1(void)
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

    int returnResult = server_connector_init(test_id);

    TEST_ASSERT_EQUAL(1, returnResult);
}

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

void test_server_connector_init_TCPStringReceived_Received_WhenCalledSuccessfully(void)
{
    uint8_t test_id = 67;

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_StubWithCallback(init_callback_stub);

    server_connector_init(test_id);

    TEST_ASSERT_NOT_NULL(captured_callback_when_message_received);

    captured_callback_when_message_received("Hej 67 fra server");

    TEST_ASSERT_TRUE(_tcp_string_received);
    TEST_ASSERT_EQUAL_STRING("Hej 67 fra server", string_received);
}

void test_server_connector_init_MessageReceived_IsTooLong_PreventsOverflow(void)
{
    uint8_t test_id = 67;

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_StubWithCallback(init_callback_stub);
    server_connector_init(test_id);

    char too_long_string[167];
    memset(too_long_string, 'O', 167);
    too_long_string[166] = '\0';

    captured_callback_when_message_received(too_long_string);

    // Expected = ikke overflow
    int expected_max_length = MAX_STRING_LENGTH - 1;

    TEST_ASSERT_EQUAL_INT(expected_max_length, strlen(string_received));

    // Sidste karakter i string_received burde være '\0'
    TEST_ASSERT_EQUAL_CHAR('\0', string_received[MAX_STRING_LENGTH - 1]);

    TEST_ASSERT_TRUE(_tcp_string_received);
}

void test_server_connector_init_MessageReceived_IsEmpty_FlagIsStillSet(void)
{
    uint8_t test_id = 67;

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_StubWithCallback(init_callback_stub);
    server_connector_init(test_id);

    captured_callback_when_message_received("");

    TEST_ASSERT_EQUAL_STRING("", string_received);

    TEST_ASSERT_TRUE(_tcp_string_received);
}

void test_server_connector_init_MessageReceived_IsNull_IgnoresLogic(void)
{
    uint8_t test_id = 67;

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_StubWithCallback(init_callback_stub);
    server_connector_init(test_id);

    captured_callback_when_message_received(NULL);

    // Intet er sket, dvs. stadig false.
    TEST_ASSERT_FALSE(_tcp_string_received);
}

void test_server_connector_init_MultipleMessagesReceived_NewMessageOverwritesOld(void)
{
    uint8_t test_id = 67;

    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);
    wifi_command_create_TCP_connection_StubWithCallback(init_callback_stub);
    server_connector_init(test_id);

    captured_callback_when_message_received("Message 1: Hello, world!");

    captured_callback_when_message_received("Message 2");

    TEST_ASSERT_EQUAL_STRING("Message 2", string_received);
    TEST_ASSERT_TRUE(_tcp_string_received);
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