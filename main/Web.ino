#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "relogio";
const char* password = "holografico";

int variable = 4;

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body>";
    html += "<h1>Variable: " + String(variable) + "</h1>";
    html += "<button onclick=\"location.href='/increment'\">Increment</button>";
    html += "<button onclick=\"location.href='/decrement'\">Decrement</button>";
    html += "<table id='data-table'><tr><th>Variable 1</th><th>Variable 2</th></tr></table>";
    html += "<canvas id='chart'></canvas>";
    html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
    html += "<script>";
    html += "var ctx = document.getElementById('chart').getContext('2d');";
    html += "var chart = new Chart(ctx, {type: 'line', data: {labels: [], datasets: [{label: 'Data', data: [], fill: false, borderColor: 'rgb(75, 192, 192)', tension: 0.1}]}});";
    html += "function updateData() {";
    html += "fetch('/data').then(response => response.json()).then(data => {";
    html += "document.getElementById('data-table').innerHTML += '<tr><td>' + data.variable1 + '</td><td>' + data.variable2 + '</td></tr>';";
    html += "chart.data.labels.push('');";
    html += "chart.data.datasets[0].data.push(data.variable1);";
    html += "chart.update();";
    html += "});";
    html += "}";
    html += "setInterval(updateData, 1000);";
    html += "</script>";
    html += "</body></html>";
    request->send(200, "text/html", html);
    });

    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"variable1\":" + String(variable1) + ",";
    json += "\"variable2\":" + String(variable2);
    json += "}";
    request->send(200, "application/json", json);
    });

    server.on("/increment", HTTP_GET, [](AsyncWebServerRequest *request){
        variable++;
        request->redirect("/");
    });

    server.on("/decrement", HTTP_GET, [](AsyncWebServerRequest *request){
        variable--;
        request->redirect("/");
    });

    server.begin();
}

void loop() {
    // Your code here
}