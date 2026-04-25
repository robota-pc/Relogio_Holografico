#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "web_server_handlers.h"
#include "sensor_control.h"
#include "utilities.h"
#include "image_data.h"
#include "led_control.h"

void setup() {
  Serial.println("oi");
  // Inicialização das portas gnfdndhfn
  pinMode(LED_PIN, OUTPUT); 
  pinMode(SENSOR_PIN, INPUT);

  // Inicialização do NeoPixel
  strip.begin();
  strip.show(); // Apaga todos os pixels
  strip.setBrightness(255);

  // Inicialização serial
  Serial.begin(115200);

  /*
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
  server.on("/t_giro_data", HTTP_GET, handleTGiroData);  // Dados do gráfico
  server.on("/system_data", HTTP_GET, handleSystemData); // Nova rota para dados do sistema
  server.on("/send_values", HTTP_POST, handleSendValues);  // Envio de valores numéricos
  server.begin();
  */
}

void loop() {
  // Função de controle do sensor
  //sensorLoop();

  // Função para ativação dos LEDs
  if (modo == 1){
    ledloop();
  }

  // Lida com requisições do servidor web
  //server.handleClient();
}
