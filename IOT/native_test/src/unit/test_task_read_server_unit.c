#include "unity.h"
#include "task_read_server.h"
#include "mocks/Mockwaterpump.h"
#include "mocks/Mockserver_connector.h"
#include "mocks/Mocktask_connection_timeout.h"
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 100
bool _tcp_string_received;
char string_received[MAX_STRING_LENGTH];
int seconds_to_timeout;

void setUp(void)
{
    seconds_to_timeout = 0;
    _tcp_string_received = false;
    memset(string_received, 0, sizeof(string_received));
}

// task_read_server_init()
void test_task_read_server_init_Should_InitializeWaterPump_When_Called(void)
{
    pump_init_Expect();
    task_read_server_init();
}

// happy path
void test_task_read_server_run_ShouldTurnOnWaterPump_When_ValidMessageFromServerReceived(void)
{
    strcpy(string_received, "water,99;");

    pump_turn_on_amount_Expect(99);

    _tcp_string_received = true;

    task_read_server_run();

    TEST_ASSERT_FALSE(_tcp_string_received);
    TEST_ASSERT_EQUAL_CHAR('\0', string_received[0]);
}

void test_task_read_server_run_ShouldNotProceed_When_NoMessageFromServerReceived(void)
{
    _tcp_string_received = false;

    task_read_server_run();
}

void test_task_read_server_run_ShouldNotPumpOutWater_When_ZeroAmount(void)
{
    strcpy(string_received, "water,0;");

    _tcp_string_received = true;

    task_read_server_run();
}

void test_task_read_server_run_ShouldTurnOnWaterPump_When_LowerBoundaryAmount(void)
{
    strcpy(string_received, "water,1;");

    _tcp_string_received = true;

    pump_turn_on_amount_Expect(1);

    task_read_server_run();
}

void test_task_read_server_run_ShouldNotPumpOutWater_When_NegativeAmount(void)
{
    strcpy(string_received, "water,-1;");

    _tcp_string_received = true;

    task_read_server_run();
}

void test_task_read_server_run_ShouldTurnOnWaterPump_When_MaxAmount(void)
{
    strcpy(string_received, "water,100;");
    _tcp_string_received = true;

    pump_turn_on_amount_Expect(100);

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_ExceedsMaxAmount(void)
{
    strcpy(string_received, "water,101;");
    _tcp_string_received = true;

    task_read_server_run();
}

void test_task_read_server_run_ShouldIgnore_When_WrongMessageFormat_MissingComma(void)
{
    strcpy(string_received, "water500;"); // mangler komma
    _tcp_string_received = true;

    task_read_server_run();

    TEST_ASSERT_FALSE(_tcp_string_received);
}

void test_task_read_server_run_ShouldIgnore_When_WrongMessageFormat_MultiplesCommas(void)
{
    strcpy(string_received, "water,,500;"); // for mange kommaer

    _tcp_string_received = true;

    // Ingen pump_turn_on_amount_Expect, da forventningen er 0 kald.

    task_read_server_run();

    TEST_ASSERT_FALSE(_tcp_string_received);
}

void test_task_read_server_run_ShouldIgnore_When_EmptyString(void)
{
    strcpy(string_received, ""); // forkert format
    _tcp_string_received = true;

    task_read_server_run();

    TEST_ASSERT_FALSE(_tcp_string_received);
}

void test_task_read_server_run_ShouldIgnore_When_EmptyValue(void)
{
    strcpy(string_received, "water,;"); // for mange kommaer
    _tcp_string_received = true;

    task_read_server_run();

    TEST_ASSERT_FALSE(_tcp_string_received);
}

void test_task_read_server_run_ShouldIgnore_When_UnknownCommand_NotWaterType(void)
{
    strcpy(string_received, "nikolaj,67;"); // Forkert type
    _tcp_string_received = true;

    task_read_server_run();

    TEST_ASSERT_FALSE(_tcp_string_received);
}

void test_task_read_server_run_ShouldTurnOnWaterPump_WithMultipleMessages(void)
{
    // Første besked:
    strcpy(string_received, "water,67;");
    _tcp_string_received = true;
    pump_turn_on_amount_Expect(67);
    task_read_server_run();

    // Anden besked:
    strcpy(string_received, "water,42;");
    _tcp_string_received = true;
    pump_turn_on_amount_Expect(42);
    task_read_server_run();

    TEST_ASSERT_FALSE(_tcp_string_received);
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenPingManyIsReceived(void)
{
    strcpy(string_received, "ping,30;");
    _tcp_string_received = true;
    task_read_server_run();

    int expected_timeout = 30 + TIME_SERVER_PING_SENSITIVTY_SECONDS;
    TEST_ASSERT_EQUAL(expected_timeout, seconds_to_timeout);

    TEST_ASSERT_EQUAL_CHAR('\0', string_received[0]);
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenPingZeroIsReceived(void)
{
    strcpy(string_received, "ping,0;");
    _tcp_string_received = true;
    task_read_server_run();

    int expected_timeout = TIME_SERVER_PING_SENSITIVTY_SECONDS;
    TEST_ASSERT_EQUAL(expected_timeout, seconds_to_timeout);

    TEST_ASSERT_EQUAL_CHAR('\0', string_received[0]);
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenExceptionPingZeroIsReceived(void)
{
    strcpy(string_received, "ping,sixseven;");
    _tcp_string_received = true;
    task_read_server_run();

    int expected_timeout = TIME_SERVER_PING_SENSITIVTY_SECONDS; // atoi giver 0 for value_str
    TEST_ASSERT_EQUAL(expected_timeout, seconds_to_timeout);

    TEST_ASSERT_EQUAL_CHAR('\0', string_received[0]);
}

void test_task_read_server_run_ShouldUpdateSecondsToTimeout_WhenNumberAndLetterPingZeroIsReceived(void)
{
    strcpy(string_received, "ping,67sixseven;");
    _tcp_string_received = true;
    task_read_server_run();

    int expected_timeout = 67 + TIME_SERVER_PING_SENSITIVTY_SECONDS; // atoi giver 67 for value_str
    TEST_ASSERT_EQUAL(expected_timeout, seconds_to_timeout);

    TEST_ASSERT_EQUAL_CHAR('\0', string_received[0]);
}