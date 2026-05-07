#include "unity.h"
#include "server_connector.h"
#include "mocks/Mockwifi.h"
#include <string.h>

// Docs: https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md

// Happy path
void test_server_connector_init_Succesfully_join_WIFI_and_TCP_retuns_1(void)
{
    // Det der skal mockes:
    // WIFI_ERROR_MESSAGE_t wifi_command_join_AP(char *ssid, char *password);
    // WIFI_ERROR_MESSAGE_t wifi_command_create_TCP_connection(char *IP, uint16_t port, WIFI_TCP_Callback_t callback_when_message_received, char *received_message_buffer);

    wifi_command_join_AP_ExpectAndReturn("", "", WIFI_OK);

    wifi_command_create_TCP_connection_ExpectAndReturn("98.71.68.49", 23, NULL, NULL, WIFI_OK);

    wifi_command_create_TCP_connection_IgnoreArg_callback_when_message_received();

    wifi_command_create_TCP_connection_IgnoreArg_received_message_buffer();

    int returnResult = server_connector_init();

    TEST_ASSERT_EQUAL(1, returnResult);
}

void test_server_connector_init_Succesfully_join_WIFI_Fails_TCP_returns_0(void)
{
    wifi_command_join_AP_ExpectAndReturn("", "", WIFI_OK);

    wifi_command_create_TCP_connection_ExpectAndReturn("98.71.68.49", 23, NULL, NULL, WIFI_FAIL);

    wifi_command_create_TCP_connection_IgnoreArg_callback_when_message_received();

    wifi_command_create_TCP_connection_IgnoreArg_received_message_buffer();

    int returnResult = server_connector_init();

    TEST_ASSERT_EQUAL(0, returnResult);
}

void test_server_connector_init_Fails_join_WIFI_returns_0(void)
{

    wifi_command_join_AP_ExpectAndReturn("", "", WIFI_FAIL);

    int returnResult = server_connector_init();

    TEST_ASSERT_EQUAL(0, returnResult);
}

// Hjælpefunktion
WIFI_ERROR_MESSAGE_t init_callback_stub(char *IP, uint16_t port, WIFI_TCP_Callback_t callback_when_message_received, char *received_message_buffer, int num_calls)
{
    callback_when_message_received("Hej 67 fra server");
    return WIFI_OK;
}

/*
void test_server_connector_init_TCPStringReceived_Received_WhenCalledSuccessfully(void)
{
    wifi_command_join_AP_IgnoreAndReturn(WIFI_OK);
    wifi_command_TCP_transmit_IgnoreAndReturn(WIFI_OK);

    wifi_command_create_TCP_connection_StubWithCallback(init_callback_stub);

    server_connector_init();

    TEST_ASSERT_TRUE(_tcp_string_received);
    TEST_ASSERT_EQUAL_STRING("Hej 67 fra server",string_received);
}

*/

/*

void test_prepare_wifi_line_buffer_adds_newline(void)
{

    char buffer[MAX_STRING_LENGTH] = "ssid123";

    server_connector_prepare_wifi_line_buffer(buffer, MAX_STRING_LENGTH);

    TEST_ASSERT_EQUAL_STRING("ssid123\r\n", buffer);
}

void test_prepare_wifi_line_buffer_Upper_boundary_of_max_length(void)
{

    char buffer[MAX_STRING_LENGTH];

    memset(buffer, 'A', MAX_STRING_LENGTH - 4);

    buffer[96] = '\0';

    server_connector_prepare_wifi_line_buffer(buffer, MAX_STRING_LENGTH);

    char expected[MAX_STRING_LENGTH];

    memset(expected, 'A', MAX_STRING_LENGTH - 4);
    expected[96] = '\r';
    expected[97] = '\n';
    expected[98] = '\0';

    TEST_ASSERT_EQUAL_STRING(expected, buffer);
}

void test_prepare_wifi_line_buffer_Out_of_bounds_Should_be_ignored(void)
{
    char buffer[MAX_STRING_LENGTH]={0};

    // Streng på 97 chars
    memset(buffer, 'A', 97); // Index 97 er '\0'

    server_connector_prepare_wifi_line_buffer(buffer, MAX_STRING_LENGTH);

    TEST_ASSERT_EQUAL_INT(97, strlen(buffer));

    TEST_ASSERT_EQUAL_INT8('\0',buffer[97]); // '\0' burde stadig stå på index 97
}

void test_prepare_wifi_line_buffer_At_limit_Should_succeed(void)
{
    char buffer[MAX_STRING_LENGTH]={0};

    // Streng på 96 chars
    memset(buffer, 'A', 96);

    server_connector_prepare_wifi_line_buffer(buffer, MAX_STRING_LENGTH);

    TEST_ASSERT_EQUAL_INT(98, strlen(buffer)); // 96 + 2 = 98

    TEST_ASSERT_EQUAL_INT8('\r',buffer[96]);
    TEST_ASSERT_EQUAL_INT8('\n',buffer[97]);
    TEST_ASSERT_EQUAL_INT8('\0',buffer[98]);
}

void test_prepare_wifi_line_buffer_empty_string(void)
{
    char buffer[MAX_STRING_LENGTH]={0};

    server_connector_prepare_wifi_line_buffer(buffer, MAX_STRING_LENGTH);

    TEST_ASSERT_EQUAL_INT(2, strlen(buffer)); // kun \r og \n

    TEST_ASSERT_EQUAL_INT8('\r',buffer[0]);
    TEST_ASSERT_EQUAL_INT8('\n',buffer[1]);
    TEST_ASSERT_EQUAL_INT8('\0',buffer[2]);
}
    */