#include <Arduino.h>
#include "../include/sensor_control.h"
#include "../include/utilities.h"
#include "../include/config.h"
#include "../include/image_data.h"


int estado = 0;
int x = largura - 1;
int green = 0, red = 0, blue = 0;

void ledloop(){
    /*

    if (currentMicros >= (tempo + t_arco)) {
        tempo += t_arco;
        
        for (int i = LED_COUNT - 1; i >= 0; i--)             {
            if ( x == y){
                Serial.println("");
                Serial.print("ligou");
                Serial.print(" x ");
                Serial.print(x);
                Serial.print(" i ");
                Serial.print(i);
                Serial.print(" y ");
                Serial.println(y);
                Serial.println("");
                strip.setPixelColor( i , strip.Color(255, 255, 255));
            }else {
                strip.setPixelColor( i , strip.Color(0, 0, 0));
            }
            Serial.print("i ");
            Serial.println(i);
            Serial.print("y ");
            Serial.println(y);
            y -= 1;
        }
        Serial.print("x ");
        Serial.println(x);
        y = LED_COUNT -1;
        x -= 1;

        strip.show();  
        if(x < 0){
            tempo = currentMicros;
            x = LED_COUNT -1;
        }
    }*/
   currentMicros = micros();

        if (currentMicros >= (tempo + 500)) {
            tempo += t_arco;
            for (int y = LED_COUNT - 1; y >= 0; y--) 
            {
                if (sessoes > 0){
                    if(x > sessoes) x = 1;
                    if ( x%5 == 0){
                        strip.setPixelColor(y, strip.Color(255, 0, 0));
                    } else if(x%5 == 1) {
                        strip.setPixelColor(y, strip.Color(0, 0, 255));
                    } else if(x%5 == 2) {
                        strip.setPixelColor(y, strip.Color(0, 255, 0));
                    } else if(x%5 == 3) {
                        strip.setPixelColor(y, strip.Color(255, 255, 0));
                    } else {
                        strip.setPixelColor(y, strip.Color(255, 0, 255));
                    } 
                } else {
                    if (y > (LED_COUNT - cima - 1)) {
                        strip.setPixelColor(y, strip.Color(75, 0, 130));
                    } else if(y > baixo){

                        estado = (y - baixo) + x * largura;
                        red = imagem1[estado][0]; 
                        green = imagem1[estado][1];
                        blue = imagem1[estado][2];

                        /*
                        Serial.print("x, ");
                        Serial.print(x);
                        Serial.print(" y, ");
                        Serial.print(y);
                        Serial.print(" estado, ");
                        Serial.print(estado);
                        Serial.println("");
                        Serial.print(red);
                        Serial.print(", ");
                        Serial.print(green);
                        Serial.print(", ");
                        Serial.print(blue);
                        Serial.println("");
                        delay(100);
                        */
                    
                        strip.setPixelColor( y , strip.Color(red, green, blue));
                    } else if (y <= baixo) {
                        strip.setPixelColor(y, strip.Color(255, 25, 0));
                    }
                }
            }
            strip.show();  
            x -= 1;
            if(x < 0){
                tempo = currentMicros;
                x = largura - 1;
            }
        }
}    
