#include "Motor.h"

Motor::Motor(int pinPWM, int pinIN1, int pinIN2) 
    : _pinPWM(pinPWM), _pinIN1(pinIN1), _pinIN2(pinIN2) {}

void Motor::inicializar() {
    pinMode(_pinPWM, OUTPUT);
    pinMode(_pinIN1, OUTPUT);
    pinMode(_pinIN2, OUTPUT);
    
    // Define a direção fixa (Ex: Sentido horário)
    digitalWrite(_pinIN1, HIGH);
    digitalWrite(_pinIN2, LOW);
}

void Motor::setVelocidade(int pwm) {
    // Garante que o PWM esteja no limite (0 a 255)
    if (pwm < 0) pwm = 0;
    if (pwm > 255) pwm = 255;
    
    analogWrite(_pinPWM, pwm);
}