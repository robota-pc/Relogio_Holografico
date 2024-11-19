#include "../include/web_server_handlers.h"
#include "../include/config.h"
#include "../include/utilities.h"
#include <Arduino.h>

/**
 * @brief Retorna os valores de t_giro em JSON para o gráfico.
 */
void handleTGiroData() {
  String json = "[";
  for (int i = 0; i < 150; i++) {  // Envia os últimos 50 valores
    json += String(historico[i]);  // Supondo valores cíclicos
    if (i < 149) json += ",";
  }
  json += "]";
  server.send(200, "application/json", json);
}

/**
 * @brief Retorna as variáveis do sistema em JSON.
 */
void handleSystemData() {
  String json = "{";
  json += "\"t_giro\":[" + String(historico[0]); // Exemplo com historico
  for (int i = 1; i < 5; i++) {
    json += "," + String(historico[i]);
  }
  json += "],";
  json += "\"t_arco\":" + String(t_arco) + ",";
  json += "\"anterior\":" + String(anterior) + ",";
  json += "\"novo\":" + String(novo) + ",";
  json += "\"qntimagens\":" + String(qntimagens) + ",";
  json += "\"sessoes\":" + String(sessoes) + ",";
  json += "\"numSetores\":" + String(numSetores) + ",";
  json += "\"modo\":" + String(modo) + ",";
  json += "\"detect\":" + String(detect);
  json += "}";
  server.send(200, "application/json", json);
}


/**
 * @brief Função para lidar com a rota raiz.
 */
void handleRoot() {
  String page = "<html><body>";
  page += "<h1>Sistema - Controle e Monitoramento</h1>";

  // Gráfico de t_giro
  page += "<h2>Gráfico dos Valores de t_giro</h2>";
  page += "<canvas id='tGiroChart' width='400' height='200'></canvas>";
  page += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
  page += "<script>";
  page += "const ctx = document.getElementById('tGiroChart').getContext('2d');";
  page += "const tGiroChart = new Chart(ctx, {";
  page += "  type: 'line',";
  page += "  data: {";
  page += "    labels: Array.from({length: 150}, (_, i) => i + 1),";
  page += "    datasets: [{";
  page += "      label: 'Valores de t_giro',";
  page += "      data: [],";
  page += "      borderColor: 'rgba(75, 192, 192, 1)',";
  page += "      backgroundColor: 'rgba(75, 192, 192, 0.2)',";
  page += "      tension: 0.1";
  page += "    }]";
  page += "  }";
  page += "});";

  // Atualizar gráfico dinamicamente
  page += "async function fetchTGiroData() {";
  page += "  const response = await fetch('/t_giro_data');";
  page += "  const data = await response.json();";
  page += "  tGiroChart.data.datasets[0].data = data;";
  page += "  tGiroChart.update();";
  page += "}";
  page += "setInterval(fetchTGiroData, 250);";

  page += "</script>";

  // Tabela de variáveis
  page += "<h2>Variáveis do Sistema</h2>";
  page += "<table border='1'>";
  page += "<thead><tr><th>Variável</th><th>Valor</th></tr></thead>";
  page += "<tbody id='systemDataTable'></tbody>";
  page += "</table>";

  // Atualizar tabela dinamicamente
  page += "<script>";
  page += "async function fetchSystemData() {";
  page += "  const response = await fetch('/system_data');";
  page += "  const data = await response.json();";
  page += "  const table = document.getElementById('systemDataTable');";
  page += "  table.innerHTML = '';"; // Limpar a tabela
  page += "  for (const [key, value] of Object.entries(data)) {";
  page += "    const row = `<tr><td>${key}</td><td>${Array.isArray(value) ? value.join(', ') : value}</td></tr>`;";
  page += "    table.innerHTML += row;";
  page += "  }";
  page += "}";
  page += "setInterval(fetchSystemData, 1000);"; // Atualizar a cada 1 segundo
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
  for (int i = 0; i < 7; i++) {
    page += "Valor " + String(i) + ": <input type='number' name='value" + String(i) + "'><br>";
  }
  page += "Valor 0 = peso valor antigo; valor atual = " + String(anterior) + "'><br>";
  page += " valor 1 = peso valor novo; valor atual = " + String(novo) + "'><br>";
  page += "valor 2 = qntimagen; valor atual = " + String(qntimagens) + "'><br>";
  page += "valor 3 = sessoes; valor atual = " + String(sessoes) + "'><br>";
  page += "valor 4 = cima; valor atual = " + String(cima) + "'><br>";
  page += "valor 5 = baixo; valor atual = " + String(baixo) + "'><br>";
  page += "valor 6 = volta; valor atual = " + String(volta) + "'><br>";
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
      float value = server.arg("value" + String(i)).toFloat() / 1000;
      float value2 = server.arg("value" + String(i)).toInt();
      Serial.println("Valor " + String(i) + ": " + String(value));
      // Aqui você pode atribuir os valores a variáveis específicas
      if (i == 0 ) {
        if(value != 0.0) {
          anterior = value;
          novo = 1 - anterior; 
        }
      }
      if (i == 1 ) {
        if (value != 0.0) {
          novo = value;
          anterior = 1 - novo;
        }
      }
      if (i == 2 ) {
        if (value2 != 0) {
          qntimagens = value2;
          numSetores = qntimagens*largura;
        }        
      }
      if (i == 3){
        sessoes = value2;
        if (value2 == 0){
          numSetores = qntimagens*largura;
        } else {
          numSetores = sessoes;
        }
      }
      if (i == 4 ) {
        cima = value2;
      }
      if (i == 5 ) {
        baixo = value2;
      }
      if (i == 6 ) {
        if (value2 != 0) volta = value2;
      }
    }
  }
  
  server.send(200, "text/plain", "Valores recebidos com sucesso.");
}

/**
 * @brief Função para ligar o LED.
 */
void handleLedOn() {
    modo = 1;
    server.send(200, "text/plain", "LED ligado");
}

/**
 * @brief Função para desligar o LED.
 */
void handleLedOff() {
    modo = 0; 
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