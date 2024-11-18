#include "utilities.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

uint32_t get_system_uptime() {
    return xTaskGetTickCount() * portTICK_PERIOD_MS / 1000;
}
