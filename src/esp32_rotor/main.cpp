#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "led_strip.h"
#include "mdns.h"

#include "config.h"
#include "web_server_handlers.h"
#include "sensor_control.h"
#include "utilities.h"
#include "image_data.h"
#include "led_control.h"

static const char *TAG = "main";

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Tentando conectar novamente ao Wi-Fi...");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Conectado com IP: " IPSTR, IP2STR(&event->ip_info.ip));
    }
}

static void wifi_init_sta(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    
    // Para conectar em rede oculta (hidden SSID), o método de scan deve abranger todos canais
    wifi_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

static void initialise_mdns(void) {
    esp_err_t err = mdns_init();
    if (err) {
        ESP_LOGE(TAG, "Falha na inicializacao do mDNS: %d", err);
        return;
    }
    mdns_hostname_set("relogio");
    mdns_instance_name_set("Relogio Holografico");
    ESP_LOGI(TAG, "mDNS inicializado. Acesse http://relogio.local");
}

static httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        register_web_server_handlers(server);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void sensor_task(void *pvParameters) {
    while (1) {
        sensorLoop();
        vTaskDelay(1); 
    }
}

void led_task(void *pvParameters) {
    while (1) {
        if (modo == 1) {
            ledloop();
        }
        vTaskDelay(1); 
    }
}

extern "C" void app_main() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Iniciando sistema. RUN_MODE = %d", RUN_MODE);

    if (RUN_MODE == MODE_NORMAL || RUN_MODE == MODE_TEST_SENSOR) {
        sensorInit();
        xTaskCreatePinnedToCore(sensor_task, "sensor_task", 4096, NULL, 5, NULL, 1);
    }

    if (RUN_MODE == MODE_NORMAL || RUN_MODE == MODE_TEST_LED) {
        led_strip_config_t strip_config = {};
        strip_config.strip_gpio_num = LED_PIN;
        strip_config.max_leds = LED_COUNT;
        
        led_strip_rmt_config_t rmt_config = {};
        rmt_config.resolution_hz = 10 * 1000 * 1000; 

        ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &strip));
        led_strip_clear(strip);
        xTaskCreatePinnedToCore(led_task, "led_task", 4096, NULL, 4, NULL, 1);
    }

    if (RUN_MODE == MODE_NORMAL || RUN_MODE == MODE_TEST_OTA) {
        wifi_init_sta();
        initialise_mdns();
        server = start_webserver();
    }
}
