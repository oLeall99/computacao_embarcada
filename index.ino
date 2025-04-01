#include <LiquidCrystal.h>

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5 };
int noteDurations[] = { 200, 200, 200, 200, 200, 200, 200, 400 };

volatile bool inMenu = false;
volatile bool inGame = false;
volatile int currentMusicIndex = 0;
volatile bool menuChanged = false;
const int TOTAL_MUSICS = 6;

struct Music {
  const char* name;
  void (*playFunction)();
};

void playMusic1();
void playMusic2();
void playMusic3();
void playMusic4();
void playMusic5();
void playMusic6();

const Music MUSICS[] = {
  {"Musica 1", playMusic1},
  {"Musica 2", playMusic2},
  {"Musica 3", playMusic3},
  {"Musica 4", playMusic4},
  {"Musica 5", playMusic5},
  {"Musica 6", playMusic6}
};

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

#define led_red 12
#define led_green 13
#define buzzer A0
#define btnStart 2
#define btnReset 3
#define btn01 A5
#define btn04 A2

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(btnStart, INPUT_PULLUP);
  pinMode(btnReset, INPUT_PULLUP);
  pinMode(btn01, INPUT_PULLUP);
  pinMode(btn04, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(btnStart), handleButtonStart, RISING);
  attachInterrupt(digitalPinToInterrupt(btnReset), handleButtonReset, RISING);

  introMusic();
}

void loop() {
  if (!inMenu && !inGame) {
    playSoftSound();
    printLCD("Bem-vindo ao", "Jogo da Memoria");
    delay(1000);
    printLCD("Pressione B1", "para jogar!");
    delay(1000);
  }

  if (inMenu) {
    checkMenuButtons();
  }

  if (inGame) {
    game();
  }
}

void checkMenuButtons() {
  if (digitalRead(btn01) == LOW) {
    Serial.println("Botão 01 pressionado! (Próxima música)");
    currentMusicIndex = (currentMusicIndex + 1) % TOTAL_MUSICS;
    menuChanged = true;
    delay(300);
  }

  if (digitalRead(btn04) == LOW) {
    Serial.println("Botão 04 pressionado! (Música anterior)");
    currentMusicIndex = (currentMusicIndex - 1 + TOTAL_MUSICS) % TOTAL_MUSICS;
    menuChanged = true;
    delay(300);
  }

  updateMenu();
}

void handleButtonStart() {
  if (!inMenu && !inGame) {
    inMenu = true;
    menuChanged = true;
  } else if (inMenu && !inGame) {
    inMenu = false;
    inGame = true;
  }
}

void handleButtonReset() {
  if (!inMenu && inGame) {
    inMenu = true;
    inGame = false;
    menuChanged = true;
  } else if (inMenu && !inGame) {
    inMenu = false;
  }
}

void printLCD(const char* topMessage, const char* bottomMessage) {
  lcd.clear();
  int topSpaces = (16 - strlen(topMessage)) / 2;
  lcd.setCursor(topSpaces, 0);
  lcd.print(topMessage);
  int bottomSpaces = (16 - strlen(bottomMessage)) / 2;
  lcd.setCursor(bottomSpaces, 1);
  lcd.print(bottomMessage);
}

void playNote(int note, int duration) {
  tone(buzzer, note, duration);
  delay(duration + 50);
}

void playSoftSound() {
  playNote(NOTE_C4, 200);
  delay(200);
  playNote(NOTE_C4, 200);
}

void introMusic() {
  for (int i = 0; i < 8; i++) {
    playNote(melody[i], noteDurations[i]);
  }
  noTone(buzzer);
  delay(100);
}

void updateMenu() {
  if (menuChanged) {
    int nextIndex = (currentMusicIndex + 1) % TOTAL_MUSICS;
    String currentLine = "-> " + String(MUSICS[currentMusicIndex].name);
    String nextLine = "   " + String(MUSICS[nextIndex].name);

    printLCD(currentLine.c_str(), nextLine.c_str());
    menuChanged = false;
  }
}

void game() {
  printLCD("Game", "para jogar!");
  delay(1000);
}

void playMusic1() { playNote(NOTE_C4, 200); playNote(NOTE_E4, 200); playNote(NOTE_G4, 200); }
void playMusic2() { playNote(NOTE_E4, 100); playNote(NOTE_G4, 100); playNote(NOTE_C4, 100); }
void playMusic3() { playNote(NOTE_G4, 150); playNote(NOTE_C4, 150); playNote(NOTE_E4, 150); }
void playMusic4() { playNote(NOTE_C4, 100); playNote(NOTE_E4, 200); playNote(NOTE_G4, 100); }
void playMusic5() { playNote(NOTE_E4, 150); playNote(NOTE_G4, 150); playNote(NOTE_C4, 150); }
void playMusic6() { playNote(NOTE_G4, 100); playNote(NOTE_C4, 200); playNote(NOTE_E4, 100); }
