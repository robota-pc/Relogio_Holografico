#ifndef CONFIG_H
#define CONFIG_H

#ifdef ESP32_ENV
#include <Arduino.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
#endif

// Configurações de Wi-Fi
#ifdef ESP32_ENV
extern const char *ssid;
extern const char *password;
#endif

// Configurações do servidor web
#ifdef ESP32_ENV
extern const int serverPort;
extern WebServer server;
#endif

// Configurações do NeoPixel
#ifdef ESP32_ENV
extern bool modo;
extern const int LED_PIN;
extern const int LED_COUNT;
extern Adafruit_NeoPixel strip;
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
extern int sessoes;
extern int detect;
extern int N_giro;
extern unsigned long currentMicros;
extern unsigned long tempoSensor;
extern unsigned long tempo;
extern unsigned long historico[150];
extern unsigned long t_giro[5];
extern unsigned long t_arco;
extern unsigned long M_giro_antes;
extern unsigned long M_giro_atual;

#endif // CONFIG_H
