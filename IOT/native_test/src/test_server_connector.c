#include "unity.h"
#include "server_connector.h"
#include "mocks/Mockwifi.h"

// Docs: https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md

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