#include <LiquidCrystal.h>
#include "HX711.h"
#include <NewPing.h>
#include <Servo.h>

// pinos do display lcd 16x2
// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(43, 41, 37, 35, 33, 31);
int pwm = 13;


Servo myservo;  // Cria um objeto baseado na biblioteca Servo

// Define os pinos para o sensor ultrassônico e a distância máxima para medir (em centímetros)
#define TRIGGER_PIN  24
#define ECHO_PIN     26
#define MAX_DISTANCE 200

// pinos dos botoes
const int botaoCima = 50;
const int botaoBaixo = 48;
const int botaoConfirmar = 46;

// cria uma instancia da biblioteca HX711 na variavel escala para o sensor da balança
HX711 escala;

// Define os pinos para o sensor da balança
#define SCK A0
#define DT A1

// Cria uma instância do objeto NewPing para o ultrassonico
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// variaveis globais e de configuração
int opcaoSelecionada = 0;
unsigned long ultimoDebounce = 0;
const long debounceDelay = 150;
int pesoMaximo = 0;
int alturaMaxima = 0;

void setup()
{
  //seta os pinos dos leds
  for(int i = 2; i <= 6; i++)
  {
    pinMode(i, OUTPUT);
  }

  // desliga os leds
  for(int i = 2; i <= 6; i++)
  {
    digitalWrite(i, HIGH);
  }
  pinMode(botaoCima, INPUT_PULLUP);
  pinMode(botaoBaixo, INPUT_PULLUP);
  pinMode(botaoConfirmar, INPUT_PULLUP);
  pinMode(botaoConfirmar, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pwm, INPUT);
  analogWrite(pwm, 130);
  atualizarDisplay();

  // inicia o servo na porta 9
  myservo.attach(9);

  // inicia a instancia 'escala'
  escala.begin (DT, SCK);
  // começa a leitura e calculos para iniciar a pesagem
  Serial.print("Leitura do Valor ADC:  ");
  Serial.println(escala.read());   // Aguarda até o dispositivo estar pronto
  Serial.println("Nao coloque nada na balanca!");
  Serial.println("Iniciando...");
  escala.set_scale(765991.129);     // Substituir o valor encontrado para escala
  escala.tare(20);
  Serial.println("Balança pronta!");

  // seta a posição inicial do servo
  myservo.write(45);
}

void loop()
{
  if (digitalRead(botaoCima) == 1)
  {
    if ((millis() - ultimoDebounce) > debounceDelay)
    {
      opcaoSelecionada = max(0, opcaoSelecionada - 1);
      atualizarDisplay();
      ultimoDebounce = millis();
    }
  }
  if (digitalRead(botaoBaixo) == 1)
  {
    if ((millis() - ultimoDebounce) > debounceDelay)
    {
      opcaoSelecionada = min(2, opcaoSelecionada + 1);
      atualizarDisplay();
      ultimoDebounce = millis();
    }
  }
  if (digitalRead(botaoConfirmar) == 1)
  {
    if ((millis() - ultimoDebounce) > debounceDelay)
    {
      executarOpcao(opcaoSelecionada);
      ultimoDebounce = millis();
    }
  }
}

void atualizarDisplay()
{
  if (opcaoSelecionada == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Setar peso");
  }
  else if (opcaoSelecionada == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Setar altura");
  }
  else if (opcaoSelecionada == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rotina");
  }
}

void executarOpcao(int opcao)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Executando ");
  lcd.print(opcao + 1);

  if (opcao == 0)
  {
    setarPeso();
  }
  else if (opcao == 1)
  {
    setarAltura();
  }
  else if (opcao == 2)
  {
    executaRotina();
  }
}

void setarPeso()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite o peso:");
  lcd.setCursor(0, 1);
  lcd.print(pesoMaximo);
  lcd.print("g");
  delay(1000);
  lcd.blink();
  lcd.setCursor(0, 1);
  while (true)
  {
    if (digitalRead(botaoCima) == 1)
    {
      pesoMaximo = min(100, pesoMaximo + 1);
      lcd.setCursor(0, 1);
      lcd.print(pesoMaximo);
      lcd.print("g");
      delay(200);
    }
    if (digitalRead(botaoBaixo) == 1)
    {
      pesoMaximo = max(0, pesoMaximo - 1);
      lcd.setCursor(0, 1);
      lcd.print(pesoMaximo);
      lcd.print("g");
      delay(200);
    }
    if (digitalRead(botaoConfirmar) == 1)
    {
      lcd.noBlink();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ok");
      delay(1000);
      break;
    }
  }
  atualizarDisplay();
}

void setarAltura()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a altura:");
  lcd.setCursor(0, 1);
  lcd.print(alturaMaxima);
  lcd.print("cm");
  delay(1000);
  lcd.blink();
  lcd.setCursor(0, 1);
  while (true)
  {
    if (digitalRead(botaoCima) == 1)
    {
      alturaMaxima = min(250, alturaMaxima + 1);
      lcd.setCursor(0, 1);
      lcd.print(alturaMaxima);
      lcd.print("cm");
      delay(200);
    }
    if (digitalRead(botaoBaixo) == 1)
    {
      alturaMaxima = max(0, alturaMaxima - 1);
      lcd.setCursor(0, 1);
      lcd.print(alturaMaxima);
      lcd.print("cm");
      delay(200);
    }
    if (digitalRead(botaoConfirmar) == 1)
    {
      lcd.noBlink();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ok");
      delay(1000);
      break;
    }
  }
  atualizarDisplay();
}

void executaRotina()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Executando rotina");
  delay(1000);
  while (true)
  {
    desligaLeds();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aguardando...");
    delay(500);
    if (getPeso() > 1)
    {
      if (getAltura() > alturaMaxima || getPeso() > pesoMaximo)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("produto descartado");
        delay(500);
        myservo.write(170);
        delay(2000);
        myservo.write(45);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("produto aceito");
        ligaLeds();
        delay(2000);
      }
    }
    if(digitalRead(botaoConfirmar) == 1 && digitalRead(botaoCima) == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fim da rotina");
      delay(2000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Peso: ");
      break;
    }
    Serial.println("Peso: ");
    Serial.println(getPeso());
    Serial.println("Altura: ");
    Serial.println(getAltura());
  }
}

int getPeso()
{
  return escala.get_units(20) * 1000;
}

int getAltura()
{
  unsigned int distance = sonar.ping_cm();
  int distancia = 10 - distance;
  return distancia;
}

void desligaLeds()
{
  for(int i = 2; i <= 6; i++)
  {
    digitalWrite(i, HIGH);
  }
}

void ligaLeds()
{
  for(int i = 2; i <= 6; i++)
  {
    digitalWrite(i, LOW);
  }
}