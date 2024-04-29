#include <WiFi.h>
#include <WebServer.h>

// Credenciais Wi-Fi (substitua pelo seu SSID e senha)
const char* ssid = "POCO NFC";
const char* password = "SUA_SENHA";

// Porta do servidor web (pode ser alterada se necessário)
const int port = 80;

// Variável para armazenar a mensagem recebida da página web
String mensagemWeb;

// Página web
WebServer server(port);

// Função de setup:
void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Conexão à rede Wi-Fi
  Serial.println("Conectando à rede Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado!");

  // Imprime o endereço IP do ESP32
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP().toString());

  // Configura a página web principal
  server.on("/", []() {
    // Envia o conteúdo HTML da página principal
    server.send(200, "text/html", R"(
      <!DOCTYPE html>
      <html lang="pt-br">
      <head>
        <meta charset="UTF-8">
        <title>Troca de Mensagens</title>
      </head>
      <body>
        <h1>Troca de Mensagens</h1>
        <p>Envie uma mensagem para o ESP32:</p>
        <form action="/send" method="post">
          <input type="text" name="mensagem" id="mensagem">
          <input type="submit" value="Enviar">
        </form>
        <p>Mensagem recebida do ESP32:</p>
        <p id="resposta"></p>
        <script>
          const respostaElement = document.getElementById("resposta");
          const xhr = new XMLHttpRequest();
          xhr.open("GET", "/resposta", true);
          xhr.onload = function() {
            if (xhr.status === 200) {
              respostaElement.textContent = xhr.responseText;
            }
          };
          xhr.send();
        </script>
      </body>
      </html>
    )");
  });

  // Rota para receber mensagens da página web
  server.on("/send", HTTP_POST, []() {
    // Recebe a mensagem enviada pela página web
    mensagemWeb = server.arg("mensagem");
    Serial.println("Mensagem recebida da web: " + mensagemWeb);
    // Envia uma resposta para a página web
    server.send(200, "text/plain", "Mensagem enviada com sucesso!");
  });

  // Rota para enviar mensagens para a página web
  server.on("/resposta", []() {
    // Envia a mensagem armazenada em `mensagemWeb` para a página web
    server.send(200, "text/plain", mensagemWeb.c_str());
  });

  // Inicia o servidor web
  server.begin();
  Serial.println("Servidor web iniciado na porta " + String(port));
}

// Função loop:
void loop() {
  // Trata as requisições do servidor web
  server.handleClient();

  // Envia a mensagem do monitor serial para a página web
  if (Serial.available()) {
    String mensagemSerial = Serial.readString();
    server.sendHeader("Content-Type", "text/plain");
    // Converte a string do ESP32 para um array de caracteres C-style
    server.send(200, mensagemSerial.c_str());
  }
}
