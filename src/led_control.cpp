#include "led_control.h"
#include "config.h"
#include "driver/rmt.h"
#include "esp_log.h"

static const char *TAG = "LED_CONTROL";

void led_init() {
    ESP_LOGI(TAG, "Inicializando LEDs...");
    // Configuração do GPIO e inicialização dos LEDs
}

void led_loop() {
    ESP_LOGI(TAG, "Executando animação dos LEDs...");
    // Lógica para animação dos LEDs
}
