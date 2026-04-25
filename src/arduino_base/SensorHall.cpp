#include "SensorHall.h"

// --- Variáveis Globais ---
static volatile unsigned long contagemPulsos = 0;
double velocidadeAtualRPM = 0;
unsigned long tempoAnteriorMedicao = 0;

// --- Implementação da ISR ---
void contarGiro() {
  contagemPulsos++;
}

// --- Implementação das Funções ---

void inicializarSensorHall() {
    pinMode(PIN_HALL, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_HALL), contarGiro, FALLING); 
}

void atualizarRPM() {
    unsigned long tempoAtual = millis();
    unsigned long deltaTempo = tempoAtual - tempoAnteriorMedicao;
    
    if (deltaTempo >= INTERVALO_MEDICAO) {
        
        noInterrupts();
        double pulsosNoIntervalo = contagemPulsos;
        contagemPulsos = 0;
        interrupts();
        
        // RPM = (pulsosNoIntervalo / PPR) * (60000 / deltaTempo)
        velocidadeAtualRPM = (pulsosNoIntervalo / PULSOS_POR_REVOLUCAO) * (60000.0 / deltaTempo);
        
        tempoAnteriorMedicao = tempoAtual;
    }
}