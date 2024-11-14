#ifndef CONFIG_H
#define CONFIG_H

#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

// Configurações de Wi-Fi
const char *ssid = "relogio";
const char *password = "holografico";

// Configurações do servidor web
const int serverPort = 80;
extern WebServer server;

// Configurações do NeoPixel
const int LED_PIN = 15;
const int LED_COUNT = 36;
extern Adafruit_NeoPixel strip;

// Configurações do sensor
const int SENSOR_PIN = 36;
const int NUM_SETORES = 87; // 3 * largura (largura = 29)

// Variáveis globais
extern int detect;
extern int N_giro;
extern unsigned long tempoSensor;
extern unsigned long tempo;
extern unsigned long t_giro[5];
extern unsigned long t_arco;
extern unsigned long M_giro_antes;
extern unsigned long M_giro_atual;

#endif
