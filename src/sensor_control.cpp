#include <Arduino.h>
#include "include/sensor_control.h"
#include "include/utilities.h"
#include "include/config.h"

/**
 * @brief Função principal de controle do sensor e cálculo de tempos.
 */
void sensorLoop() {
  // Leitura do sensor com tratamento de erros
  int sensorValue = analogRead(SENSOR_PIN);
  if (sensorValue == -1) {
    Serial.println("Erro na leitura do sensor.");
    return;
  }

  if (sensorValue == 0) {
    detect += 1;

    if (detect == 1) {
      if (N_giro == 5) {
        N_giro = 0;
      }
      Serial.println("\n;");
      Serial.println("Detectou");

      unsigned long currentMicros = micros();
      t_giro[N_giro] = currentMicros - tempoSensor;

      // Verificação para evitar divisão por zero
      if (t_giro[N_giro] == 0) {
        Serial.println("Erro: Tempo de giro é zero.");
        return;
      }

      // Cálculo do tempo médio de giro
      unsigned long sumSquares = 0;
      for (int i = 0; i < 5; i++) {
        sumSquares += t_giro[i] * t_giro[i];
      }
      M_giro_atual = sqrt(sumSquares) / 5;
      M_giro_atual = filtro(M_giro_antes, M_giro_atual);
      M_giro_antes = M_giro_atual;

      t_arco = M_giro_atual / NUM_SETORES;
      N_giro += 1;

      Serial.print("Tempo de giro [");
      Serial.print(N_giro);
      Serial.print("]: ");
      Serial.println(t_giro[N_giro]);

      Serial.print("Tempo por setor: ");
      Serial.println(t_arco);
      Serial.println(";\n");
    } else if (detect == 2) {
      tempoSensor = micros();
      tempo = micros();
    }
  } else {
    detect = 0;
  }

  // Atualização do tempo e controle dos LEDs
  unsigned long currentMicros = micros();
  if (currentMicros >= (tempo + t_arco)) {
    tempo += t_arco;
    // Aqui você pode adicionar o código para atualizar os LEDs
  }
}
