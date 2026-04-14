#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pinos
#define MQ3_PIN 34
#define BUZZER 25
#define BUTTON 26

int threshold = 900;

// WIFI
const char* ssid = "Rafael-2.4";
const char* password = "rafael2906";

// MQTT
const char* mqtt_server = "56.124.107.241";

WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis
bool silenciado = false;
unsigned long tempoSeguro = 0;

// ================= CALLBACK MQTT =================
void callback(char* topic, byte* payload, unsigned int length) {

  String mensagem;

  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }

  Serial.print("Mensagem recebida: ");
  Serial.println(mensagem);

  if (mensagem == "silenciar") {
    silenciado = true;
    digitalWrite(BUZZER, HIGH);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Wire.begin(21, 22);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED falhou");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("INICIANDO");
  display.display();
  delay(2000);

  // WIFI
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");

  // MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ================= MQTT RECONNECT =================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
      // Inscreve no tópico de controle ao conectar
      client.subscribe("sensor/controle");
    } else {
      Serial.print("Erro: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ================= LOOP =================
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  int valor = analogRead(MQ3_PIN);

  // ENVIO MQTT
  char msg[150];

  if (valor > threshold) {
    if (silenciado) {
      sprintf(msg, "{\"ppm\": %d, \"status\": \"SILENCIADO\", \"mensagem\": \"Ambiente perigoso\"}", valor);
    } else {
      sprintf(msg, "{\"ppm\": %d, \"status\": \"ALARME ATIVO\", \"mensagem\": \"Ambiente perigoso\"}", valor);
    }
  } else {
    sprintf(msg, "{\"ppm\": %d, \"status\": \"SEGURO\", \"mensagem\": \"Ambiente seguro\"}", valor);
  }

  client.publish("sensor/esp32", msg);

  Serial.print("Enviado: ");
  Serial.println(msg);

  bool botao = digitalRead(BUTTON) == LOW;
  if (botao) {
    silenciado = true;
    digitalWrite(BUZZER, LOW);
  }

  // ================= PERIGO =================
  if (valor > threshold) {

    tempoSeguro = 0;

    if (!silenciado) {
      digitalWrite(BUZZER, HIGH);
    } else {
      digitalWrite(BUZZER, LOW);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    if (silenciado) {
      display.println("ALARME SILENCIADO");
      display.println("Ambiente perigoso");
    } else {
      display.println("PERIGO!");
      display.println("Alto nivel de gas");
    }

    display.display();
  }

  // ================= SEGURO =================
  else {

    digitalWrite(BUZZER, LOW);
    silenciado = false;

    if (tempoSeguro == 0) {
      tempoSeguro = millis();
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    if (millis() - tempoSeguro > 2000) {
      display.println("Ambiente seguro");
    } else {
      display.println("Estabilizando...");
    }

    display.display();
  }

  delay(2000);
}
