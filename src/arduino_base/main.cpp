#include <Arduino.h>
#include <PID_v1.h>
#include <rgb_lcd.h>

// Inclusão dos módulos criados
#include "Motor.h"
#include "SensorHall.h"
#include "LeituraPots.h"

// --- Instanciação de Componentes ---

// LCD
rgb_lcd lcd;

// Motor (PWM: 9, IN1: 7, IN2: 8)
Motor motor(9, 7, 8);

// Leitura dos Potenciômetros
LeituraPots pots;

// --- Variáveis de Controle PID ---
double Setpoint, Input, Output;
double Kp, Ki, Kd;

// Objeto PID (Inicializado no setup com os ganhos lidos)
PID meuPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

unsigned long ultimoTempoLcd = 0;
const unsigned long INTERVALO_LCD = 300; // Atualiza o LCD a cada 300ms

// --- Setup ---

void setup() {
    Serial.begin(9600);

    // 1. Inicializa o Motor e o Sensor Hall
    motor.inicializar();
    inicializarSensorHall(); // Função do SensorHall.cpp

    // 2. Inicializa o LCD
    lcd.begin(16, 2);
    lcd.setRGB(0, 50, 255); 
    lcd.print("Controle PID Motor");

    // 3. Configura o PID
    // Leitura inicial dos ganhos e Setpoint
    pots.getGanhosPID(Kp, Ki, Kd); // Le e define os ganhos iniciais
    Setpoint = pots.getSetpointRPM();
    
    meuPID.SetMode(AUTOMATIC);
    meuPID.SetOutputLimits(0, 255); 
    meuPID.SetTunings(Kp, Ki, Kd);
    
    delay(2000);
}

// --- Loop Principal ---

void loop() {
    // 1. Leitura de Entradas (Pots)
    Setpoint = pots.getSetpointRPM();
    
    // Atualiza Kp, Ki, Kd se os potenciômetros mudaram.
    // O ponteiro do PID deve ser atualizado se os ganhos mudarem.
    double novoKp, novoKi, novoKd;
    pots.getGanhosPID(novoKp, novoKi, novoKd);

    if (novoKp != Kp || novoKi != Ki || novoKd != Kd) {
        Kp = novoKp;
        Ki = novoKi;
        Kd = novoKd;
        meuPID.SetTunings(Kp, Ki, Kd);
    }
    
    // 2. Medição do Estado Atual (RPM)
    atualizarRPM(); // Calcula e armazena o RPM em velocidadeAtualRPM
    Input = velocidadeAtualRPM; 

    // 3. Cálculo PID e Controle
    meuPID.Compute(); 

    // 4. Aplica a saída PWM
    motor.setVelocidade((int)Output);

    // 5. Exibição no LCD (não-bloqueante)
    unsigned long tempoAtual = millis();
    if (tempoAtual - ultimoTempoLcd >= INTERVALO_LCD) {
        ultimoTempoLcd = tempoAtual;

        lcd.setCursor(0, 0);
        lcd.print("SP:"); 
        lcd.print(Setpoint, 0); 
        lcd.print("  "); // Limpeza
        
        lcd.setCursor(8, 0);
        lcd.print("RPM A:"); 
        lcd.print(Input, 0); 
        lcd.print("   "); // Limpeza

        lcd.setCursor(0, 1);
        lcd.print("Kp:"); lcd.print(Kp, 2); lcd.print(" ");
        lcd.print("Ki:"); lcd.print(Ki, 2); lcd.print(" ");
        lcd.print("Kd:"); lcd.print(Kd, 2); lcd.print("  ");
    }
}