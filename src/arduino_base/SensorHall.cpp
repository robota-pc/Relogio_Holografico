#include "SensorHall.h"

// --- Variáveis Globais ---
static volatile unsigned long contagemPulsos = 0;
double velocidadeAtualRPM = 0;
unsigned long tempoAnteriorMedicao = 0;
bool imaDetectado = false;
unsigned long INTERVALO_MEDICAO; // ms


// Variáveis para o filtro PID
double rpmFiltrado = 0.0;

void contarGiro() {
    int valorBruto = analogRead(PIN_HALL);
    
    // HISTERESE: Cria uma faixa de segurança para evitar contagem dupla
    // Ajuste esses valores (800 e 600) dependendo da força do seu ímã!
    if (valorBruto > 600 && !imaDetectado) {
        imaDetectado = true; // O ímã chegou perto
        contagemPulsos++;    // Conta 1 giro
    } 
    else if (valorBruto < 500 && imaDetectado) {
        imaDetectado = false; // O ímã se afastou o suficiente para rearmar
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
        
        // Copia os pulsos e zera
        unsigned long pulsosCopiados = contagemPulsos;
        contagemPulsos = 0;
        
        // RPM Bruto
        double rpmBruto = ((double)pulsosCopiados / PULSOS_POR_REVOLUCAO) * (60000.0 / deltaTempo);
        
        // Filtro Passa-Baixa
        rpmFiltrado = (FATOR_SUAVIZACAO * rpmBruto) + ((1.0 - FATOR_SUAVIZACAO) * rpmFiltrado);
        velocidadeAtualRPM = rpmFiltrado;
        
        tempoAnteriorMedicao = tempoAtual;

        INTERVALO_MEDICAO = velocidadeAtualRPM < 300 ? 300 : (map(velocidadeAtualRPM, 300, 1500, 300, 100)); // Ajusta o intervalo de medição com base na velocidade
    }
}