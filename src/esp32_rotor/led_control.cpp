#include "led_control.h"
#include "sensor_control.h"
#include "utilities.h"
#include "config.h"
#include "image_data.h"
#include "esp_timer.h"
#include "led_strip.h"

int estado = 0;
int x = largura - 1;
int green = 0, red = 0, blue = 0;

void ledloop() {
   currentMicros = esp_timer_get_time();

    if (currentMicros >= (tempo + 500)) {
        tempo += t_arco;
        for (int y = LED_COUNT - 1; y >= 0; y--) {
            if (sessoes > 0) {
                if (x > sessoes) x = 1;
                if (x % 5 == 0) {
                    led_strip_set_pixel(strip, y, 255, 0, 0);
                } else if (x % 5 == 1) {
                    led_strip_set_pixel(strip, y, 0, 0, 255);
                } else if (x % 5 == 2) {
                    led_strip_set_pixel(strip, y, 0, 255, 0);
                } else if (x % 5 == 3) {
                    led_strip_set_pixel(strip, y, 255, 255, 0);
                } else {
                    led_strip_set_pixel(strip, y, 255, 0, 255);
                }
            } else {
                if (y > (LED_COUNT - cima - 1)) {
                    led_strip_set_pixel(strip, y, 75, 0, 130);
                } else if (y > baixo) {
                    estado = (y - baixo) + x * largura;
                    red = imagem1[estado][0]; 
                    green = imagem1[estado][1];
                    blue = imagem1[estado][2];
                    
                    led_strip_set_pixel(strip, y, red, green, blue);
                } else if (y <= baixo) {
                    led_strip_set_pixel(strip, y, 255, 25, 0);
                }
            }
        }
        led_strip_refresh(strip);
        x -= 1;
        if (x < 0) {
            tempo = currentMicros;
            x = largura - 1;
        }
    }
}
