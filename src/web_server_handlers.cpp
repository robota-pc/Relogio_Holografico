#include "web_server_handlers.h"
#include "config.h"
#include "led_control.h"
#include "esp_log.h"

static const char *TAG = "web_server";

// Handler para ligar o LED
esp_err_t handle_led_on(httpd_req_t *req) {
    set_led_state(1);
    const char *resp = "LED ligado!";
    httpd_resp_send(req, resp, strlen(resp));
    return ESP_OK;
}

// Handler para desligar o LED
esp_err_t handle_led_off(httpd_req_t *req) {
    set_led_state(0);
    const char *resp = "LED desligado!";
    httpd_resp_send(req, resp, strlen(resp));
    return ESP_OK;
}

// Handler para enviar dados do sistema
esp_err_t handle_system_data(httpd_req_t *req) {
    char resp[512];
    snprintf(resp, sizeof(resp),
             "{\"anterior\":%d,\"novo\":%d,\"qntimagens\":%d,\"sessoes\":%d,\"numSetores\":%d,\"modo\":%d}",
             anterior, novo, qntimagens, sessoes, numSetores, modo);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp, strlen(resp));
    return ESP_OK;
}

// Inicia o servidor web
void start_webserver() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri_led_on = {
            .uri = "/led/on",
            .method = HTTP_GET,
            .handler = handle_led_on,
            .user_ctx = NULL};
        httpd_uri_t uri_led_off = {
            .uri = "/led/off",
            .method = HTTP_GET,
            .handler = handle_led_off,
            .user_ctx = NULL};
        httpd_uri_t uri_system_data = {
            .uri = "/system_data",
            .method = HTTP_GET,
            .handler = handle_system_data,
            .user_ctx = NULL};

        httpd_register_uri_handler(server, &uri_led_on);
        httpd_register_uri_handler(server, &uri_led_off);
        httpd_register_uri_handler(server, &uri_system_data);
    } else {
        ESP_LOGE(TAG, "Falha ao iniciar o servidor web");
    }
}

static const char *TAG = "web_server";

// Handler para enviar dados de `t_giro`
esp_err_t handle_t_giro_data(httpd_req_t *req) {
    char response[512];
    snprintf(response, sizeof(response), "[");
    for (int i = 0; i < 50; i++) {
        snprintf(response + strlen(response), sizeof(response) - strlen(response), "%d", historico[i]);
        if (i < 49) {
            strncat(response, ",", sizeof(response) - strlen(response) - 1);
        }
    }
    strncat(response, "]", sizeof(response) - strlen(response) - 1);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

// Handler para controlar o LED
esp_err_t handle_led_on(httpd_req_t *req) {
    set_led_state(1);
    const char *r
