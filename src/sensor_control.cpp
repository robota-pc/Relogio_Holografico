#include "sensor_control.h"
#include "config.h"
#include "driver/adc.h"
#include "esp_log.h"

static const char *TAG = "SENSOR_CONTROL";

void sensor_init() {
    ESP_LOGI(TAG, "Inicializando sensor...");
    // Configuração do ADC e GPIO do sensor
}

void sensor_loop() {
    ESP_LOGI(TAG, "Lendo sensor...");
    // Leitura do sensor e cálculo dos tempos
}
