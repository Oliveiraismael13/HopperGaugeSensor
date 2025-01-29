#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// Para o shield da Adafruit, estas são as definições padrão.
#define TFT_DC 4
#define TFT_CS 5

// Use SPI com os pinos CS/DC definidos acima.
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Variáveis para os valores
int pressao = 1234; // Exemplo de valor para pressão
int nivel = 5678;   // Exemplo de valor para nível

void setup() {
  tft.begin();
  tft.setRotation(1); // Ajusta a orientação da tela
}

void loop() {
  tft.fillScreen(ILI9341_BLACK);  // Limpa a tela
  tft.setTextSize(6);             // Define o tamanho do texto

  // Exibe "PSI" em branco
  tft.setTextColor(ILI9341_WHITE); 
  tft.setCursor(7, 60);          // Define a posição inicial (coluna, linha)
  tft.print("PSI:");             // Escreve o texto
  
  // Exibe valor de pressão em verde
  tft.setTextColor(ILI9341_GREEN); 
  tft.print(pressao);             // Escreve o valor de pressão

  // Exibe "GAL" em branco
  tft.setTextColor(ILI9341_WHITE); 
  tft.setCursor(7, 140);         // Define a posição para o próximo texto
  tft.print("GAL:");             // Escreve o texto

  // Exibe valor de nível em verde
  tft.setTextColor(ILI9341_GREEN); 
  tft.print(nivel);               // Escreve o valor de nível

  // Exibe "Alarmes" no canto inferior esquerdo em vermelho e com uma fonte menor
  tft.setTextSize(2);             // Define um tamanho de texto menor
  tft.setTextColor(ILI9341_RED);  // Define a cor do texto como vermelho
  tft.setCursor(10, 220);         // Define a posição no canto inferior esquerdo
  tft.print("Alarmes");           // Escreve "Alarmes"

  while (1); // Pausa a execução aqui para não redesenhar repetidamente
}