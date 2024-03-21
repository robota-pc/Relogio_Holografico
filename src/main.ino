#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "SEU_SSID";
const char *password = "SUA_SENHA";

WebServer server(80);

int ledPin = 2; // Pin do LED
int numeros[5] = {123, 456, 789, 101, 112}; // Array com os 5 números

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

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
