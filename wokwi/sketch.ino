#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED I2C
#define SDA_PIN 8
#define SCL_PIN 9

// Pinos do projeto
#define MQ_PIN 4
#define BUZZER_PIN 18
#define BUTTON_PIN 14

// Limite e histerese
int limite = 2000;
int histerese = 200;

bool alarmeAtivo = false;
bool alarmeSilenciado = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erro no OLED");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {

  int valorGas = analogRead(MQ_PIN);

  // ---------- HISTERSE ----------
  if (!alarmeAtivo && valorGas > limite) {
    alarmeAtivo = true;
    alarmeSilenciado = false;
  }

  if (alarmeAtivo && valorGas < (limite - histerese)) {
    alarmeAtivo = false;
    noTone(BUZZER_PIN);
  }

  // ---------- BOTÃO SILENCIAR ----------
  if (digitalRead(BUTTON_PIN) == LOW && alarmeAtivo) {
    alarmeSilenciado = true;
    noTone(BUZZER_PIN);
    delay(300); // debounce simples
  }

  // ---------- BUZZER ----------
  if (alarmeAtivo && !alarmeSilenciado) {
    tone(BUZZER_PIN, 2000);
  }

  // ---------- OLED ----------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Leitura: ");
  display.println(valorGas);

  display.print("Limite: ");
  display.println(limite);

  if (alarmeAtivo) {
    display.println("!!! GAS DETECTADO !!!");
  } else {
    display.println("Ambiente Seguro");
  }

  if (alarmeSilenciado) {
    display.println("Alarme Silenciado");
  }

  display.display();

  delay(200);
}
