#include "LeituraPots.h"

double LeituraPots::getSetpointRPM() {
    int leitura = analogRead(PIN_SETPOINT);
    Serial.print(" SP"); Serial.print(analogRead(PIN_SETPOINT)); Serial.print(" ");

    // Setpoint de 0 a 1000 RPM
    return map(leitura, 0, 1023, 0, 1000); 
}

void LeituraPots::getGanhosPID(double &kp, double &ki, double &kd) {
    
    // Leitura e Mapeamento dos Ganhos
    double novoKp = (analogRead(PIN_KP) / 1023.0) * KP_MAX;
    Serial.print(" P"); Serial.print(analogRead(PIN_KP)); Serial.print(" ");
    
    double novoKi = (analogRead(PIN_KI) / 1023.0) * KI_MAX;
    Serial.print(" I"); Serial.print(analogRead(PIN_KI)); Serial.print(" ");
    
    double novoKd = (analogRead(PIN_KD) / 1023.0) * KD_MAX;
    Serial.print(" D"); Serial.print(analogRead(PIN_KD)); Serial.println(" ");

    // Atualiza apenas se houver mudança significativa
    if (fabs(novoKp - _kpAtual) > 0.01 || fabs(novoKi - _kiAtual) > 0.01 || fabs(novoKd - _kdAtual) > 0.01) {
        _kpAtual = novoKp;
        _kiAtual = novoKi;
        _kdAtual = novoKd;
    }

    // Retorna os valores atuais para o PID no main.cpp
    kp = _kpAtual;
    ki = _kiAtual;
    kd = _kdAtual;
}

