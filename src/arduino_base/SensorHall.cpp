#include "SensorHall.h"

// --- Variáveis Globais ---
static volatile unsigned long contagemPulsos = 0;
double velocidadeAtualRPM = 0;
unsigned long tempoAnteriorMedicao = 0;
int marcacao = 0;

// --- Implementação da ISR ---
void contarGiro() {
    int valorBruto = analogRead(PIN_HALL);
    //Serial.print(marcacao); Serial.print(" / "); Serial.println(valorBruto);
    if(valorBruto > 800 && marcacao == 0){
        contagemPulsos++;
        marcacao ++;
    } else { 
        marcacao = 0;
    }
}

// --- Implementação das Funções ---

void inicializarSensorHall() {
    pinMode(PIN_HALL, INPUT);
}

void atualizarRPM() {
    unsigned long tempoAtual = millis();
    unsigned long deltaTempo = tempoAtual - tempoAnteriorMedicao;
    
    if (deltaTempo >= INTERVALO_MEDICAO) {
        
        double pulsosNoIntervalo = contagemPulsos;
        contagemPulsos = 0;
        
        // RPM = (pulsosNoIntervalo / PPR) * (60000 / deltaTempo)
        velocidadeAtualRPM = (pulsosNoIntervalo / PULSOS_POR_REVOLUCAO) * (60000.0 / deltaTempo);
        
        tempoAnteriorMedicao = tempoAtual;
    }
}