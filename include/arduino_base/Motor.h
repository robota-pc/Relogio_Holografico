#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    // Construtor: recebe os pinos de controle
    Motor(int pinPWM, int pinIN1, int pinIN2); 
    
    // Inicializa os pinos (chamado no setup)
    void inicializar(); 
    
    // Aplica o valor de PWM (0 a 255)
    void setVelocidade(int pwm); 

private:
    int _pinPWM;
    int _pinIN1;
    int _pinIN2;
};

#endif