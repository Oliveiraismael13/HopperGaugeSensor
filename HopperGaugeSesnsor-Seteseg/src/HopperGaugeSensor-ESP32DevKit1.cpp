#include <Arduino.h>

// Segmentos compartilhados para os dois displays
int segmentos[] = {2, 4, 16, 17, 18, 19, 21}; // Pinos PWM para segmentos a, b, c, d, e, f, g
int pontoDecimal = 22;                        // Ponto decimal controlado por PWM

// Dígitos para os displays
int digitosDisplay1[] = {32, 33, 25, 26};     // Dígitos do display 1 (Nível do tanque)
int digitosDisplay2[] = {27, 14, 12, 13};     // Dígitos do display 2 (Pressão)

// Botão tátil
const int pinoBotao = 23;                     // Pino de entrada do botão DIM (brilho)

// Sensores (Potenciômetros)
const int pinoPotNivel = 34;                  // ADC1_6 (Leitura do nível do tanque)
const int pinoPotPressao = 35;                // ADC1_7 (Leitura da pressão)

// Estados de brilho
enum Brilho { BRILHO_MIN = 64, BRILHO_MED = 128, BRILHO_MAX = 255 };
int estadoBrilho = BRILHO_MIN;

// Mapeamento dos números para os segmentos
const int Numero[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

// Controle do botão
bool botaoPressionado = false;

void configurarPinos();
void inicializarDisplays(); // Function declaration
void verificarBotao();
void exibirNoDisplay(int valor, int digitos[], bool exibirPonto);
float calcularPressao(int leituraADC);
int formatarValor(int valor);

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando ESP32 com pinos otimizados...");

  // Configuração dos pinos
  configurarPinos();

  // Inicializa os displays desligados
  inicializarDisplays();
}

void loop() {
  verificarBotao();

  // Leitura do sensor de nível simulada pelo potenciômetro
  int nivel = map(analogRead(pinoPotNivel), 0, 4095, 0, 9999);

  // Leitura do sensor de pressão simulada pelo potenciômetro
  float pressao = calcularPressao(analogRead(pinoPotPressao));

  // Atualiza os displays
  for (int i = 0; i < 50; i++) {
    exibirNoDisplay(formatarValor(nivel), digitosDisplay1, true);        // Exibe nível com ponto decimal na terceira casa
    exibirNoDisplay(formatarValor(pressao * 10), digitosDisplay2, true); // Exibe pressão com ponto decimal na terceira casa
  }
}

void configurarPinos() {
  for (int i = 0; i < 7; i++) pinMode(segmentos[i], OUTPUT);
  pinMode(pontoDecimal, OUTPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(digitosDisplay1[i], OUTPUT);
    pinMode(digitosDisplay2[i], OUTPUT);
  }

  pinMode(pinoBotao, INPUT_PULLUP);
}

void inicializarDisplays() { // Function definition
  for (int i = 0; i < 7; i++) digitalWrite(segmentos[i], LOW);
  digitalWrite(pontoDecimal, LOW);
  for (int i = 0; i < 4; i++) {
    digitalWrite(digitosDisplay1[i], HIGH);
    digitalWrite(digitosDisplay2[i], HIGH);
  }
}

void verificarBotao() {
  static unsigned long ultimaLeitura = 0; // Armazena o tempo da última leitura válida
  const int debounceDelay = 50;           // Tempo de debounce em milissegundos

  if (digitalRead(pinoBotao) == LOW && !botaoPressionado) {
    unsigned long agora = millis();
    if (agora - ultimaLeitura > debounceDelay) { // Verifica o tempo de debounce
      botaoPressionado = true;
      ultimaLeitura = agora;
      
      // Alterna o estado do brilho
      estadoBrilho = (estadoBrilho == BRILHO_MIN) ? BRILHO_MED : 
                     (estadoBrilho == BRILHO_MED) ? BRILHO_MAX : BRILHO_MIN;
      Serial.println("Brilho ajustado para: " + String(estadoBrilho));
    }
  }
  if (digitalRead(pinoBotao) == HIGH) botaoPressionado = false;
}

void exibirNoDisplay(int valor, int digitos[], bool exibirPonto) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) digitalWrite(digitos[j], HIGH);

    int digito = (valor / (int)pow(10, 3 - i)) % 10;

    for (int j = 0; j < 7; j++) 
      analogWrite(segmentos[j], Numero[digito][j] * estadoBrilho);

    analogWrite(pontoDecimal, (exibirPonto && i == 2) ? estadoBrilho : 0);

    digitalWrite(digitos[i], LOW);
    delay(2);

    digitalWrite(digitos[i], HIGH);
  }
}

float calcularPressao(int leituraADC) {
  if (leituraADC >= 819) {
    return map(leituraADC, 819, 4095, 0, 1500) / 10.0;
  } else {
    return 0;
  }
}

int formatarValor(int valor) {
  return valor;
}