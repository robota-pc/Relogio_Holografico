#include <Adafruit_NeoPixel.h>
#include <Arduino.h>



int largura = 10;
int LED_COUNT = 36, um_quarto = (LED_COUNT/4), tres_quarto = (3*LED_COUNT/4)-3 ;
int LED_PIN = 15, sensor = 36;
int detect = 0, cont = 0;



int numSetores = largura,  j = 1, N_giro = 0;
long int estado = 1;
unsigned long int tempoSensor = 0, tempo = 0, t_giro[5] = {0,0,0,0,0}, t_arco = 0, M_giro_antes = 0, M_giro_atual = 0, aux_tempo1 = 0, aux_tempo2 = 0;

int imagemIndex = 0;

// NeoPixel
uint8_t green = 0, red = 0, blue = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
/*
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
*/


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(sensor, INPUT);

  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(255);

  
  Serial.begin(115200);

  for(int h = 0; h < largura ; h++ ){
    Serial.println(h);
    for( int f = 0; f < LED_COUNT; f++){

      estado = h + f  * largura;
      if ( (imagem1[estado][0] == 210) && (imagem1[estado][1] == 210) && (imagem1[estado][2]  == 210)) {
          imagem1[estado][0] = 0;
          imagem1[estado][1] = 0;
          imagem1[estado][2] = 0;
      }
      /*
      if ( (f < um_quarto) || (f > tres_quarto)){
        if( (h%2) == 1) {
          strip.setPixelColor(f, strip.Color(255, 0, 0));
        } else {
          strip.setPixelColor(f, strip.Color(0, 0, 255));
        }
      } 
      */

      /*
      Serial.print("(");
      Serial.print(imagem1[estado][0]);
      Serial.print(",");
      Serial.print(imagem1[estado][1]);
      Serial.print(",");
      Serial.print(imagem1[estado][2]);
      Serial.print(")");
      */

    }
    strip.show();   // Turn OFF all pixels ASAP
  }
}





void loop() {
  
if (analogRead(sensor) == 0) {
    detect += 1;
    if (detect == 1 ) {
      cont += 1;
      if (cont == 3){
        
        /*
        M_giro_atual = millis() - tempoSensor;
        t_arco = M_giro_atual / numSetores;
        tempoSensor = millis();
        tempo = millis();
        */
        cont = 0;


        if (N_giro == 5){ N_giro = 0;}
        t_giro[N_giro]  = micros() - tempoSensor;
        M_giro_atual = sqrt((t_giro[0]*t_giro[0] + t_giro[1]*t_giro[1] + t_giro[2]*t_giro[2] + t_giro[3]*t_giro[3] + t_giro[4]*t_giro[4])) / 5;
        M_giro_atual = filtro(M_giro_antes,M_giro_atual);
        M_giro_antes = M_giro_atual;
        t_arco = M_giro_atual / numSetores;
        N_giro += 1; 
        tempoSensor = millis();
        tempo = millis();


        strip.show();
        Serial.println("");
        Serial.println(":");
        Serial.println(M_giro_atual);
        Serial.println(t_arco);
        Serial.println(":");
        Serial.println("");
      }
      tempoSensor = millis();
      tempo = millis();
      
    } 
  } else {
  detect = 0;
  }


  if (millis() >= (tempo + t_arco)) {
    tempo += t_arco;
    aux_tempo1 = millis();

    for (int i = um_quarto; i < tres_quarto; i++) 
    {
      
      //if ((i < 7) || (i > 27)) {
        if( (j%2) == 1) {
          strip.setPixelColor(i, strip.Color(255, 0, 0));
        } else {
          strip.setPixelColor(i, strip.Color(0, 0, 255));
        }
      //} 
      /*else {
        estado = j + (i - 3)  * largura;

        strip.setPixelColor(LED_COUNT - i, strip.Color(imagem1[estado][0], imagem1[estado][1], imagem1[estado][2]));
      }*/
      
    }
    aux_tempo2 = millis();

    Serial.println(aux_tempo1);
    Serial.println(aux_tempo2);
    Serial.println(aux_tempo2 - aux_tempo1);



    strip.show();
    Serial.println("");
    Serial.println(":");
    Serial.println(j);
    Serial.println(":");
    Serial.println("");
    j += 1;
    
    if (j > largura) {
      j = 1;
    }
  }
  
}


unsigned long int filtro(unsigned long int y0,unsigned long int y1){
  unsigned long int y2 ;
  y2 = 0,9*y0 + 0,1*y1;
  return y2;
}
 
