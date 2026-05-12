#include "unity.h"
#include "task_connection_timeout.h"
#include "mocks/Mockreboot.h"

void setUp(void) {
    seconds_to_timeout = 60;
}

void test_task_connection_timeout_run_callRebootWhenTimeIsZero(void) {
    seconds_to_timeout = 0;
    
    reboot_Expect();

    task_connection_timeout_run();
    TEST_ASSERT_EQUAL(0, seconds_to_timeout);
}

void test_task_connection_timeout_run_callRebootWhenTimeIsNegative(void) {
    seconds_to_timeout = -1;

    reboot_Expect();

    task_connection_timeout_run();
    TEST_ASSERT_EQUAL(-1, seconds_to_timeout);
}

void test_task_connection_timeout_run_dontCallRebootWhenTimeIsMany(void) {
    task_connection_timeout_run();
    TEST_ASSERT_EQUAL(59, seconds_to_timeout);
}

void test_task_connection_timeout_run_dontCallRebootWhenTimeIsOne(void) {
    seconds_to_timeout = 1;
    
    task_connection_timeout_run();
    TEST_ASSERT_EQUAL(0, seconds_to_timeout);
}