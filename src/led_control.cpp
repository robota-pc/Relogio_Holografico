#include <Arduino.h>
#include "../include/sensor_control.h"
#include "../include/utilities.h"
#include "../include/config.h"
#include "../include/image_data.h"


int estado = 0;
int x = 0, y = 0;
int green = 0, red = 0, blue = 0;

void ledloop(){
    if (detect > 0) {
        x = 0;
    } else {
        if (currentMicros >= (tempo + t_arco)) {
            tempo += t_arco;
            
            for (int i = LED_COUNT - 1; i >= 0; i--) 
            {
                
                if (sessoes > 0){
                    if(x > sessoes) x = 1;
                    
                    if ( x%5 == 0){
                        strip.setPixelColor(i, strip.Color(255, 0, 0));
                    } else if(x%5 == 1) {
                        strip.setPixelColor(i, strip.Color(0, 0, 255));
                    } else if(x%5 == 2) {
                        strip.setPixelColor(i, strip.Color(0, 255, 0));
                    } else if(x%5 == 3) {
                        strip.setPixelColor(i, strip.Color(255, 255, 0));
                    } else {
                        strip.setPixelColor(i, strip.Color(255, 0, 255));
                    } 

                } else {
                        
                    if (i > (LED_COUNT - cima - 1)) {
                        strip.setPixelColor(i, strip.Color(75, 0, 130));
                        
                    } else if(i > baixo){
                        
                        estado = x + (y) * largura;
                        red = imagem1[estado][0]; 
                        green = imagem1[estado][1];
                        blue = imagem1[estado][2];
                        
                    
                        strip.setPixelColor( i , strip.Color(red, green, blue));
                    } else if (i <= baixo) {
                        strip.setPixelColor(i, strip.Color(255, 25, 0));
                    }
                }
                y += 1;
            }
            strip.show();  
            x += 1;
            if(x == largura - 1){
                x = 0;
            }
        }
    }
}