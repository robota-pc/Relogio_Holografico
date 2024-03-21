#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "relogio";
const char *password = "holografico";

WebServer server(80);

int numeros[5] = {123, 456, 789, 101, 112}; // Array com os 5 números

int largura = 29;
int LED_COUNT = 36;
int LED_PIN = 15, sensor = 36;
int detect = 0;

int numSetores = 8*largura, estado = 1, j = 0;
unsigned long int tempoSensor = 0, tempo = 0, t_giro = 0, t_arco = 0;

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

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/led/on", HTTP_GET, handleLedOn);
  server.on("/led/off", HTTP_GET, handleLedOff);
  server.on("/send", HTTP_POST, handleSend);
  server.begin();
}

void loop() {
  server.handleClient();

if (analogRead(sensor) == 0) {
    detect += 1;
    if (detect == 1) {
      Serial.println("detectou");
      t_giro = micros() - tempoSensor;
      t_arco = t_giro / numSetores;
      tempoSensor = micros();
      tempo = micros();
      Serial.println(t_giro);
      Serial.println(t_arco);
    }
  }




  if (micros() >= (tempo + t_arco)) {
    for (int i = 0; i < LED_COUNT; i++) 
    {
      if (i < 7) {
        strip.setPixelColor(i, strip.Color(255, 25, 0));
      } else 
      {
        estado = j + (i - 7) * largura;

        red = imagem1[estado][0];
        green = imagem1[estado][1];
        blue = imagem1[estado][2];

        /*
        if(green < 150){
          green = 0;
        }else {
          green = 250;
        }
        if(red < 125){
          red = 0;
        }else {
          red = 250;
        }
        if(blue <= 125){
          blue = 0;
        }else {
          blue = 250;
        }
        
        blue =50;
        green = 100;
        */
        if (blue && red && green == 255) {
          blue = 0;
          red = 0;
          green = 0;
        } //else {
          //red = 255;
         // green = 25;
          //blue = 0;
        //}



        strip.setPixelColor(LED_COUNT - i + 3, strip.Color(red, green, blue));
      }
      if (i > 26) {
        strip.setPixelColor(i, strip.Color(75, 0, 130));
      }
    }
    for (int i = 0; i < 3; i++) {
      strip.setPixelColor(i, strip.Color(255, 25, 0));
    }
    j += 1;
    tempo = micros();
    strip.show();
    //Serial.println(j);
  }
  if (j >= largura) {
    Serial.println(j);
    j = 0;
    detect = 0;
  }
  
}

void handleRoot() {
  String page = "<html><body>";
  page += "<h1>Números:</h1>";
  // Exibir os 5 números
  for (int i = 0; i < 5; i++) {
    page += "<p>" + String(i+1) + ": " + String(numeros[i]) + "</p>";
  }
  page += "<h2>Controlar LED:</h2>";
  page += "<a href='/led/on'>Ligar LED</a><br>";
  page += "<a href='/led/off'>Desligar LED</a><br>";
  page += "<h2>Enviar String:</h2>";
  page += "<form method='post' action='/send'>";
  page += "<input type='text' name='inputString'><br>";
  page += "<input type='submit' value='Enviar'>";
  page += "</form>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "LED ligado");
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED desligado");
}

void parseStringToIntMatrix(String inputString, int imagem[1080][4]) {
  // Remova os caracteres '{' e '}' da string
  inputString.remove(0, 1);
  inputString.remove(inputString.length() - 1, 1);
  
  // Divida a string em tokens separados por vírgula
  int rowIndex = 0;
  int colIndex = 0;
  String token;
  while (inputString.length() > 0) {
    int commaIndex = inputString.indexOf(',');
    if (commaIndex != -1) {
      token = inputString.substring(0, commaIndex);
      inputString.remove(0, commaIndex + 1);
    } else {
      token = inputString;
      inputString = "";
    }
    // Converter o token para int e atribuí-lo à matriz
    imagem[rowIndex][colIndex] = token.toInt();
    // Avançar para a próxima coluna
    colIndex++;
    // Se atingiu a última coluna, avançar para a próxima linha
    if (colIndex >= 4) {
      colIndex = 0;
      rowIndex++;
    }
  }
}

void handleSend() {
  if (server.hasArg("inputString")) {
    String inputString = server.arg("inputString");
    int imagem[1080][4]; // Matriz para armazenar os valores da imagem
    parseStringToIntMatrix(inputString, imagem);
    // Agora você pode usar a matriz 'imagem' conforme necessário
    Serial.println("Matriz imagem criada com sucesso.");
    server.send(200, "text/plain", "Matriz imagem criada com sucesso.");
  } else {
    server.send(400, "text/plain", "Requisição inválida");
  }
}



