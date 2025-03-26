#include <LiquidCrystal.h>

volatile bool ligado = true;
LiquidCrystal lcd(9, 8, 7, 6, 5, 4); // LCD

#define led_red 1
#define led_green 2
#define buzzer 3
#define btn01 10
#define btn02 11
#define btn03 12
#define btn04 13


void setup()
{
  lcd.begin(16, 2);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(buzzer, OUTPUT);

  setup_btn(btn01);
  setup_btn(btn02);
  setup_btn(btn03);
  setup_btn(btn04);
}

void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Bem vindo ao Jogo!");
  lcd.print("	");

  
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
  
  delay(100);
}

void blink(){
  ligado = !ligado;
  digitalWrite(13,ligado);
}

void setup_btn(int btn)
{
  pinMode(btn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn), blink, FALLING);
}