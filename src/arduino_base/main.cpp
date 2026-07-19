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
const unsigned long INTERVALO_LCD = 200; // Atualiza o LCD a cada 200ms

// --- Setup ---

void setup() {
    Serial.begin(9600);
    
    // 1. Inicializa o Motor e o Sensor Hall
    motor.inicializar();
    inicializarSensorHall(); // Função do SensorHall.cpp
    // 2. Inicializa o LCD
    lcd.begin(16, 2);
    lcd.setRGB(0, 150, 255); 
    lcd.setCursor(0, 0);
    lcd.print("PID Motor");
    lcd.setCursor(0, 1);
    lcd.print("Giroflex");
    
    
    
    // 3. Configura o PID
    // Leitura inicial dos ganhos e Setpoint
    pots.getGanhosPID(Kp, Ki, Kd); // Le e define os ganhos iniciais
    Setpoint = pots.getSetpointRPM();
    
    meuPID.SetMode(AUTOMATIC);
    meuPID.SetOutputLimits(100, 255); 
    meuPID.SetTunings(Kp, Ki, Kd);
    
    delay(2000);
    
    lcd.setCursor(0, 0);
    lcd.print("Calibracao");
    lcd.setCursor(0, 1);
    lcd.print("POTENCIOMETROS");

    delay(2000);

    lcd.setCursor(0, 0);
    lcd.print("GIRE TODOS OS");
    lcd.setCursor(0, 1);
    lcd.print("POTENCIOMETROS!");

    pots.inicializarPotenciometros();

//     Serial.println("Calibracao finalizada!");
    lcd.setCursor(0, 0);
    lcd.print("Calibracao        ");
    lcd.setCursor(0, 1);
    lcd.print("finalizada!       ");

    delay(2000);
    
    lcd.setCursor(0, 0);
    lcd.print("minSP"); lcd.print(pots.minSP, 1); lcd.print(" maxSP"); lcd.print(pots.maxSP, 1); lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print("minP"); lcd.print(pots.minKP, 1); lcd.print(" maxP"); lcd.print(pots.maxKP, 1); lcd.print("   ");
    delay(5000);
    lcd.setCursor(0, 0);
    lcd.print("minI"); lcd.print(pots.minKI, 1); lcd.print(" maxI"); lcd.print(pots.maxKI, 1); lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print("minD"); lcd.print(pots.minKD, 1); lcd.print(" maxD"); lcd.print(pots.maxKD, 1); lcd.print("   ");

    delay(5000);

    lcd.setCursor(0, 0);
    lcd.print("                     ");
    lcd.setCursor(0, 1);
    lcd.print("                     ");

}

// --- Loop Principal ---

void loop() {
    // 0. Contagem continua do giro
    contarGiro();


    
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
        
    // temporizador para atualização dos outros valores
    unsigned long tempoAtual = millis();
    if (tempoAtual - ultimoTempoLcd >= INTERVALO_LCD) {
        // 5. Exibição no LCD 
        
        ultimoTempoLcd = tempoAtual;
        
        
        lcd.setCursor(0, 0);
        lcd.print("SP"); 
        lcd.print(Setpoint, 0); 
        lcd.print(" P"); lcd.print(Kp, 2); lcd.print("       ");
        
        lcd.setCursor(0, 1);
        lcd.print("RPM"); 
        lcd.print(Input, 0); 
        lcd.print(" I"); lcd.print(Ki*100, 0); 
        lcd.print(" D"); lcd.print(Kd*100, 0); 
        
    }
    
}