#include <Arduino.h>
#include <WiFi.h>
#include "include/config.h"
#include "include/web_server_handlers.h"
#include "include/sensor_control.h"
#include "include/utilities.h"
#include "include/image_data.h"

// Instância do servidor web
WebServer server(serverPort);

// Instância do NeoPixel
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Variáveis globais
int detect = 0;
int N_giro = 0;
unsigned long tempoSensor = 0;
unsigned long tempo = 0;
unsigned long t_giro[5] = {0, 0, 0, 0, 0};
unsigned long t_arco = 0;
unsigned long M_giro_antes = 0;
unsigned long M_giro_atual = 0;

void setup() {
  // Inicialização das portas
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  // Inicialização do NeoPixel
  strip.begin();
  strip.show(); // Apaga todos os pixels
  strip.setBrightness(255);

  // Inicialização serial
  Serial.begin(115200);

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configuração das rotas do servidor web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led/on", HTTP_GET, handleLedOn);
  server.on("/led/off", HTTP_GET, handleLedOff);
  server.on("/send", HTTP_POST, handleSend);
  server.begin();
}

void loop() {
  // Função de controle do sensor
  sensorLoop();

  // Lida com requisições do servidor web
  server.handleClient();
}
