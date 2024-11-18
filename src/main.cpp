#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "led_control.h"
#include "sensor_control.h"
#include "web_server.h"
#include "config.h"

static const char *TAG = "MAIN";

void app_main() {
    ESP_LOGI(TAG, "Inicializando...");

    // Inicialização do NVS
    ESP_ERROR_CHECK(nvs_flash_init());

    // Inicialização do Wi-Fi
    wifi_init();

    // Inicialização dos LEDs
    led_init();

    // Inicialização do sensor
    sensor_init();

    // Inicialização do servidor web
    web_server_init();

    // Loop principal
    while (true) {
        sensor_loop();
        if (get_led_mode() == 1) {
            led_loop();
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay para evitar sobrecarga
    }
}
