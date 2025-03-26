#include <LiquidCrystal.h>

volatile bool inMenu = false;
LiquidCrystal lcd(9, 8, 7, 6, 5, 4); // LCD

#define led_red 12
#define led_green 13
#define buzzer A5
#define btn01 2
#define btn02 10
#define btn03 11
#define btn04 3

// Definição das notas musicais em Hz
#define NOTE_C4  262
#define NOTE_E4  330
#define NOTE_G4  392

// Estrutura para armazenar informações da música
struct Music {
  const char* name;
  void (*playFunction)();
};

// Variáveis globais para controle do menu
volatile int currentMusicIndex = 0;
volatile bool menuChanged = false;
const int TOTAL_MUSICS = 6;

// funcoes
void playMusic1();
void playMusic2();
void playMusic3();
void playMusic4();
void playMusic5();
void playMusic6();

// Musicas
const Music MUSICS[] = {
  {"Musica 1", playMusic1},
  {"Musica 2", playMusic2},
  {"Musica 3", playMusic3},
  {"Musica 4", playMusic4},
  {"Musica 5", playMusic5},
  {"Musica 6", playMusic6}
};


void handleButtonPress() {
  if(inMenu) {
    selectMusic();
  } else {
    inMenu = true;
  }
}

void handleButtonExit() {
  if(inMenu) {
    inMenu = false;
    currentMusicIndex = 0; 
    menuChanged = true;
  }
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(btn01, INPUT);
  pinMode(btn02, INPUT);
  pinMode(btn03, INPUT);
  pinMode(btn04, INPUT);

  attachInterrupt(digitalPinToInterrupt(btn01), handleButtonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(btn04), handleButtonExit, RISING);
}

void loop()
{
  while(!inMenu){
    printLCD("Bem vindo ao", "Jogo!");
    playSoftSound();
    delay(1000);

    printLCD("Pressione B1", "para jogar!");
    playSoftSound();
    delay(1000);
  }

  while(inMenu){
  
    if(digitalRead(btn02) == HIGH) {
      currentMusicIndex = (currentMusicIndex + 1) % TOTAL_MUSICS;
      menuChanged = true;
      delay(200); // Debounce
    }
    
    if(digitalRead(btn03) == HIGH) {
      currentMusicIndex = (currentMusicIndex - 1 + TOTAL_MUSICS) % TOTAL_MUSICS;
      menuChanged = true;
      delay(200); // Debounce
    }
    
    updateMenu();
    delay(100);
  }
}

void updateMenu() {
  if(menuChanged) {
    int nextIndex = (currentMusicIndex + 1) % TOTAL_MUSICS;
    String currentLine = "-> " + String(MUSICS[currentMusicIndex].name);
    String nextLine = "   " + String(MUSICS[nextIndex].name);
    
    printLCD(currentLine.c_str(), nextLine.c_str());
    menuChanged = false;
  }
}

// Função para selecionar e tocar a música
void selectMusic() {
  inMenu = false;
  printLCD("Tocando:", MUSICS[currentMusicIndex].name);
  MUSICS[currentMusicIndex].playFunction();
  delay(10000);
  inMenu = true;
  menuChanged = true;
}

// Implementação das funções de música
void playMusic1() {
  // Melodia simples e alegre
  playNote(NOTE_C4, 200);
  playNote(NOTE_E4, 200);
  playNote(NOTE_G4, 200);
  playNote(NOTE_C4, 200);
  playNote(NOTE_E4, 200);
  playNote(NOTE_G4, 200);
}

void playMusic2() {
  // Melodia com ritmo mais rápido
  playNote(NOTE_E4, 100);
  playNote(NOTE_G4, 100);
  playNote(NOTE_C4, 100);
  playNote(NOTE_E4, 100);
  playNote(NOTE_G4, 100);
  playNote(NOTE_C4, 100);
}

void playMusic3() {
  // Melodia com notas mais altas
  playNote(NOTE_G4, 150);
  playNote(NOTE_C4, 150);
  playNote(NOTE_E4, 150);
  playNote(NOTE_G4, 150);
  playNote(NOTE_C4, 150);
  playNote(NOTE_E4, 150);
}

void playMusic4() {
  // Melodia com ritmo alternado
  playNote(NOTE_C4, 100);
  playNote(NOTE_E4, 200);
  playNote(NOTE_G4, 100);
  playNote(NOTE_C4, 200);
  playNote(NOTE_E4, 100);
  playNote(NOTE_G4, 200);
}

void playMusic5() {
  // Melodia com sequência crescente
  playNote(NOTE_E4, 150);
  playNote(NOTE_G4, 150);
  playNote(NOTE_C4, 150);
  playNote(NOTE_E4, 150);
  playNote(NOTE_G4, 150);
  playNote(NOTE_C4, 150);
}

void playMusic6() {
  // Melodia com ritmo mais complexo
  playNote(NOTE_G4, 100);
  playNote(NOTE_C4, 200);
  playNote(NOTE_E4, 100);
  playNote(NOTE_G4, 200);
  playNote(NOTE_C4, 100);
  playNote(NOTE_E4, 200);
}

// Função para imprimir mensagens no LCD
void printLCD(const char* topMessage, const char* bottomMessage) {
  lcd.clear();
  
  // Centraliza a mensagem superior
  int topSpaces = (16 - strlen(topMessage)) / 2;
  lcd.setCursor(topSpaces, 0);
  lcd.print(topMessage);
  
  // Centraliza a mensagem inferior
  int bottomSpaces = (16 - strlen(bottomMessage)) / 2;
  lcd.setCursor(bottomSpaces, 1);
  lcd.print(bottomMessage);
}

// Função para tocar uma nota com um tempo de duração
void playNote(int note, int duration) {
  tone(buzzer, note, duration);
  delay(duration + 25);
}

// Música de Introdução
void playSoftSound() {
  playNote(NOTE_C4, 100);
  playNote(NOTE_E4, 100);
  playNote(NOTE_G4, 100);
  delay(200);
}

// Música de botão pressionado
void playButtonSound() {
  playNote(NOTE_C4, 100);  // Toca uma nota C4 por 50ms
  delay(25);  // Pequena pausa para separar o som
}