#include <Arduino.h>
#include "../include/sensor_control.h"
#include "../include/utilities.h"
#include "../include/config.h"

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

      Serial.println(currentMicros);
      Serial.println(tempoSensor);
      currentMicros = micros();
      t_giro[N_giro] = currentMicros - tempoSensor;
      tempoSensor = micros();
      Serial.println(t_giro[N_giro]);

      // Verificação para evitar divisão por zero
      if (t_giro[N_giro] == 0) {
        Serial.println("Erro: Tempo de giro é zero.");
        return;
      }

      // Cálculo do tempo médio de giro
      unsigned long sum = 0;
      for (int i = 0; i < 5; i++) {
        sum += t_giro[i] ;
      }
      M_giro_atual = sum / 5;
      M_giro_atual = filtro(M_giro_antes, M_giro_atual);
      M_giro_antes = M_giro_atual;
      t_giro[N_giro] = M_giro_atual;

      t_arco = M_giro_atual / numSetores;
      N_giro += 1;

      Serial.print("Tempo de giro [");
      Serial.print(N_giro);
      Serial.print("]: ");
      Serial.println(t_giro[N_giro]);

      Serial.print("Tempo por setor: ");
      Serial.println(t_arco);
      Serial.println(";\n");

      Serial.print("array de tempo: ");
      for (int  i = 0; i < 5; i++){
        Serial.print(t_giro[i]);
        Serial.print("; ");
      }
      Serial.println(";\n");


    } 
  } else {
    detect = 0;
  }

  // Atualização do tempo e controle dos LEDs
  currentMicros = micros();

}
