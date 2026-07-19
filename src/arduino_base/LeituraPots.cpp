#include "LeituraPots.h"

void LeituraPots::inicializarPotenciometros() {
//     Serial.println("Iniciando calibracao...");
//     Serial.println("GIRE TODOS OS POTENCIOMETROS DE PONTA A PONTA!");
    
    unsigned long tempoInicio = millis();
    
    // Fica lendo as portas analógicas até o tempo limite acabar
    while (millis() - tempoInicio < tempoCalibracaoMs) {
        
        int l_sp = analogRead(PIN_SETPOINT);
        if(l_sp < minSP) minSP = l_sp;
        if(l_sp > maxSP) maxSP = l_sp;

        int l_kp = analogRead(PIN_KP);
        if(l_kp < minKP) minKP = l_kp;
        if(l_kp > maxKP) maxKP = l_kp;

        int l_ki = analogRead(PIN_KI);
        if(l_ki < minKI) minKI = l_ki;
        if(l_ki > maxKI) maxKI = l_ki;

        int l_kd = analogRead(PIN_KD);
        if(l_kd < minKD) minKD = l_kd;
        if(l_kd > maxKD) maxKD = l_kd;
    }

}

double LeituraPots::mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
    // Garante que o valor de entrada não ultrapasse os limites da calibração (equivale ao constrain)
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;
    
    // Regra de 3 para floats/doubles
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double LeituraPots::getSetpointRPM() {
    int leitura = analogRead(PIN_SETPOINT);
    
    // Setpoint de 0 a 1000 RPM
    leitura = constrain(leitura, minSP, maxSP);
    return map(leitura, 0, 1023, RPMmin, RPMmax); // Ajuste para 0-1000 RPM
}

void LeituraPots::getGanhosPID(double &kp, double &ki, double &kd) {
    
    // Leitura e Mapeamento dos Ganhos
    int leituraKp = analogRead(PIN_KP);
    int leituraKi = analogRead(PIN_KI);
    int leituraKd = analogRead(PIN_KD);

    // Usamos o mapDouble passando os limites exatos da calibração de CADA porta analógica
    double novoKp = mapDouble(leituraKp, minKP, maxKP, 0.0, KP_MAX);
    double novoKi = mapDouble(leituraKi, minKI, maxKI, 0.0, KI_MAX);
    double novoKd = mapDouble(leituraKd, minKD, maxKD, 0.0, KD_MAX);

    // Atualiza apenas se houver mudança significativa
    if (fabs(novoKp - _kpAtual) > 0.05 || fabs(novoKi - _kiAtual) > 0.05 || fabs(novoKd - _kdAtual) > 0.05) {
        _kpAtual = novoKp;
        _kiAtual = novoKi;
        _kdAtual = novoKd;
    }

    // Retorna os valores atuais para o PID no main.cpp
    kp = _kpAtual;
    ki = _kiAtual;
    kd = _kdAtual;
}

