#include "../include/web_server_handlers.h"
#include "../include/config.h"
#include "../include/utilities.h"
#include <Arduino.h>

/**
 * @brief Retorna os valores de t_giro em JSON para o gráfico.
 */
void handleTGiroData() {
  String json = "[";
  for (int i = 0; i < 50; i++) {  // Envia os últimos 50 valores
    json += String(historico[i]);  // Supondo valores cíclicos
    if (i < 49) json += ",";
  }
  json += "]";
  server.send(200, "application/json", json);
}

/**
 * @brief Função para lidar com a rota raiz.
 */
void handleRoot() {
  String page = "<html><body>";
  page += "<h1>Gráfico Dinâmico dos Valores de t_giro</h1>";
  page += "<canvas id='tGiroChart' width='400' height='200'></canvas>";
  page += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
  page += "<script>";
  page += "const ctx = document.getElementById('tGiroChart').getContext('2d');";
  page += "const tGiroChart = new Chart(ctx, {";
  page += "  type: 'line',";
  page += "  data: {";
  page += "    labels: Array.from({length: 50}, (_, i) => i + 1),"; // Últimos 50 valores
  page += "    datasets: [{";
  page += "      label: 'Valores de t_giro',";
  page += "      data: [],"; // Inicialmente vazio
  page += "      borderColor: 'rgba(75, 192, 192, 1)',";
  page += "      backgroundColor: 'rgba(75, 192, 192, 0.2)',";
  page += "      tension: 0.1";
  page += "    }]";
  page += "  },";
  page += "  options: {";
  page += "    scales: {";
  page += "      y: { beginAtZero: true }";
  page += "    }";
  page += "  }";
  page += "});";

  // Atualizar gráfico dinamicamente
  page += "async function fetchTGiroData() {";
  page += "  const response = await fetch('/t_giro_data');";
  page += "  const data = await response.json();";
  page += "  tGiroChart.data.datasets[0].data = data;";
  page += "  tGiroChart.update();";
  page += "}";

  // Atualizar a cada 1 segundo
  page += "setInterval(fetchTGiroData, 1000);";

  page += "</script>";

  // Controle de LED
  page += "<h2>Controlar LED:</h2>";
  page += "<a href='/led/on'>Ligar LED</a><br>";
  page += "<a href='/led/off'>Desligar LED</a><br>";

  // Enviar matriz para nova imagem
  page += "<h2>Enviar uma Matriz para uma Nova Imagem:</h2>";
  page += "<form method='post' action='/send'>";
  page += "<textarea name='inputMatrix' rows='5' cols='40'></textarea><br>";
  page += "<input type='submit' value='Enviar Matriz'>";
  page += "</form>";

  // Enviar 5 valores numéricos
  page += "<h2>Enviar Valores das Variaveis:</h2>";
  page += "<form method='post' action='/send_values'>";
  for (int i = 0; i < 5; i++) {
    page += "Valor " + String(i) + ": <input type='number' name='value" + String(i) + "'><br>";
  }
   page += "Valor 1 = peso valor antigo; valor 2 = peso valor novo; valor 3 = qntimagens";
  page += "<input type='submit' value='Enviar Valores'>";
  page += "</form>";

  page += "</body></html>";

  server.send(200, "text/html", page);
}

/**
 * @brief Lida com o envio de valores numéricos.
 */
void handleSendValues() {
  for (int i = 0; i < 5; i++) {
    if (server.hasArg("value" + String(i))) {
      int value = server.arg("value" + String(i)).toInt();
      Serial.println("Valor " + String(i) + ": " + String(value));
      // Aqui você pode atribuir os valores a variáveis específicas
      if (i == 0 ) anterior = value;
      if (i == 1 ) novo = value;
      if (i == 2 ) qntimagens = value;
    }
  }
  server.send(200, "text/plain", "Valores recebidos com sucesso.");
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
    if (server.hasArg("inputMatrix")) {
        String inputString = server.arg("inputMatrix");
        int imagem[1080][4];  // Matriz para armazenar os valores da imagem

        // Tratamento de erro ao converter a string em matriz
        bool success = parseStringToIntMatrix(inputString, imagem);
        if (success) {
            Serial.println("Matriz imagem criada com sucesso.");
            server.send(200, "text/plain", "Matriz imagem criada com sucesso.");
        } else {
            server.send(400, "text/plain", "Erro ao processar a matriz de imagem.");
        }
    } else {
        server.send(400, "text/plain", "Requisição inválida: 'inputMatrix' não fornecida.");
    }
}