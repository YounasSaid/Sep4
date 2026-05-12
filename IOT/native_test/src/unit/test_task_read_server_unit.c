#include "unity.h"
#include "task_read_server.h"
#include "mocks/Mockwaterpump.h"
#include "mocks/Mockserver_connector.h"
#include "mocks/Mocktask_connection_timeout.h"
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 100

int seconds_to_timeout;

void setUp(void)
{
}

// task_read_server_init()
void test_task_read_server_init_Should_InitializeWaterPump_When_Called(void)
{
    pump_init_Expect();
    task_read_server_init();
}

static char message_to_return[MAX_STRING_LENGTH];

void callback_copy_string(char *buffer, size_t size, int num_calls)
{
    strncpy(buffer, message_to_return, size - 1);
    buffer[size - 1] = '\0';
}

// happy path
void test_task_read_server_run_ShouldTurnOnWaterPump_When_ValidMessageFromServerReceived(void)
{
    strcpy(message_to_return, "water,99;");
    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    pump_turn_on_amount_Expect(99);

    task_read_server_run();
}

void test_task_read_server_run_ShouldNotProceed_When_NoMessageFromServerReceived(void)
{

    server_connector_has_received_message_ExpectAndReturn(false);

    task_read_server_run();
}

void test_task_read_server_run_ShouldNotPumpOutWater_When_ZeroAmount(void)
{
    strcpy(message_to_return, "water,0;");

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_read_server_run();
}

void test_task_read_server_run_ShouldTurnOnWaterPump_When_LowerBoundaryAmount(void)
{
    strcpy(message_to_return, "water,1;");

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    pump_turn_on_amount_Expect(1);

    task_read_server_run();
}

void test_task_read_server_run_ShouldNotPumpOutWater_When_NegativeAmount(void)
{
    strcpy(message_to_return, "water,-1;");

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_read_server_run();
}

void test_task_read_server_run_ShouldTurnOnWaterPump_When_MaxAmount(void)
{
    strcpy(message_to_return, "water,100;");

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    pump_turn_on_amount_Expect(100);

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_ExceedsMaxAmount(void)
{
    strcpy(message_to_return, "water,101;");

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_WrongMessageFormat_MissingComma(void)
{
    strcpy(message_to_return, "water500;");

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_WrongMessageFormat_MultiplesCommas(void)
{

    strcpy(message_to_return, "water,,500;"); // for mange kommaer

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    // Ingen pump_turn_on_amount_Expect, da forventningen er 0 kald.

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_EmptyString(void)
{
    strcpy(message_to_return, ""); // forkert format

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_EmptyValue(void)
{
    strcpy(message_to_return, "water,;"); // for mange kommaer

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_UnknownCommand_NotWaterType(void)
{
    strcpy(message_to_return, "nikolaj,67;"); // Forkert type

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_read_server_run();
}

void test_task_read_server_run_ShouldTurnOnWaterPump_WithMultipleMessages(void)
{
    // Første besked:
    strcpy(message_to_return, "water,67;");
    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();
    pump_turn_on_amount_Expect(67);
    task_read_server_run();

    // Anden besked:
    strcpy(message_to_return, "water,42;");
    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();
    pump_turn_on_amount_Expect(42);
    task_read_server_run();
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenPingManyIsReceived(void)
{

    strcpy(message_to_return, "ping,30;");
    int expected_timeout = 30 + TIME_SERVER_PING_SENSITIVTY_SECONDS;

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_connection_timeout_set_seconds_to_timeout_Expect(expected_timeout);

    task_read_server_run();
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenPingZeroIsReceived(void)
{
    strcpy(message_to_return, "ping,0;");
    int expected_timeout = 0 + TIME_SERVER_PING_SENSITIVTY_SECONDS;

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_connection_timeout_set_seconds_to_timeout_Expect(expected_timeout);

    task_read_server_run();
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenExceptionPingZeroIsReceived(void)
{
    strcpy(message_to_return, "ping,sixseven;");
    int expected_timeout = 0 + TIME_SERVER_PING_SENSITIVTY_SECONDS; // atoi giver 0 for value_str

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_connection_timeout_set_seconds_to_timeout_Expect(expected_timeout);

    task_read_server_run();
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenNumberAndLetterPingZeroIsReceived(void)
{
    strcpy(message_to_return, "ping,67sixseven;");
    int expected_timeout = 67 + TIME_SERVER_PING_SENSITIVTY_SECONDS;

    server_connector_has_received_message_ExpectAndReturn(true);
    server_connector_get_received_message_StubWithCallback(callback_copy_string);
    server_connector_clear_received_message_Expect();

    task_connection_timeout_set_seconds_to_timeout_Expect(expected_timeout);

    task_read_server_run();
}
