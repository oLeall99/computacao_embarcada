#include <LiquidCrystal.h>

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

#define led_red 12
#define led_green 13
#define buzzer A0
#define btnStart 2
#define btnReset 3
#define btn01 A5
#define btn02 A4
#define btn03 A3
#define btn04 A2

// Notas utilizadas pelas músicas
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Váriaveis Globais
volatile bool inMenu = false;
volatile bool inGame = false;
volatile bool menuChanged = false;
volatile int currentMusicIndex = 0;
const int TOTAL_MUSICS = 6;

// Struct das Músicas
struct Music {
  const char* name;
  const int* melody;
  const int* duration;
};

// Música de introdução
int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,NOTE_F4, NOTE_A4, NOTE_B4, NOTE_C5 };
int noteDurations[] = { 200, 200, 200, 200, 200, 200, 200, 400 };


int melody1[] = { NOTE_F4, NOTE_F4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4 };
int duration1[] = { 300, 300, 300, 400, 300, 300, 300, 400, 300, 300, 300, 400, 300, 300, 300, 400, 500, 500, 500, 500 };

int melody2[] = { NOTE_C4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_F4 };
int duration2[] = { 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250 };

int melody3[] = { NOTE_D4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4 };
int duration3[] = { 150, 150, 150, 150, 200, 200, 200, 150, 150, 150, 150, 150, 150, 200, 200, 200, 150, 150, 150, 150 };

int melody4[] = { NOTE_E4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_D4 };
int duration4[] = { 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200 };

int melody5[] = { NOTE_F4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4 };
int duration5[] = { 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300 };

int melody6[] = { NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4 };
int duration6[] = { 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 500, 500, 500, 500 };

const Music MUSICS[] = {
  {"Star Wars", melody1, duration1},
  {"Underworld", melody2, duration2},
  {"Megalovania", melody3, duration3},
  {"Tetris Theme", melody4, duration4},
  {"Axel F", melody5, duration5},
  {" GOT Theme", melody6, duration6},
};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(btnStart, INPUT_PULLUP);
  pinMode(btnReset, INPUT_PULLUP);
  pinMode(btn01, INPUT_PULLUP);
  pinMode(btn02, INPUT_PULLUP);
  pinMode(btn03, INPUT_PULLUP);
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
  else if (inMenu) {
    checkMenuButtons();
  }
  else if (inGame) {
    game();
  }
}

void checkMenuButtons() {
  if (digitalRead(btn01) == LOW) {
    currentMusicIndex = (currentMusicIndex + 1) % TOTAL_MUSICS;
    menuChanged = true;
    delay(300);
  }

  if (digitalRead(btn04) == LOW) {
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
  } else if(!inMenu && inGame) {
    inMenu = true;
    inGame = false;
    menuChanged = true;
  }
  delay(500);
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

void playSequence(int level) {
  printLCD("Memorize!", "");
  delay(1000);

  for (int i = 0; i < level * 4; i++) {
    int note = MUSICS[currentMusicIndex].melody[i];
    const char* button = getButtonForNote(note);
    printLCD("Memorize!", ("Botao: " + String(button)).c_str());
    playNote(note, MUSICS[currentMusicIndex].duration[i]);
    delay(500);
  }
}

bool getUserInput(int level) {
  printLCD("Sua vez!", "Repita a seq.");
  delay(1000);

  for (int i = 0; i < level * 4; i++) {
    printLCD("Sua vez!", "Repita a seq.");
    int expectedNote = MUSICS[currentMusicIndex].melody[i];
    const char* expectedButton = getButtonForNote(expectedNote);
    const char* userButton = "";
    while (true) {
      if (digitalRead(btn01) == LOW){
        Serial.println(digitalRead(btn01));
        Serial.println("btn01");
        playNote(NOTE_C4, 200);
        delay(500);
        userButton = "btn01";
        break;
      } 
      if (digitalRead(btn02) == LOW) {
        Serial.println(digitalRead(btn02));
        Serial.println("btn02");
        playNote(NOTE_D4, 200);
        delay(500);
        userButton = "btn02";
        break;
      }
      if (digitalRead(btn03) == LOW) {
        Serial.println(digitalRead(btn03));
        Serial.println("btn03");
        playNote(NOTE_E4, 200);
        delay(500);
        userButton = "btn03";
        break;
      }
      if (digitalRead(btn04) == LOW) {
        Serial.println(digitalRead(btn04));
        Serial.println("btn04");
        playNote(NOTE_C4, 200);
        delay(500);
        userButton = "btn04";
        break;
      } 
      delay(10);
     }

    if (strcmp(userButton, expectedButton) != 0) {
      digitalWrite(led_red, HIGH);
      printLCD("Voce Errou", "Tente de Novo");
      delay(2000);
      digitalWrite(led_red, LOW);
      return false;
    }
    else {
      digitalWrite(led_green, HIGH);
      printLCD("Voce Acertou", "parabens");
      delay(200);
      digitalWrite(led_green, LOW);
    }
  }
  if(level < 5) {
    printLCD("Parabens!", "Prox Nivel");
    delay(2000);
  }
  
  return true;
}

void game() {
  int level = 1;
  while(level < 5) {
    if(level > 5){
      printLCD("Parabens", "Musica Feita");
      delay(2000);
      inGame = false;
      inMenu = true;
      menuChanged = true;
      updateMenu();
      return;
    }
    else{
     playSequence(level);
      if (getUserInput(level)) {
        level++;
      }       
    }
  }

    
    
  
}

const char* getButtonForNote(int note) {
  switch (note) {
    case NOTE_C4: return "btn01";
    case NOTE_D4: return "btn02";
    case NOTE_E4: return "btn03";
    case NOTE_F4: return "btn04";
    default: return "0";
  }
}


