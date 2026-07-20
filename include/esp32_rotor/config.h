#ifndef CONFIG_H
#define CONFIG_H

// Modos de Operação
#define MODE_NORMAL 0
#define MODE_TEST_OTA 1
#define MODE_TEST_SENSOR 2
#define MODE_TEST_LED 3

// Flag para definir qual parte testar
#define RUN_MODE MODE_NORMAL

#ifdef ESP32_ENV
#include <string>
#include "esp_http_server.h"
#include "led_strip.h"
#endif

// Configurações de Wi-Fi
#ifdef ESP32_ENV
extern const char *ssid;
extern const char *password;
#endif

// Configurações do servidor web
#ifdef ESP32_ENV
extern const int serverPort;
extern httpd_handle_t server;
#endif

// Configurações do NeoPixel
#ifdef ESP32_ENV
extern bool modo;
extern const int LED_PIN;
extern const int LED_COUNT;
extern led_strip_handle_t strip;
#endif

// Configurações do sensor
#ifdef ESP32_ENV
extern const int SENSOR_PIN;
#endif

// Variáveis da imagem
#ifdef ESP32_ENV
extern int largura;
extern int qntimagens;
extern int numSetores;
extern int cima;
extern int baixo;
#endif

// Variaveis do filtro
#ifdef ESP32_ENV
extern float anterior;
extern float novo;
#endif

// Variáveis globais
extern int volta;
extern int volta_restante;
extern int sessoes;
extern int detect;
extern int N_giro;
extern uint64_t currentMicros;
extern uint64_t tempoSensor;
extern uint64_t tempo;
extern uint64_t historico[150];
extern uint64_t t_giro[5];
extern uint64_t t_arco;
extern uint64_t M_giro_antes;
extern uint64_t M_giro_atual;

#endif // CONFIG_H
