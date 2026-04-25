#ifndef SENSORHALL_H
#define SENSORHALL_H

#include <Arduino.h>

// --- Variáveis Externas (compartilhadas com main.cpp) ---
extern double velocidadeAtualRPM;

// --- Constantes de Configuração ---
const int PIN_HALL = 2; // Pino Digital 2 (Interrupção 0 no Uno)
const unsigned int PULSOS_POR_REVOLUCAO = 2; // AJUSTE
const unsigned long INTERVALO_MEDICAO = 100; // ms

// --- Protótipos de Funções ---

// ISR: Chamada a cada pulso do sensor Hall
void contarGiro(); 

// Inicializa o pino e a interrupção
void inicializarSensorHall();

// Calcula e atualiza o RPM
void atualizarRPM();

#endif