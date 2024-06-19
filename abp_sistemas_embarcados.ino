#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int botaoCima = 6;
const int botaoBaixo = 7;
const int botaoConfirmar = 8;

int opcaoSelecionada = 0;
unsigned long ultimoDebounce = 0;
const long debounceDelay = 150;
int pesoMaximo = 0;
int alturaMaxima = 0;
int pesoAtual = A0;   // variaveis de teste
int alturaAtual = A1; // variaveis de teste

void setup()
{
  pinMode(botaoCima, INPUT_PULLUP);
  pinMode(botaoBaixo, INPUT_PULLUP);
  pinMode(botaoConfirmar, INPUT_PULLUP);
  pinMode(botaoConfirmar, INPUT_PULLUP);
  pinMode(pesoAtual, INPUT);
  pinMode(alturaAtual, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  atualizarDisplay();
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

  Serial.println("Peso: ");
  Serial.println(analogRead(pesoAtual));
  Serial.println("Altura: ");
  Serial.println(analogRead(alturaAtual));
}

void atualizarDisplay()
{
  if (opcaoSelecionada == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Peso ");
    lcd.print(opcaoSelecionada + 1);
  }
  else if (opcaoSelecionada == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Altura ");
    lcd.print(opcaoSelecionada + 1);
  }
  else if (opcaoSelecionada == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Executar ");
    lcd.print(opcaoSelecionada + 1);
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

int setarPeso()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite o peso:");
  lcd.setCursor(0, 1);
  lcd.print(pesoMaximo);
  lcd.print(" mg");
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
      lcd.print(" kg");
      delay(200);
    }
    if (digitalRead(botaoBaixo) == 1)
    {
      pesoMaximo = max(0, pesoMaximo - 1);
      lcd.setCursor(0, 1);
      lcd.print(pesoMaximo);
      lcd.print(" kg");
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

int setarAltura()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a altura:");
  lcd.setCursor(0, 1);
  lcd.print(alturaMaxima);
  lcd.print(" cm");
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
      lcd.print(" cm");
      delay(200);
    }
    if (digitalRead(botaoBaixo) == 1)
    {
      alturaMaxima = max(0, alturaMaxima - 1);
      lcd.setCursor(0, 1);
      lcd.print(alturaMaxima);
      lcd.print(" cm");
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

int executaRotina()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Executando rotina");
  delay(1000);
  while (true)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aguardando...");
    delay(1000);
    if (analogRead(pesoAtual) > 10)
    {
      if (analogRead(alturaAtual) > alturaMaxima || analogRead(pesoAtual) > pesoMaximo)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("produto descartado");
        delay(2000);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("produto aceito");
        delay(2000);
      }
    }
    Serial.println("Peso: ");
    Serial.println(analogRead(pesoAtual));
    Serial.println("Altura: ");
    Serial.println(analogRead(alturaAtual));
  }
}