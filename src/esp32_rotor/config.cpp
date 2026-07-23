#include "config.h"

// Configurações de Wi-Fi
const char *ssid = "ROBOTA";
const char *password = "robotarecomeco";

// Configurações do servidor web
const int serverPort = 80;
httpd_handle_t server = NULL;

// Configurações do NeoPixel
bool modo = 1;
const int LED_PIN = 15;
const int LED_COUNT = 36;
led_strip_handle_t strip = NULL;

// Configurações do sensor
const int SENSOR_PIN = 36;
int currentSensorValue = 0;

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
uint64_t currentMicros = 0;
uint64_t tempoSensor = 0;
uint64_t tempo = 0;
uint64_t historico[150] = {0};
uint64_t t_giro[5] = {0}; 
uint64_t t_arco = 0;
uint64_t M_giro_antes = 0;
uint64_t M_giro_atual = 0;
