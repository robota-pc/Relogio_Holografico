#include <Arduino.h>
#include <AUnit.h>
#include "../include/sensor_control.h"
#include "../include/config.h"

// Mock das variáveis globais
int detect = 0;
int N_giro = 0;
unsigned long tempoSensor = 0;
unsigned long tempo = 0;
unsigned long t_giro[5] = {0, 0, 0, 0, 0};
unsigned long t_arco = 0;
unsigned long M_giro_antes = 0;
unsigned long M_giro_atual = 0;

// Mock das funções de hardware
int analogRead(int pin) {
  // Simula o valor do sensor
  return 0;
}

unsigned long micros() {
  // Retorna um valor fixo para simulação
  return 1000000; // 1 segundo
}

test(sensorLoopTest) {
  // Chama a função que será testada
  sensorLoop();

  // Verifica se as variáveis globais foram atualizadas corretamente
  assertEqual(detect, 1);
  assertEqual(N_giro, 1);
  // Outros asserts podem ser adicionados conforme a lógica da função
}
