#include "include/web_server_handlers.h"
#include "include/utilities.h"
#include "include/config.h"
#include "include/image_data.h"
#include <Arduino.h>

/**
 * @brief Função para lidar com a rota raiz.
 */
void handleRoot() {
  String page = "<html><body>";
  page += "<h1>Números:</h1>";
  // Exibir os 5 números com verificação de valores
  for (int i = 0; i < 5; i++) {
    page += "<p>" + String(i + 1) + ": " + String(t_giro[i]) + "</p>";
  }
  page += "<h2>Controlar LED:</h2>";
  page += "<a href='/led/on'>Ligar LED</a><br>";
  page += "<a href='/led/off'>Desligar LED</a><br>";
  page += "<h2>Enviar String:</h2>";
  page += "<form method='post' action='/send'>";
  page += "<input type='text' name='inputString'><br>";
  page += "<input type='submit' value='Enviar'>";
  page += "</form>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

/**
 * @brief Função para ligar o LED.
 */
void handleLedOn() {
  strip.fill(strip.Color(255, 255, 255), 0, strip.numPixels());
  strip.show();
  server.send(200, "text/plain", "LED ligado");
}

/**
 * @brief Função para desligar o LED.
 */
void handleLedOff() {
  strip.clear();
  strip.show();
  server.send(200, "text/plain", "LED desligado");
}

/**
 * @brief Função para processar o envio de dados.
 */
void handleSend() {
  if (server.hasArg("inputString")) {
    String inputString = server.arg("inputString");
    int imagem[1080][4];  // Matriz para armazenar os valores da imagem

    // Tratamento de erro ao converter a string em matriz
    bool success = parseStringToIntMatrix(inputString, imagem);
    if (success) {
      // Agora você pode usar a matriz 'imagem' conforme necessário
      Serial.println("Matriz imagem criada com sucesso.");
      server.send(200, "text/plain", "Matriz imagem criada com sucesso.");
    } else {
      server.send(400, "text/plain", "Erro ao processar a matriz de imagem.");
    }
  } else {
    server.send(400, "text/plain", "Requisição inválida: 'inputString' não fornecida.");
  }
}
