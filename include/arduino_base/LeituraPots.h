#ifndef LEITURAPOTS_H
#define LEITURAPOTS_H

#include <Arduino.h>

// --- Definições de Pinos ---
const int PIN_SETPOINT = A0; 
const int PIN_KP = A1;       
const int PIN_KI = A2;       
const int PIN_KD = A3;       

class LeituraPots {
public:
    // Lê e mapeia o potenciômetro do Setpoint (0-1000 RPM)
    double getSetpointRPM(); 
    
    // Lê e mapeia os ganhos PID (retorna o valor atual, ou atualiza o ponteiro)
    void getGanhosPID(double &kp, double &ki, double &kd);

private:
    // Mapeamentos específicos para ajuste fino (Ajuste estas faixas!)
    const double KP_MAX = 5.00; // Kp de 0.00 a 5.00
    const double KI_MAX = 0.20; // Ki de 0.00 a 0.20
    const double KD_MAX = 1.00; // Kd de 0.00 a 1.00

    double _kpAtual = 0.0;
    double _kiAtual = 0.0;
    double _kdAtual = 0.0;
};

#endif