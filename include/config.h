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
extern const int LED_PIN;
extern const int LED_COUNT;
extern Adafruit_NeoPixel strip;
#endif

// Configurações do sensor
#ifdef ESP32_ENV
extern const int SENSOR_PIN;
extern const int NUM_SETORES;
#endif

// Variáveis globais
extern int detect;
extern int N_giro;
extern unsigned long tempoSensor;
extern unsigned long tempo;
extern unsigned long t_giro[5];
extern unsigned long t_arco;
extern unsigned long M_giro_antes;
extern unsigned long M_giro_atual;

#endif // CONFIG_H
