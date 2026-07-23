#include "web_server_handlers.h"
#include "config.h"
#include "utilities.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include "esp_log.h"
#include "led_strip.h"
#include "esp_ota_ops.h"
#include "esp_system.h"

static const char *TAG = "web_handlers";

static void extract_param_val(const std::string& data, const std::string& param, std::string& val) {
    size_t start = data.find(param + "=");
    if (start != std::string::npos) {
        start += param.length() + 1;
        size_t end = data.find('&', start);
        if (end == std::string::npos) end = data.length();
        val = data.substr(start, end - start);
    }
}

/**
 * @brief Retorna os valores de t_giro em JSON para o gráfico.
 */
static esp_err_t handleTGiroData(httpd_req_t *req) {
  std::string json = "[";
  for (int i = 0; i < 150; i++) {
    json += std::to_string(historico[i]);
    if (i < 149) json += ",";
  }
  json += "]";
  httpd_resp_set_type(req, "application/json");
  return httpd_resp_send(req, json.c_str(), json.length());
}

/**
 * @brief Retorna as variáveis do sistema em JSON.
 */
static esp_err_t handleSystemData(httpd_req_t *req) {
  std::string json = "{";
  json += "\"t_giro\":[" + std::to_string(historico[0]);
  for (int i = 1; i < 5; i++) {
    json += "," + std::to_string(historico[i]);
  }
  json += "],";
  json += "\"t_arco\":" + std::to_string(t_arco) + ",";
  json += "\"anterior\":" + std::to_string(anterior) + ",";
  json += "\"novo\":" + std::to_string(novo) + ",";
  json += "\"qntimagens\":" + std::to_string(qntimagens) + ",";
  json += "\"sessoes\":" + std::to_string(sessoes) + ",";
  json += "\"numSetores\":" + std::to_string(numSetores) + ",";
  json += "\"modo\":" + std::to_string(modo) + ",";
  json += "\"detect\":" + std::to_string(detect) + ",";
  json += "\"sensorValue\":" + std::to_string(currentSensorValue) + ",";
  json += "\"magnetDetected\":" + std::string((currentSensorValue > 550 || currentSensorValue < 320) ? "true" : "false");
  json += "}";
  httpd_resp_set_type(req, "application/json");
  return httpd_resp_send(req, json.c_str(), json.length());
}

/**
 * @brief Função para lidar com a rota raiz.
 */
static esp_err_t handleRoot(httpd_req_t *req) {
  std::string page = R"HTML(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Dashboard do Rotor</title>
  <script src='https://cdn.jsdelivr.net/npm/chart.js'></script>
  <style>
    :root {
      --bg: #121212;
      --card-bg: #1e1e1e;
      --text: #e0e0e0;
      --accent: #00e5ff; /* Cyan neon para combinar com projetos de LED */
      --accent-hover: #00b3cc;
      --danger: #cf6679;
      --danger-hover: #b00020;
      --border: #333;
    }
    body {
      font-family: 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
      background-color: var(--bg);
      color: var(--text);
      margin: 0;
      padding: 20px;
    }
    h1 { text-align: center; color: var(--accent); margin-bottom: 30px; letter-spacing: 1px; }
    
    /* Layout em Grid (Responsivo) */
    .dashboard {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(340px, 1fr));
      gap: 20px;
      max-width: 1200px;
      margin: 0 auto;
    }
    
    /* Cartões */
    .card {
      background-color: var(--card-bg);
      border-radius: 12px;
      padding: 25px;
      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.4);
      border: 1px solid var(--border);
    }
    .card h2 {
      margin-top: 0;
      font-size: 1.25rem;
      border-bottom: 1px solid var(--border);
      padding-bottom: 10px;
      color: #fff;
    }
    
    /* Gráficos e Tabelas */
    canvas { width: 100% !important; max-height: 250px; }
    table { width: 100%; border-collapse: collapse; margin-top: 15px; }
    th, td { padding: 12px 10px; text-align: left; border-bottom: 1px solid var(--border); }
    th { color: var(--accent); font-weight: 600; }
    
    /* Botões */
    .btn-group { display: flex; gap: 10px; margin-bottom: 20px; }
    .btn {
      flex: 1; text-align: center; padding: 12px; border-radius: 6px;
      text-decoration: none; font-weight: bold; font-size: 0.95rem;
      background-color: var(--accent); color: #000; transition: 0.2s;
      border: none; cursor: pointer; display: block; width: 100%; box-sizing: border-box;
    }
    .btn:hover { background-color: var(--accent-hover); box-shadow: 0 0 8px var(--accent); }
    .btn-danger { background-color: var(--danger); color: #fff; }
    .btn-danger:hover { background-color: var(--danger-hover); box-shadow: 0 0 8px var(--danger); }
    
    /* Formulários */
    input[type='number'], textarea {
      width: 100%; padding: 12px; margin: 8px 0 16px 0; border-radius: 6px;
      border: 1px solid var(--border); background: #2c2c2c; color: var(--text);
      box-sizing: border-box; font-family: monospace; font-size: 1rem;
    }
    input[type='number']:focus, textarea:focus { outline: none; border-color: var(--accent); }
    .form-label { font-size: 0.85rem; color: #bbb; display: block; font-weight: 500;}
    .status-text { margin-top: 10px; font-size: 0.85rem; color: #999; line-height: 1.6; background: #181818; padding: 15px; border-radius: 6px;}
    .status-text b { color: #fff; }
  </style>
</head>
<body>
  <h1>Painel de Controle e Monitoramento</h1>
  <div class="dashboard">
    
    <!-- Bloco 1: Gráfico -->
    <div class="card" style="grid-column: 1 / -1;">
      <h2>Gráfico de t_giro</h2>
      <canvas id="tGiroChart"></canvas>
    </div>

    <!-- Bloco 2: Variáveis do Sistema -->
    <div class="card">
      <h2>Variáveis do Sistema</h2>
      <table>
        <thead><tr><th>Variável</th><th>Valor</th></tr></thead>
        <tbody id="systemDataTable"></tbody>
      </table>
    </div>

    <!-- Bloco 3: Controles Diretos -->
    <div class="card">
      <h2>Controle do LED</h2>
      <div class="btn-group">
        <a href="/led/on" class="btn">Ligar LED</a>
        <a href="/led/off" class="btn btn-danger">Desligar LED</a>
      </div>

      <h2>Enviar Nova Imagem</h2>
      <form method="post" action="/send">
        <label class="form-label">Matriz da Imagem</label>
        <textarea name="inputMatrix" rows="6" placeholder="Insira os dados da matriz aqui..."></textarea>
        <button type="submit" class="btn">Enviar Matriz</button>
      </form>
    </div>

    <!-- Bloco 4: Configuração de Variáveis -->
    <div class="card">
      <h2>Atualizar Parâmetros</h2>
      <form method="post" action="/send_values">
)HTML";

  // Injetando os formulários numericos no HTML de forma mais limpa
  for (int i = 0; i < 7; i++) {
    page += "<label class='form-label'>Valor " + std::to_string(i) + "</label>";
    page += "<input type='number' step='any' name='value" + std::to_string(i) + "'>";
  }

  page += R"HTML(
        <div class="status-text">
          <b>Valores Atuais:</b><br>
)HTML";

  // Injetando as variáveis atuais formatadas
  page += "[0] Peso Antigo: <b>" + std::to_string(anterior) + "</b><br>";
  page += "[1] Peso Novo: <b>" + std::to_string(novo) + "</b><br>";
  page += "[2] Qtd Imagens: <b>" + std::to_string(qntimagens) + "</b><br>";
  page += "[3] Sessões: <b>" + std::to_string(sessoes) + "</b><br>";
  page += "[4] Cima: <b>" + std::to_string(cima) + "</b><br>";
  page += "[5] Baixo: <b>" + std::to_string(baixo) + "</b><br>";
  page += "[6] Volta: <b>" + std::to_string(volta) + "</b><br>";

  page += R"HTML(
        </div>
        <button type="submit" class="btn" style="margin-top: 15px;">Salvar Valores</button>
      </form>
    </div>
  </div>

  <script>
    // Configuração do Gráfico (Tematizado para Escuro)
    const ctx = document.getElementById('tGiroChart').getContext('2d');
    const tGiroChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: Array.from({length: 150}, (_, i) => i + 1),
        datasets: [{
          label: 'Valores de t_giro',
          data: [],
          borderColor: '#00e5ff',
          backgroundColor: 'rgba(0, 229, 255, 0.1)',
          borderWidth: 2,
          pointRadius: 0,
          fill: true,
          tension: 0.3
        }]
      },
      options: {
        responsive: true,
        maintainAspectRatio: false,
        plugins: { legend: { labels: { color: '#e0e0e0' } } },
        scales: {
          x: { ticks: { color: '#aaa' }, grid: { color: '#333' } },
          y: { ticks: { color: '#aaa' }, grid: { color: '#333' } }
        }
      }
    });

    // Função assíncrona para buscar dados do gráfico
    async function fetchTGiroData() {
      try {
        const response = await fetch('/t_giro_data');
        if(response.ok) {
            const data = await response.json();
            tGiroChart.data.datasets[0].data = data;
            tGiroChart.update();
        }
      } catch(e) { console.log("Aguardando ESP32..."); }
    }
    setInterval(fetchTGiroData, 250);

    // Função assíncrona para buscar variáveis da tabela
    async function fetchSystemData() {
      try {
        const response = await fetch('/system_data');
        if(response.ok) {
            const data = await response.json();
            const table = document.getElementById('systemDataTable');
            table.innerHTML = '';
            for (const [key, value] of Object.entries(data)) {
              const formattedVal = Array.isArray(value) ? value.join(', ') : value;
              table.innerHTML += `<tr><td><b>${key}</b></td><td>${formattedVal}</td></tr>`;
            }
        }
      } catch(e) {}
    }
    setInterval(fetchSystemData, 1000);
  </script>
</body>
</html>
)HTML";

  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, page.c_str(), page.length());
}

/**
 * @brief Lida com o envio de valores numéricos.
 */
static esp_err_t handleSendValues(httpd_req_t *req) {
  char buf[200];
  int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
  if (ret <= 0) {
      if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
          httpd_resp_send_408(req);
      }
      return ESP_FAIL;
  }
  buf[ret] = '\0';
  std::string data(buf);

  for (int i = 0; i < 7; i++) {
    std::string val_str;
    extract_param_val(data, "value" + std::to_string(i), val_str);
    if (!val_str.empty()) {
      int value2 = 0;
      float value = 0.0;
      char *endptr = nullptr;
      long parsed = strtol(val_str.c_str(), &endptr, 10);
      if (endptr != val_str.c_str()) {
        value2 = (int)parsed;
        value = (float)value2 / 1000.0f;
      }

      ESP_LOGI(TAG, "Valor %d: %f", i, value);
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
  
  const char* resp = "Valores recebidos com sucesso.";
  return httpd_resp_send(req, resp, strlen(resp));
}

/**
 * @brief Função para ligar o LED.
 */
static esp_err_t handleLedOn(httpd_req_t *req) {
    modo = 1;
    const char* resp = "LED ligado";
    return httpd_resp_send(req, resp, strlen(resp));
}

/**
 * @brief Função para desligar o LED.
 */
static esp_err_t handleLedOff(httpd_req_t *req) {
    modo = 0; 
    led_strip_clear(strip);
    const char* resp = "LED desligado";
    return httpd_resp_send(req, resp, strlen(resp));
}

/**
 * @brief Função para processar o envio de dados.
 */
static esp_err_t handleSend(httpd_req_t *req) {
    char* buf = (char*)malloc(req->content_len + 1);
    if (!buf) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    
    int ret = httpd_req_recv(req, buf, req->content_len);
    if (ret <= 0) {
        free(buf);
        return ESP_FAIL;
    }
    buf[ret] = '\0';
    std::string data(buf);
    free(buf);

    std::string inputMatrix;
    extract_param_val(data, "inputMatrix", inputMatrix);

    if (!inputMatrix.empty()) {
        int imagem[1080][4]; 

        bool success = parseStringToIntMatrix(inputMatrix, imagem);
        if (success) {
            ESP_LOGI(TAG, "Matriz imagem criada com sucesso.");
            const char* resp = "Matriz imagem criada com sucesso.";
            return httpd_resp_send(req, resp, strlen(resp));
        } else {
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Erro ao processar a matriz de imagem.");
            return ESP_FAIL;
        }
    } else {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Requisição inválida: 'inputMatrix' não fornecida.");
        return ESP_FAIL;
    }
}

/**
 * @brief Handler OTA via WebServer (HTTP POST /update)
 */
static esp_err_t handleOTAUpdate(httpd_req_t *req) {
    esp_err_t err;
    esp_ota_handle_t update_handle = 0 ;
    const esp_partition_t *update_partition = NULL;

    ESP_LOGI(TAG, "Iniciando processo OTA...");

    update_partition = esp_ota_get_next_update_partition(NULL);
    if (update_partition == NULL) {
        ESP_LOGE(TAG, "Erro: Nenhuma partição OTA encontrada!");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_begin falhou (%s)", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    int received = 0;
    int remaining = req->content_len;
    char ota_write_data[1024];

    while (remaining > 0) {
        int to_read = (remaining > sizeof(ota_write_data)) ? sizeof(ota_write_data) : remaining;
        int recv_len = httpd_req_recv(req, ota_write_data, to_read);

        if (recv_len <= 0) {
            if (recv_len == HTTPD_SOCK_ERR_TIMEOUT) continue;
            ESP_LOGE(TAG, "Erro recebendo os dados do firmware!");
            esp_ota_abort(update_handle);
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        err = esp_ota_write(update_handle, (const void *)ota_write_data, recv_len);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "esp_ota_write falhou (%s)", esp_err_to_name(err));
            esp_ota_abort(update_handle);
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        received += recv_len;
        remaining -= recv_len;
    }

    err = esp_ota_end(update_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_end falhou (%s)", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition falhou (%s)", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    const char* resp = "OTA executado com sucesso! Reiniciando a placa...";
    httpd_resp_send(req, resp, strlen(resp));

    ESP_LOGI(TAG, "OTA Realizado. Reiniciando...");
    vTaskDelay(pdMS_TO_TICKS(1000));
    esp_restart();
    return ESP_OK;
}

void register_web_server_handlers(httpd_handle_t server) {
    httpd_uri_t root = { .uri = "/", .method = HTTP_GET, .handler = handleRoot, .user_ctx = NULL };
    httpd_uri_t led_on = { .uri = "/led/on", .method = HTTP_GET, .handler = handleLedOn, .user_ctx = NULL };
    httpd_uri_t led_off = { .uri = "/led/off", .method = HTTP_GET, .handler = handleLedOff, .user_ctx = NULL };
    httpd_uri_t send_post = { .uri = "/send", .method = HTTP_POST, .handler = handleSend, .user_ctx = NULL };
    httpd_uri_t t_giro_data = { .uri = "/t_giro_data", .method = HTTP_GET, .handler = handleTGiroData, .user_ctx = NULL };
    httpd_uri_t system_data = { .uri = "/system_data", .method = HTTP_GET, .handler = handleSystemData, .user_ctx = NULL };
    httpd_uri_t send_values = { .uri = "/send_values", .method = HTTP_POST, .handler = handleSendValues, .user_ctx = NULL };
    httpd_uri_t ota_update = { .uri = "/update", .method = HTTP_POST, .handler = handleOTAUpdate, .user_ctx = NULL };

    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &led_on);
    httpd_register_uri_handler(server, &led_off);
    httpd_register_uri_handler(server, &send_post);
    httpd_register_uri_handler(server, &t_giro_data);
    httpd_register_uri_handler(server, &system_data);
    httpd_register_uri_handler(server, &send_values);
    httpd_register_uri_handler(server, &ota_update);
}