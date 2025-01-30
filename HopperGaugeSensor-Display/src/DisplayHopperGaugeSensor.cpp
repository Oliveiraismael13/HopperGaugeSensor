#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// Definição dos pinos para o display ILI9341
#define TFT_DC 4
#define TFT_CS 5

// Inicializa o display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Sensores (Potenciômetros)
const int pinoPotNivel = 34;  // ADC1_6 (Leitura do nível do tanque)
const int pinoPotPressao = 35; // ADC1_7 (Leitura da pressão)

// Variáveis para armazenar valores anteriores
float nivelAnterior = -1.0;
float pressaoAnterior = -1.0;

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);  // Ajusta a orientação da tela
  tft.fillScreen(ILI9341_BLACK);

  // Exibe textos fixos
  tft.setTextSize(4);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(7, 80);
  tft.print("PSI: ");

  tft.setCursor(7, 160);
  tft.print("GAL: ");

  // Remova a exibição fixa de "Alarmes"
  // tft.setTextSize(2);
  // tft.setTextColor(ILI9341_RED);
  // tft.setCursor(10, 220);
  // tft.print("Alarmes");
}

void piscarAlarme() {
  for (int i = 0; i < 3; i++) {
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_RED);
    tft.setCursor(10, 220);
    tft.print("PRESS");  // Liga o alarme
    delay(500);
    tft.fillRect(10, 220, 200, 30, ILI9341_BLACK);  // Desliga o alarme
    delay(500);
  }
}

void loop() {
  // Leitura dos sensores
  float nivel = map(analogRead(pinoPotNivel), 0, 4095, 0, 9999) / 10.0;  // Mapeia para 0 a 999.9
  float pressao = map(analogRead(pinoPotPressao), 819, 4095, 0, 1500) / 10.0;

  // Atualiza a leitura de pressão, se necessário
  if (pressao != pressaoAnterior) {
    tft.setTextSize(6);
    tft.setTextColor(ILI9341_BLACK);  // Apaga o valor anterior
    tft.setCursor(100, 60);
    tft.print(pressaoAnterior, 1);  // Exibe o valor anterior em preto

    tft.setTextColor(ILI9341_GREEN);
    tft.setCursor(100, 60);
    tft.print(pressao, 1);  // Exibe o novo valor

    pressaoAnterior = pressao;
  }

  // Verifica se a pressão atingiu o valor máximo e pisca o alarme
  if (pressao >= 150.0) {
    piscarAlarme();
  }

  // Atualiza a leitura do nível, se necessário
  if (nivel != nivelAnterior) {
    tft.setTextSize(6);
    tft.setTextColor(ILI9341_BLACK);  // Apaga o valor anterior
    tft.setCursor(100, 140);
    tft.print(nivelAnterior, 1);  // Exibe o valor anterior em preto

    tft.setTextColor(ILI9341_GREEN);
    tft.setCursor(100, 140);
    tft.print(nivel, 1);  // Exibe o novo valor

    nivelAnterior = nivel;
  }

  delay(200);  // Atraso para uma atualização suave
}
