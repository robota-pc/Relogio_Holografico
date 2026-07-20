#include "sensor_control.h"
#include "utilities.h"
#include "config.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/adc.h"

static const char *TAG = "sensor_control";
static int h = 0;

/**
 * @brief Inicializa o ADC para o sensor
 */
void sensorInit() {
    adc1_config_width(ADC_WIDTH_BIT_10);
    // GPIO 34 é o canal 6 do ADC1
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_12);
    ESP_LOGI(TAG, "Sensor ADC inicializado no canal 6 (GPIO 34)");
}

/**
 * @brief Função principal de controle do sensor e cálculo de tempos.
 */
void sensorLoop() {
  int sensorValue = adc1_get_raw(ADC1_CHANNEL_6);

  // O sensor hall 49E detecta acima de 800
  if (sensorValue < 1000) {
    detect += 1;

    if (detect == 1) {
      if (N_giro == 5) {
        N_giro = 0;
      }
      ESP_LOGI(TAG, "\n;");
      ESP_LOGI(TAG, "Detectou (Valor: %d)", sensorValue);

      currentMicros = esp_timer_get_time() / 1000; // millisecond equivalent
      t_giro[N_giro] = currentMicros - tempoSensor;
      tempoSensor = esp_timer_get_time() / 1000;

      // Verificação para evitar divisão por zero
      if (t_giro[N_giro] == 0) {
        ESP_LOGE(TAG, "Erro: Tempo de giro é zero.");
        return;
      }

      // Cálculo do tempo médio de giro
      uint64_t sum = 0;
      for (int i = 0; i < 5; i++) {
        sum += t_giro[i] ;
      }
      M_giro_atual = sum / 5;
      M_giro_atual = filtro(M_giro_antes, M_giro_atual);
      M_giro_antes = M_giro_atual;
      t_giro[N_giro] = M_giro_atual;
      
      if (numSetores > 0) {
        if (volta == volta_restante) {
          t_arco = M_giro_atual / numSetores;
          volta_restante = 0;
        } else {
          volta_restante += 1;
        }
      }

      ESP_LOGI(TAG, "Tempo de giro [%d]: %llu", N_giro, (unsigned long long)t_giro[N_giro]);
      ESP_LOGI(TAG, "Tempo por setor: %llu;\n", (unsigned long long)t_arco);

      if (h == 149) {
        for (int i = 0; i < 149; i++) {
          historico[i] = historico[i+1];
        }
        historico[h] = t_giro[N_giro];
      } else {
        historico[h] = t_giro[N_giro];
        h += 1;
      }

      N_giro += 1;
    } 
  } else {
    detect = 0;
  }

  currentMicros = esp_timer_get_time() / 1000;
}
