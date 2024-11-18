#ifndef CONFIG_H
#define CONFIG_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Configurações de Wi-Fi
#define WIFI_SSID "relogio"
#define WIFI_PASS "holografico"

// Configurações dos LEDs
#define LED_PIN GPIO_NUM_15
#define LED_COUNT 36

// Configuração do sensor
#define SENSOR_PIN GPIO_NUM_36

void wifi_init(void);
bool get_led_mode(void);
void set_led_mode(bool mode);

#endif // CONFIG_H
