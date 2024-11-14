#include "config.h"

// Configurações de Wi-Fi
const char *ssid = "relogio";
const char *password = "holografico";

// Configurações do servidor web
const int serverPort = 80;
WebServer server(serverPort);

// Configurações do NeoPixel
const int LED_PIN = 15;
const int LED_COUNT = 36;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Configurações do sensor
const int SENSOR_PIN = 36;
const int NUM_SETORES = 87; // 3 * largura (largura = 29)

// Variáveis globais
int detect = 0;
int N_giro = 0;
unsigned long tempoSensor = 0;
unsigned long tempo = 0;
unsigned long t_giro[5] = {0};
unsigned long t_arco = 0;
unsigned long M_giro_antes = 0;
unsigned long M_giro_atual = 0;
