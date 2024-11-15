#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "../include/config.h"


void setup() {
  strip.begin();            // Inicializa a fita NeoPixel
  strip.show();             // Certifica-se de que todos os LEDs começam apagados
  Serial.begin(115200);     // Inicializa a comunicação serial (opcional para debug)
}


// Liga os LEDs em ordem, um por um
void ligarLedsSequencialmente(int delayTime) {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Define a cor do LED (vermelho)
    strip.show();                                   // Atualiza a fita com a nova cor
    delay(delayTime);                               // Espera antes de ligar o próximo LED
  }
}

// Desliga os LEDs em ordem, um por um
void desligarLedsSequencialmente(int delayTime) {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, 0); // Define a cor como "apagado"
    strip.show();              // Atualiza a fita para desligar o LED
    delay(delayTime);          // Espera antes de desligar o próximo LED
  }
}

void loop() {
  ligarLedsSequencialmente(100);  // Liga os LEDs um por um com atraso de 100ms
  delay(1000);                    // Espera 1 segundo com todos os LEDs ligados
  desligarLedsSequencialmente(100); // Desliga os LEDs um por um com atraso de 100ms
  delay(1000);                    // Espera 1 segundo com todos os LEDs desligados
}