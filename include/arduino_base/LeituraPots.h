#ifndef LEITURAPOTS_H
#define LEITURAPOTS_H

#include <Arduino.h>

// --- Definições de Pinos ---
const int PIN_SETPOINT = A2; 
const int PIN_KP = A0;       
const int PIN_KI = A3;       
const int PIN_KD = A1;     


class LeituraPots {
public:

    // Nova função de calibração que dura 5 segundos por padrão
    void inicializarPotenciometros(unsigned long tempoCalibracaoMs = 200);

    // Lê e mapeia o potenciômetro do Setpoint (0-1000 RPM)
    double getSetpointRPM(); 
    
    // Lê e mapeia os ganhos PID (retorna o valor atual, ou atualiza o ponteiro)
    void getGanhosPID(double &kp, double &ki, double &kd);

    // Variáveis para armazenar a calibração de cada potenciômetro
    // Min começa alto e Max começa baixo para serem sobrepostos na leitura
    int minSP = 100, maxSP = 900;
    int minKP = 100, maxKP = 900;
    int minKI = 100, maxKI = 900;
    int minKD = 100, maxKD = 900;

private:
    // Mapeamentos específicos para ajuste fino (Ajuste estas faixas!)
    const double KP_MAX = 5.00; // Kp de 0.00 a 5.00
    const double KI_MAX = 0.20; // Ki de 0.00 a 0.20
    const double KD_MAX = 1.00; // Kd de 0.00 a 1.00

    double _kpAtual = 0.0;
    double _kiAtual = 0.0;
    double _kdAtual = 0.0;


    // Função auxiliar para mapear números com casas decimais (double) e travar os limites
    double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);
};

#endif