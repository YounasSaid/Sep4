#include "task_connection_timeout.h"
#include "reboot.h"

static int seconds_to_timeout = 60;

void task_connection_timeout_run() {
    if (seconds_to_timeout <= 0) {
        reboot();
    } else {
        seconds_to_timeout--;
    }
}

int task_connection_timeout_get_seconds_to_timeout() {
    return seconds_to_timeout;
}

void task_connection_timeout_set_seconds_to_timeout(int seconds) {
    seconds_to_timeout = seconds;
}