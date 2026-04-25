#include "config.h"

// Configurações de Wi-Fi
const char *ssid = "relogio";
const char *password = "holografico";

// Configurações do servidor web
const int serverPort = 80;
WebServer server(serverPort);

// Configurações do NeoPixel
bool modo = 1;
const int LED_PIN = 15;
const int LED_COUNT = 36;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Configurações do sensor
const int SENSOR_PIN = 36;

// Variáveis da imagem
int largura = 20;
int qntimagens = 2;
int numSetores = qntimagens*largura;
int cima = 8;
int baixo = 8;

// Variaveis do filtro
float anterior = 0.9;
float novo = 0.1;


// Variáveis globais
int volta = 2;
int volta_restante = 0;
int sessoes = 0;
int detect = 0;
int N_giro = 0;
unsigned long currentMicros = 0;
unsigned long tempoSensor = 0;
unsigned long tempo = 0;
unsigned long historico[150] = {0};
unsigned long t_giro[5] = {0}; 
unsigned long t_arco = 0;
unsigned long M_giro_antes = 0;
unsigned long M_giro_atual = 0;
