#include "task_connection_timeout.h"
#include "reboot.h"

extern int seconds_to_timeout = 60;


void task_connection_timeout_run() {
    if (seconds_to_timeout <= 0) {
        reboot();
    } else {
        seconds_to_timeout--;
    }
}