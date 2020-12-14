#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//Buttons and debouncing
byte but[3]; byte debouncer = 13; unsigned long lastPressTime; unsigned long pressDelay = 300;
byte b[16]; //For sending text
byte brightness = 2; //Default to MED
byte buzzer = 10; boolean sound = true;
//Game organization
byte progress = 0; byte lastProgress; byte phaser; byte lastPhaser;
byte boopNum = 49; byte boop1; byte boop2; byte boop3; byte boop4; byte boop5; byte boop6; byte boop7; byte boop8; //Collect all the boops to win
boolean youWin = false;
//MiniGames
byte sixteen = 100; byte lastSixteen;
byte num; byte numOne = 48; byte numTwo = 48; byte shift = 6; byte line; byte level; boolean toggler = false; boolean buttonPow; byte score; //"line" is basically a boolean
unsigned long timer = 0; unsigned long startTimer;
byte nums[16] = {11, 7, 14, 13, 10, 5, 6, 15, 1, 13, 4, 2, 12, 3, 8, 9};
byte P1[] = {
  0x00,
  0x00,
  0x00,
  0x01,
  0x02,
  0x04,
  0x05,
  0x05
};
byte P2[] = {
  0x00,
  0x00,
  0x00,
  0x1F,
  0x00,
  0x1C,
  0x02,
  0x09
};
byte P3[] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x18,
  0x04,
  0x04,
  0x02
};
byte Q1[] = {
  0x05,
  0x04,
  0x02,
  0x01,
  0x00,
  0x00,
  0x00,
  0x00
};
byte Q2[] = {
  0x05,
  0x19,
  0x02,
  0x1C,
  0x00,
  0x00,
  0x00,
  0x00
};
byte Q3[] = {
  0x02,
  0x1E,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};
byte cap[] = {
  0x00,
  0x15,
  0x0E,
  0x1F,
  0x0E,
  0x15,
  0x00,
  0x00
};
byte wineBot[] = {
  0x0E,
  0x0E,
  0x0A,
  0x0A,
  0x0E,
  0x0E,
  0x04,
  0x04
};

class lineBuilder {
    char newLine[16];
    char* output; //Not sure why this fixes issue with extra space at end, but it does
  public:
    lineBuilder() {
      strcpy(newLine, "");
    }
    char* y (byte parcel[]) {
      for (byte x = 0; x < 16; x++) {
        newLine[x] = (char)parcel[x];
      }
      return newLine;
    }
};
lineBuilder x;
void DC() {
  delay(2500);
  lcd.clear();
}

//SOUNDS
void getGood() {
  tone(buzzer, 415, 90);
  delay(100);
  tone(buzzer, 466, 90);
  delay(100);
  tone(buzzer, 349, 90);
  delay(200);
  tone(buzzer, 466, 180);
}
void getBad() {
  tone(buzzer, 415, 90);
  delay(100);
  tone(buzzer, 350, 90);
  delay(200);
  tone(buzzer, 349, 180);
}
void spooky() {
  tone(buzzer, 277, 90);
  delay(150);
  tone(buzzer, 329, 45);
}
void getBoop() {
  tone(buzzer, 1318, 125);
  delay(250);
  tone(buzzer, 1174, 100);
  delay(100);
  tone(buzzer, 1108, 100);
  delay(100);
  tone(buzzer, 1174, 100);
  delay(100);
  tone(buzzer, 1318, 100);
  delay(100);
  tone(buzzer, 1174, 125);
  delay(300);
  tone(buzzer, 1760, 500);
}
void getLocation() {
  tone(buzzer, 391, 120);
  delay(400);
  tone(buzzer, 391, 120);
  delay(133);
  tone(buzzer, 523, 180);
  delay(400);
  tone(buzzer, 523, 180);
  delay(400);
  tone(buzzer, 523, 180);
  delay(267);
  tone(buzzer, 391, 120);
  delay(434);
  tone(buzzer, 329, 240);
  delay(266);
  tone(buzzer, 349, 120);
  delay(133);
  tone(buzzer, 369, 240);
  delay(266);
  tone(buzzer, 391, 180);
  delay(400);
  tone(buzzer, 329, 120);
  delay(434);
  tone(buzzer, 261, 180);
  delay(400);
  tone(buzzer, 261, 180);
  delay(400);
  tone(buzzer, 261, 270);
}
void funkyTown() {
  tone(buzzer, 261, 368);
  delay(408);
  tone(buzzer, 261, 91);
  delay(200);
  tone(buzzer, 261, 91);
  delay(200);
  tone(buzzer, 261, 183);
  delay(204);
  tone(buzzer, 329, 368);
  delay(408);
  tone(buzzer, 329, 91);
  delay(200);
  tone(buzzer, 329, 91);
  delay(200);
  tone(buzzer, 391, 91);
  delay(200);
  tone(buzzer, 391, 91);
  delay(200);
  tone(buzzer, 391, 183);
  delay(204);
  tone(buzzer, 659, 275);
  delay(400);
  tone(buzzer, 587, 91);
  delay(200);
  tone(buzzer, 523, 736);
}
void beatGame() {
  tone(buzzer, 233, 182);
  delay(200);
  tone(buzzer, 261, 91);
  delay(100);
  tone(buzzer, 277, 91);
  delay(100);
  tone(buzzer, 349, 34);
  delay(200);
  tone(buzzer, 293, 34);
  delay(100);
  tone(buzzer, 349, 37);
  delay(100);
  tone(buzzer, 293, 34);
  delay(200);
  tone(buzzer, 293, 34);
  delay(100);
  tone(buzzer, 293, 34);
  delay(100);
  tone(buzzer, 293, 34);
  delay(200);
  tone(buzzer, 233, 48);
  delay(200);
  tone(buzzer, 233, 273);
  delay(400);
  tone(buzzer, 293, 34);
  delay(200);
  tone(buzzer, 293, 34);
  delay(100);
  tone(buzzer, 293, 34);
  delay(100);
  tone(buzzer, 293, 34);
  delay(200);
  tone(buzzer, 293, 34);
  delay(100);
  tone(buzzer, 293, 34);
  delay(100);
  tone(buzzer, 293, 34);
  delay(200);
  tone(buzzer, 261, 48);
  delay(200);
  tone(buzzer, 261, 273);
  delay(400);
  tone(buzzer, 261, 34);
  delay(200);
  tone(buzzer, 261, 34);
  delay(100);
  tone(buzzer, 261, 34);
  delay(100);
  tone(buzzer, 261, 34);
  delay(200);
  tone(buzzer, 261, 34);
  delay(100);
  tone(buzzer, 261, 34);
  delay(100);
  tone(buzzer, 261, 34);
  delay(200);
  tone(buzzer, 261, 90);
  delay(200);
  tone(buzzer, 261, 182);
  delay(300);
  tone(buzzer, 261, 90);
  delay(100);
  tone(buzzer, 349, 34);
  delay(200);
  tone(buzzer, 349, 34);
  delay(100);
  tone(buzzer, 349, 34);
  delay(100);
  tone(buzzer, 349, 34);
  delay(200);
  tone(buzzer, 311, 34);
  delay(100);
  tone(buzzer, 311, 34);
  delay(100);
  tone(buzzer, 311, 34);
  delay(200);
  tone(buzzer, 349, 48);
  delay(200);
  tone(buzzer, 349, 273);
}

void setup() {
  randomSeed(analogRead(0));
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(debouncer, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(6, OUTPUT); //Brightness control
  for (byte x = 7; x <= 9; x++) {
    pinMode(x, INPUT);
  }
  //Welcome
  lcd.setCursor(3, 0); b[0] = 87; b[1] = 69; b[2] = 76; b[3] = 67; b[4] = 79; b[5] = 77; b[6] = 69; b[7] = 32; b[8] = 84; b[9] = 79; b[10] = 0; lcd.print(x.y(b));
  lcd.setCursor(3, 1); b[0] = 66; b[1] = 79; b[2] = 79; b[3] = 80; b[4] = 32; b[5] = 81; b[6] = 85; b[7] = 69; b[8] = 83; b[9] = 84; b[10] = 0; lcd.print(x.y(b));
  analogWrite(6, 80);
  //Fade Up
  for (byte x = 0; x <= 80; x++) {
    analogWrite(6, x);
    delay(50);
  }
  analogWrite(6, 80);
  //Make customs chars
  lcd.createChar(0, P1); lcd.createChar(1, P2); lcd.createChar(2, P3);
  lcd.createChar(3, Q1); lcd.createChar(4, Q2); lcd.createChar(5, Q3);
  lcd.createChar(6, cap);
  lcd.createChar(7, wineBot);
}

void loop() {
  //Debouncing
  if (lastPressTime == 0 || (millis() - lastPressTime) > pressDelay) {
    digitalWrite(debouncer, HIGH);
    buttonPow = true;
  }
  byte readBut[3];
  for (byte i = 0; i < 3; i++) {
    readBut[i] = digitalRead(i + 7);
  }
  for (byte j = 0; j < 3; j++) {
    if (but[j] != readBut[j]) {
      but[j] = readBut[j];
      lastPressTime = millis();
      if (progress != 253) { //Let piano play
        digitalWrite(debouncer, LOW);
        buttonPow = false;
      }
    }
  }
  //Gameplay
  if (progress != lastProgress) {
    if (lastProgress < 252 && lastProgress < progress || youWin == true) { //For exiting subquests and upper level progressors
      phaser = 0;
    }
    lastProgress = progress;
    lcd.clear();
  }
  if (phaser != lastPhaser) {
    lastPhaser = phaser;
    lcd.clear();
  }
  progressor();
}

void progressor() {
  switch (progress) {
    default:  mainMenu();  break;
    case 255: beaten();    break;
    case 254: piano();     break;
    case 253: playPiano(); break;
    case 252: resetGame(); break;
    case 0:   mainMenu();  break;
    case 1:   startGame(); break;
    case 10:  ca();        break;
    case 11:  hike();      break;
    case 12:  peak();      break;
    case 13:  geocache();  break;
    case 14:  beach();     break;
    case 15:  shells();    break;
    case 16:  sun();       break;
    case 17:  water();     break;
    case 18:  coffee();    break;
    case 50:  fl();        break;
    case 51:  party();     break;
    case 52:  study();     break;
    case 53:  memorize();  break;
    case 90:  mt();        break;
    case 91:  playAgain(); break;
  }
}
void phaseScroller(byte startCycle, byte endCycle) {
  if (phaser == startCycle && but[0] == HIGH) {
    phaser = endCycle;
  }
  else if (but[0] == HIGH) {
    phaser--;
  };
  if (phaser == endCycle && but[2] == HIGH) {
    phaser = startCycle;
  }
  else if (but[2] == HIGH) {
    phaser++;
  };
}
void mainMenu() {
  youWin = false;
  lcd.setCursor(0, 0); b[0] = 67; b[1] = 104; b[2] = 111; b[3] = 111; b[4] = 115; b[5] = 101; b[6] = 32; b[7] = 97; b[8] = 110; b[9] = 32; b[10] = 79; b[11] = 112; b[12] = 116; b[13] = 105; b[14] = 111; b[15] = 110; b[16] = 0; lcd.print(x.y(b));
  switch (phaser) {
    default:
      phaser = 0;
      break;
    case 0:
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 80; b[3] = 108; b[4] = 97; b[5] = 121; b[6] = 32; b[7] = 100; b[8] = 97; b[9] = 32; b[10] = 71; b[11] = 97; b[12] = 109; b[13] = 101; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(0, 4);
      if (but[1] == HIGH) {
        if (checkBeat(0) == 1) {
          progress = 252;
        }
        else {
          lcd.clear();
          lcd.setCursor(0, 0); b[0] = 87; b[1] = 104; b[2] = 101; b[3] = 114; b[4] = 101; b[5] = 32; b[6] = 119; b[7] = 105; b[8] = 108; b[9] = 108; b[10] = 32; b[11] = 121; b[12] = 111; b[13] = 117; b[14] = 114; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 113; b[1] = 117; b[2] = 101; b[3] = 115; b[4] = 116; b[5] = 32; b[6] = 98; b[7] = 101; b[8] = 103; b[9] = 105; b[10] = 110; b[11] = 63; b[12] = 0; lcd.print(x.y(b));
          funkyTown();
          delay(2000);
          progress++;
        }
      }
      break;
    case 1:
      switch (brightness) {
        case 0:
          lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 76; b[3] = 105; b[4] = 103; b[5] = 104; b[6] = 116; b[7] = 32; b[8] = 32; b[9] = 91; b[10] = 79; b[11] = 70; b[12] = 70; b[13] = 93; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
          analogWrite(6, LOW);
          break;
        case 1:
          lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 76; b[3] = 105; b[4] = 103; b[5] = 104; b[6] = 116; b[7] = 32; b[8] = 32; b[9] = 91; b[10] = 76; b[11] = 79; b[12] = 87; b[13] = 93; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
          analogWrite(6, 20);
          break;
        case 2:
          lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 76; b[3] = 105; b[4] = 103; b[5] = 104; b[6] = 116; b[7] = 32; b[8] = 32; b[9] = 91; b[10] = 77; b[11] = 69; b[12] = 68; b[13] = 93; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
          analogWrite(6, 80);
          break;
        case 3:
          lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 76; b[3] = 105; b[4] = 103; b[5] = 104; b[6] = 116; b[7] = 32; b[8] = 91; b[9] = 72; b[10] = 73; b[11] = 71; b[12] = 72; b[13] = 93; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
          analogWrite(6, 255);
          break;
      }
      if (but[1] == HIGH) {
        if (brightness == 3) {
          brightness = 0;
        }
        else (brightness++);
      }
      phaseScroller(0, 4);
      break;
    case 2:
      if (sound == false) {
        lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 83; b[3] = 111; b[4] = 117; b[5] = 110; b[6] = 100; b[7] = 32; b[8] = 32; b[9] = 91; b[10] = 79; b[11] = 70; b[12] = 70; b[13] = 93; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
        buzzer = 100;
      }
      if (sound == true) {
        lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 83; b[3] = 111; b[4] = 117; b[5] = 110; b[6] = 100; b[7] = 32; b[8] = 32; b[9] = 32; b[10] = 91; b[11] = 79; b[12] = 78; b[13] = 93; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
        buzzer = 10;
      }
      if (line == 1) {
        tone(buzzer, 800, 300);
        delay(200);
        tone(buzzer, 1000, 150);
        line = 0;
      }
      if (but[1] == HIGH) {
        sound = !sound;
        if (sound == true) {
          line = 1;
        }
      }
      phaseScroller(0, 4);
      break;
    case 3:
      if (checkBeat(0) == 1) {
        progress = 255;
      }
      else if (but[1] == HIGH) {
        spooky();
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 42; b[3] = 42; b[4] = 32; b[5] = 76; b[6] = 79; b[7] = 67; b[8] = 75; b[9] = 69; b[10] = 68; b[11] = 32; b[12] = 42; b[13] = 42; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(0, 4);
      break;
    case 4:
      if (checkBeat(0) == 1) {
        progress = 254;
      }
      else if (but[1] == HIGH) {
        spooky();
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 65; b[3] = 76; b[4] = 83; b[5] = 79; b[6] = 32; b[7] = 32; b[8] = 76; b[9] = 79; b[10] = 67; b[11] = 75; b[12] = 69; b[13] = 68; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(0, 4);
      break;
  }
}
byte checkBeat(byte checkWin) {
  if (boop1 == 1 && boop2 == 1 && boop3 == 1 && boop4 == 1 && boop5 == 1 && boop6 == 1 && boop7 == 1 && boop8 == 1) {
    lcd.clear();
    if (checkWin == 1 || checkWin == 2 || checkWin == 3) {
      lcd.setCursor(0, 0); b[0] = 89; b[1] = 79; b[2] = 85; b[3] = 32; b[4] = 70; b[5] = 79; b[6] = 85; b[7] = 78; b[8] = 68; b[9] = 32; b[10] = 65; b[11] = 76; b[12] = 76; b[13] = 32; b[14] = 79; b[15] = 70; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 32; b[1] = 32; b[2] = 32; b[3] = 84; b[4] = 72; b[5] = 69; b[6] = 32; b[7] = 66; b[8] = 79; b[9] = 79; b[10] = 80; b[11] = 83; b[12] = 33; b[13] = 0; lcd.print(x.y(b));
      getLocation();
      delay(500);
      lcd.noDisplay();
      delay(1500);
      lcd.clear();
      lcd.display();
      lcd.setCursor(0, 0); b[0] = 65; b[1] = 108; b[2] = 108; b[3] = 32; b[4] = 109; b[5] = 101; b[6] = 110; b[7] = 117; b[8] = 32; b[9] = 111; b[10] = 112; b[11] = 116; b[12] = 105; b[13] = 111; b[14] = 110; b[15] = 115; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 110; b[1] = 111; b[2] = 119; b[3] = 32; b[4] = 117; b[5] = 110; b[6] = 108; b[7] = 111; b[8] = 99; b[9] = 107; b[10] = 101; b[11] = 100; b[12] = 0; lcd.print(x.y(b));
      getGood();
      DC();
      youWin = true;
      progress = 0;
    }
    return 1;
  }
  if (checkWin == 1 && boop1 == 1 && boop2 == 1 && boop3 == 1) {
    lcd.clear();
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 102; b[5] = 111; b[6] = 117; b[7] = 110; b[8] = 100; b[9] = 32; b[10] = 97; b[11] = 108; b[12] = 108; b[13] = 32; b[14] = 67; b[15] = 65; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(5, 1); b[0] = 98; b[1] = 111; b[2] = 111; b[3] = 112; b[4] = 115; b[5] = 33; b[6] = 0; lcd.print(x.y(b));
    getLocation();
    delay(3000);
    progress = 1;
  }
  if (checkWin == 2 && boop4 == 1 && boop5 == 1) {
    lcd.clear();
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 102; b[5] = 111; b[6] = 117; b[7] = 110; b[8] = 100; b[9] = 32; b[10] = 97; b[11] = 108; b[12] = 108; b[13] = 32; b[14] = 70; b[15] = 76; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(5, 1); b[0] = 98; b[1] = 111; b[2] = 111; b[3] = 112; b[4] = 115; b[5] = 33; b[6] = 0; lcd.print(x.y(b));
    getLocation();
    delay(3000);
    progress = 1;
  }
  if (checkWin == 3 && boop6 == 1 && boop7 == 1 && boop8 == 1) {
    lcd.clear();
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 102; b[5] = 111; b[6] = 117; b[7] = 110; b[8] = 100; b[9] = 32; b[10] = 97; b[11] = 108; b[12] = 108; b[13] = 32; b[14] = 77; b[15] = 84; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(5, 1); b[0] = 98; b[1] = 111; b[2] = 111; b[3] = 112; b[4] = 115; b[5] = 33; b[6] = 0; lcd.print(x.y(b));
    getLocation();
    delay(3000);
    progress = 1;
  }
}
void beaten() {
  lcd.setCursor(0, 0); b[0] = 67; b[1] = 104; b[2] = 111; b[3] = 111; b[4] = 115; b[5] = 101; b[6] = 32; b[7] = 97; b[8] = 110; b[9] = 32; b[10] = 79; b[11] = 112; b[12] = 116; b[13] = 105; b[14] = 111; b[15] = 110; b[16] = 0; lcd.print(x.y(b));
  lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 32; b[3] = 66; b[4] = 101; b[5] = 97; b[6] = 116; b[7] = 32; b[8] = 32; b[9] = 71; b[10] = 97; b[11] = 109; b[12] = 101; b[13] = 32; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
  if (but[1] == HIGH) {
    lcd.clear();
    lcd.setCursor(2, 0); b[0] = 116; b[1] = 105; b[2] = 110; b[3] = 121; b[4] = 117; b[5] = 114; b[6] = 108; b[7] = 46; b[8] = 99; b[9] = 111; b[10] = 109; b[11] = 47; b[12] = 0; lcd.print(x.y(b)); lcd.setCursor(4, 1); b[0] = 121; b[1] = 57; b[2] = 118; b[3] = 53; b[4] = 110; b[5] = 100; b[6] = 56; b[7] = 97; b[8] = 0; lcd.print(x.y(b));
    for (byte x = 0; x < 3; x++) {
      beatGame();
    }
  }
  if (but[0] == HIGH) {
    phaser = 2; progress = 0;
  }
  if (but[2] == HIGH) {
    phaser = 4; progress = 0;
  }
}
void resetGame() {
  lcd.setCursor(1, 0); b[0] = 84; b[1] = 104; b[2] = 105; b[3] = 115; b[4] = 32; b[5] = 119; b[6] = 105; b[7] = 108; b[8] = 108; b[9] = 32; b[10] = 114; b[11] = 101; b[12] = 115; b[13] = 101; b[14] = 116; b[15] = 0; lcd.print(x.y(b));
  lcd.setCursor(0, 1); b[0] = 79; b[1] = 75; b[2] = 32; b[3] = 32; b[4] = 116; b[5] = 104; b[6] = 101; b[7] = 32; b[8] = 103; b[9] = 97; b[10] = 109; b[11] = 101; b[12] = 32; b[13] = 32; b[14] = 78; b[15] = 79; lcd.print(x.y(b));
  if (but[0] == HIGH) {
    boop1 = 0; boop2 = 0; boop3 = 0; boop4 = 0; boop5 = 0; boop6 = 0; boop7 = 0; boop8 = 0;
    for (byte x = 10; x > 0; x--) {
      grind();
      tone(buzzer, (x * 100) + 100, 100);
      grind();
    }
    lcd.clear();
    delay(2000);
    lcd.setCursor(0, 0); b[0] = 87; b[1] = 104; b[2] = 101; b[3] = 114; b[4] = 101; b[5] = 32; b[6] = 119; b[7] = 105; b[8] = 108; b[9] = 108; b[10] = 32; b[11] = 121; b[12] = 111; b[13] = 117; b[14] = 114; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 113; b[1] = 117; b[2] = 101; b[3] = 115; b[4] = 116; b[5] = 32; b[6] = 98; b[7] = 101; b[8] = 103; b[9] = 105; b[10] = 110; b[11] = 63; b[12] = 0; lcd.print(x.y(b));
    funkyTown();
    delay(2000);
    progress = 1;
  }
  if (but[2] == HIGH) {
    progress = 0;
  }
}
void startGame() {
  lcd.setCursor(1, 0); b[0] = 83; b[1] = 101; b[2] = 108; b[3] = 101; b[4] = 99; b[5] = 116; b[6] = 32; b[7] = 97; b[8] = 32; b[9] = 80; b[10] = 108; b[11] = 97; b[12] = 99; b[13] = 101; b[14] = 0; lcd.print(x.y(b));
  switch (phaser) {
    default:
      phaser = 0;
      break;
    case 0:
      if (boop1 == 1 && boop2 == 1 && boop3 == 1) {
        phaser = 1;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 32; b[3] = 67; b[4] = 97; b[5] = 108; b[6] = 105; b[7] = 102; b[8] = 111; b[9] = 114; b[10] = 110; b[11] = 105; b[12] = 97; b[13] = 32; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(0, 2);
      if (but[1] == HIGH) {
        progress = 10;
      }
      break;
    case 1:
      if (boop4 == 1 && boop5 == 1) {
        phaser = 2;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 32; b[3] = 32; b[4] = 70; b[5] = 108; b[6] = 111; b[7] = 114; b[8] = 105; b[9] = 100; b[10] = 97; b[11] = 32; b[12] = 32; b[13] = 32; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(0, 2);
      if (but[1] == HIGH) {
        progress = 50;
      }
      break;
    case 2:
      if (boop6 == 1 && boop7 == 1 && boop8 == 1) {
        phaser = 0;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 32; b[3] = 32; b[4] = 77; b[5] = 111; b[6] = 110; b[7] = 116; b[8] = 97; b[9] = 110; b[10] = 97; b[11] = 32; b[12] = 32; b[13] = 32; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(0, 2);
      if (but[1] == HIGH) {
        progress = 90;
      }
      break;
  }
}

void ca() {
  checkBeat(1);
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 119; b[5] = 97; b[6] = 107; b[7] = 101; b[8] = 32; b[9] = 117; b[10] = 112; b[11] = 32; b[12] = 105; b[13] = 110; b[14] = 32; b[15] = 97; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 113; b[1] = 117; b[2] = 101; b[3] = 101; b[4] = 110; b[5] = 32; b[6] = 115; b[7] = 105; b[8] = 122; b[9] = 101; b[10] = 100; b[11] = 32; b[12] = 98; b[13] = 101; b[14] = 100; b[15] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 73; b[1] = 116; b[2] = 32; b[3] = 108; b[4] = 111; b[5] = 111; b[6] = 107; b[7] = 115; b[8] = 32; b[9] = 110; b[10] = 105; b[11] = 99; b[12] = 101; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 111; b[1] = 117; b[2] = 116; b[3] = 115; b[4] = 105; b[5] = 100; b[6] = 101; b[7] = 0; lcd.print(x.y(b));
    delay(2500);
    phaser++;
  }
  lcd.setCursor(2, 0); b[0] = 84; b[1] = 105; b[2] = 109; b[3] = 101; b[4] = 32; b[5] = 102; b[6] = 111; b[7] = 114; b[8] = 32; b[9] = 97; b[10] = 58; b[11] = 0; lcd.print(x.y(b));
  switch (phaser) {
    default:
      phaser = 1;
      break;
    case 1:
      if (boop1 == 1) {
        phaser = 2;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 81; b[2] = 117; b[3] = 105; b[4] = 99; b[5] = 107; b[6] = 32; b[7] = 108; b[8] = 105; b[9] = 108; b[10] = 32; b[11] = 104; b[12] = 105; b[13] = 107; b[14] = 101; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(1, 3);
      if (but[1] == HIGH) {
        progress = 11;
      }
      break;
    case 2:
      if (boop2 == 1) {
        phaser = 3;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 32; b[3] = 66; b[4] = 101; b[5] = 97; b[6] = 99; b[7] = 104; b[8] = 32; b[9] = 100; b[10] = 97; b[11] = 121; b[12] = 32; b[13] = 32; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(1, 3);
      if (but[1] == HIGH) {
        progress = 14;
      }
      break;
    case 3:
      if (boop3 == 1) {
        phaser = 1;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 67; b[3] = 117; b[4] = 112; b[5] = 32; b[6] = 111; b[7] = 32; b[8] = 99; b[9] = 111; b[10] = 102; b[11] = 102; b[12] = 101; b[13] = 101; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(1, 3);
      if (but[1] == HIGH) {
        progress = 18;
      }
      break;
  }
}
void hike() {
  delay(10);
  if (sixteen == 100 || sixteen != lastSixteen) {
    lcd.setCursor(0, 0); b[0] = 32; b[1] = 32; b[2] = 32; b[3] = 32; b[4] = 32; b[5] = 32; b[6] = 32; b[7] = 32; b[8] = 32; b[9] = 32; b[10] = 32; b[11] = 32; b[12] = 32; b[13] = 32; b[14] = 32; b[15] = 32; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 32; b[1] = 32; b[2] = 255; b[3] = 32; b[4] = 32; b[5] = 32; b[6] = 32; b[7] = 255; b[8] = 255; b[9] = 255; b[10] = 32; b[11] = 32; b[12] = 32; b[13] = 255; b[14] = 32; b[15] = 32; b[16] = 32; lcd.print(x.y(b));
    if (sixteen == 100) {
      sixteen = 0;
    }
    else {
      if (sixteen == 0) {
        tone(buzzer, 50, 20);
      }
      else {
        tone(buzzer, sixteen * 200, 20);
      }
    }
    lastSixteen = sixteen;
  }
  else if (sixteen == 0) {
    lcd.setCursor(sixteen, 1);
    lcd.print((char)252);
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 1 || sixteen == 6 || sixteen == 12) {
    lcd.setCursor(sixteen, 1);
    lcd.print((char)252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[1] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 2 || sixteen == 13) {
    lcd.setCursor(sixteen, 0);
    lcd.print((char)252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 3 || sixteen == 10 || sixteen == 14) {
    lcd.setCursor(sixteen, 1);
    lcd.print((char)252);
    if (but[1] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 4 || sixteen == 5 || sixteen == 11) {
    lcd.setCursor(sixteen, 1);
    lcd.print((char)252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 7 || sixteen == 8 || sixteen == 9) {
    lcd.setCursor(sixteen, 0);
    lcd.print((char)252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 15) {
    lcd.setCursor(sixteen, 1);
    lcd.print((char)252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen = 100;
      progress++;
    }
  }
}
void peak() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 87; b[1] = 104; b[2] = 101; b[3] = 119; b[4] = 44; b[5] = 32; b[6] = 103; b[7] = 111; b[8] = 111; b[9] = 100; b[10] = 32; b[11] = 104; b[12] = 105; b[13] = 107; b[14] = 101; b[15] = 33; lcd.print(x.y(b));
    tone(buzzer, 2800, 90);
    delay(100);
    tone(buzzer, 2600, 90);
    delay(100);
    tone(buzzer, 3000, 90);
    delay(200);
    tone(buzzer, 3200, 180);
    DC();
    lcd.setCursor(0, 0); b[0] = 79; b[1] = 99; b[2] = 101; b[3] = 97; b[4] = 110; b[5] = 32; b[6] = 118; b[7] = 105; b[8] = 101; b[9] = 119; b[10] = 32; b[11] = 97; b[12] = 110; b[13] = 100; b[14] = 32; b[15] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 103; b[1] = 101; b[2] = 111; b[3] = 99; b[4] = 97; b[5] = 99; b[6] = 104; b[7] = 101; b[8] = 32; b[9] = 97; b[10] = 116; b[11] = 32; b[12] = 116; b[13] = 111; b[14] = 112; b[15] = 0; lcd.print(x.y(b));
    delay(2500);
    phaser++;
  }
  lcd.setCursor(2, 0); b[0] = 87; b[1] = 104; b[2] = 97; b[3] = 116; b[4] = 32; b[5] = 116; b[6] = 111; b[7] = 32; b[8] = 100; b[9] = 111; b[10] = 63; b[11] = 0; lcd.print(x.y(b));
  switch (phaser) {
    case 1:
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 71; b[3] = 111; b[4] = 32; b[5] = 98; b[6] = 97; b[7] = 99; b[8] = 107; b[9] = 32; b[10] = 104; b[11] = 111; b[12] = 109; b[13] = 101; b[14] = 32; b[15] = 62; lcd.print(x.y(b)); phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 10;
      }
      break;
    case 2:
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 79; b[2] = 112; b[3] = 101; b[4] = 110; b[5] = 32; b[6] = 116; b[7] = 104; b[8] = 101; b[9] = 32; b[10] = 99; b[11] = 97; b[12] = 99; b[13] = 104; b[14] = 101; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 13;
      }
      break;
  }
}
void geocache() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 72; b[1] = 97; b[2] = 110; b[3] = 100; b[4] = 119; b[5] = 97; b[6] = 114; b[7] = 109; b[8] = 101; b[9] = 114; b[10] = 115; b[11] = 44; b[12] = 32; b[13] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 118; b[1] = 105; b[2] = 115; b[3] = 105; b[4] = 116; b[5] = 111; b[6] = 114; b[7] = 32; b[8] = 108; b[9] = 111; b[10] = 103; b[11] = 44; b[12] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 97; b[1] = 32; b[2] = 115; b[3] = 113; b[4] = 117; b[5] = 105; b[6] = 115; b[7] = 104; b[8] = 121; b[9] = 32; b[10] = 98; b[11] = 97; b[12] = 108; b[13] = 108; b[14] = 44; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 97; b[1] = 110; b[2] = 100; b[3] = 46; b[4] = 46; b[5] = 46; b[6] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 119; b[1] = 104; b[2] = 97; b[3] = 116; b[4] = 39; b[5] = 115; b[6] = 32; b[7] = 116; b[8] = 104; b[9] = 105; b[10] = 115; b[11] = 63; b[12] = 33; b[13] = 0; lcd.print(x.y(b));
    DC();
    boop1 = 1; //GET BOOP 1
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(1, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 102; b[5] = 111; b[6] = 117; b[7] = 110; b[8] = 100; b[9] = 32; b[10] = 98; b[11] = 111; b[12] = 111; b[13] = 112; b[14] = 0; lcd.print(x.y(b));
    lcd.setCursor(5, 1); b[0] = boopNum; b[1] = 32; b[2] = 111; b[3] = 102; b[4] = 32; b[5] = 56; b[6] = 0; lcd.print(x.y(b));
    boopNum++;
    getBoop();
    delay(3500);
    lcd.clear();
    progress = 10;
  }
}
void beach() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 65; b[1] = 102; b[2] = 116; b[3] = 101; b[4] = 114; b[5] = 32; b[6] = 97; b[7] = 32; b[8] = 110; b[9] = 105; b[10] = 99; b[11] = 101; b[12] = 32; b[13] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 99; b[1] = 97; b[2] = 110; b[3] = 121; b[4] = 111; b[5] = 110; b[6] = 32; b[7] = 100; b[8] = 114; b[9] = 105; b[10] = 118; b[11] = 101; b[12] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 121; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 97; b[5] = 114; b[6] = 114; b[7] = 105; b[8] = 118; b[9] = 101; b[10] = 32; b[11] = 97; b[12] = 116; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 80; b[1] = 111; b[2] = 105; b[3] = 110; b[4] = 116; b[5] = 32; b[6] = 68; b[7] = 117; b[8] = 109; b[9] = 101; b[10] = 46; b[11] = 0; lcd.print(x.y(b));
    delay(2500);
    phaser++;
  }
  lcd.setCursor(1, 0); b[0] = 39; b[1] = 83; b[2] = 117; b[3] = 112; b[4] = 32; b[5] = 102; b[6] = 114; b[7] = 101; b[8] = 99; b[9] = 107; b[10] = 108; b[11] = 101; b[12] = 115; b[13] = 63; b[14] = 0; lcd.print(x.y(b));
  switch (phaser) {
    default:
      phaser = 1;
      break;
    case 1:
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 65; b[2] = 114; b[3] = 114; b[4] = 97; b[5] = 110; b[6] = 103; b[7] = 101; b[8] = 32; b[9] = 115; b[10] = 104; b[11] = 101; b[12] = 108; b[13] = 108; b[14] = 115; b[15] = 62; lcd.print(x.y(b));
      if (but[1] == HIGH) {
        progress = 15;
      }
      phaseScroller(1, 3);
      break;
    case 2:
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 69; b[3] = 110; b[4] = 106; b[5] = 111; b[6] = 121; b[7] = 32; b[8] = 100; b[9] = 97; b[10] = 32; b[11] = 115; b[12] = 117; b[13] = 110; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      if (but[1] == HIGH) {
        progress = 16;
      }
      phaseScroller(1, 3);
      break;
    case 3:
      if (boop2 == 0) {
        lcd.setCursor(0, 1); b[0] = 60; b[1] = 71; b[2] = 111; b[3] = 32; b[4] = 105; b[5] = 110; b[6] = 32; b[7] = 100; b[8] = 97; b[9] = 32; b[10] = 119; b[11] = 97; b[12] = 116; b[13] = 101; b[14] = 114; b[15] = 62; lcd.print(x.y(b));
        if (but[1] == HIGH) {
          progress = 17;
        }
      }
      if (boop2 == 1) {
        lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 71; b[3] = 111; b[4] = 32; b[5] = 98; b[6] = 97; b[7] = 99; b[8] = 107; b[9] = 32; b[10] = 104; b[11] = 111; b[12] = 109; b[13] = 101; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
        if (but[1] == HIGH) {
          progress = 10;
        }
      }
      phaseScroller(1, 3);
      break;
  }
}
void drawShell(byte shift) {
  if (phaser != 9) {
    lcd.clear();
  }
  lcd.setCursor(shift, 0);
  lcd.write(byte(0)); lcd.write(byte(1)); lcd.write(byte(2));
  lcd.setCursor(shift, 1);
  lcd.write(byte(3)); lcd.write(byte(4)); lcd.write(byte(5));
}
void drawCap(byte shift, byte line) {
  lcd.clear();
  lcd.setCursor(shift, line);
  lcd.write(byte(6));
}
void playShell() {
  if (shift > 1) {
    if (but[0] == HIGH) {
      shift--;
      drawShell(shift);
    }
    if (but[1] == HIGH) {
      drawShell(shift);
    }
    if (but[2] == HIGH && shift < 9) {
      shift++;
      drawShell(shift);
    }
  }
  else {
    shift = 6;
    numOne++;
    getGood();
    phaser++;
  }
}
void playCap() {
  if (shift < 12) {
    if (but[0] == HIGH && shift > 4) {
      shift--;
      drawCap(shift, line);
    }
    if (but[1] == HIGH) {
      drawCap(shift, line);
    }
    if (but[2] == HIGH) {
      shift++;
      drawCap(shift, line);
    }
  }
  else {
    if (line == 1) {
      line = 0;
    }
    else (line = 1); //lollllll s/o to boolean am i rite
    shift = 6;
    numTwo++;
    getBad();
    phaser++;
  }
}
void shells() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 80; b[1] = 117; b[2] = 116; b[3] = 32; b[4] = 115; b[5] = 104; b[6] = 101; b[7] = 108; b[8] = 108; b[9] = 115; b[10] = 32; b[11] = 111; b[12] = 110; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 116; b[1] = 104; b[2] = 101; b[3] = 32; b[4] = 108; b[5] = 101; b[6] = 102; b[7] = 116; b[8] = 0; lcd.print(x.y(b));
    delay(2500);
    lcd.setCursor(0, 0); b[0] = 97; b[1] = 110; b[2] = 100; b[3] = 32; b[4] = 98; b[5] = 111; b[6] = 116; b[7] = 116; b[8] = 108; b[9] = 101; b[10] = 99; b[11] = 97; b[12] = 112; b[13] = 115; b[14] = 32; b[15] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 111; b[1] = 110; b[2] = 32; b[3] = 116; b[4] = 104; b[5] = 101; b[6] = 32; b[7] = 114; b[8] = 105; b[9] = 103; b[10] = 104; b[11] = 116; b[12] = 46; b[13] = 0; lcd.print(x.y(b));
    delay(2500);
    phaser++;
  }
  lcd.setCursor(0, 0); b[0] = numOne; b[1] = 32; b[2] = 32; b[3] = 124; b[4] = 0; lcd.print(x.y(b)); lcd.setCursor(12, 0); b[0] = 124; b[1] = 32; b[2] = 32; b[3] = numTwo; b[4] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 32; b[1] = 32; b[2] = 32; b[3] = 124; b[4] = 0; lcd.print(x.y(b)); lcd.setCursor(12, 1); b[0] = 124; b[1] = 32; b[2] = 32; b[3] = 32; b[4] = 0; lcd.print(x.y(b));
  switch (phaser) {
    case 1:
      playShell();
      break;
    case 2:
      playCap();
      break;
    case 3:
      playCap();
      break;
    case 4:
      playShell();
      break;
    case 5:
      playShell();
      break;
    case 6:
      playShell();
      break;
    case 7:
      playCap();
      break;
    case 8:
      playShell();
      break;
    case 9:
      lcd.clear();
      lcd.setCursor(0, 0); b[0] = 70; b[1] = 105; b[2] = 118; b[3] = 101; b[4] = 32; b[5] = 112; b[6] = 114; b[7] = 101; b[8] = 116; b[9] = 116; b[10] = 121; b[11] = 32; b[12] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 115; b[1] = 104; b[2] = 101; b[3] = 108; b[4] = 108; b[5] = 115; b[6] = 33; b[7] = 0; lcd.print(x.y(b));
      delay(2500);
      lcd.clear();
      for (byte x = 0; x <= 12; x += 3) {
        drawShell(x);
      }
      delay(4000);
      line = 0;
      progress = 14;
      break;
  }
}
void sun() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 65; b[1] = 112; b[2] = 112; b[3] = 108; b[4] = 121; b[5] = 32; b[6] = 115; b[7] = 117; b[8] = 110; b[9] = 115; b[10] = 99; b[11] = 114; b[12] = 101; b[13] = 101; b[14] = 110; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 119; b[1] = 105; b[2] = 116; b[3] = 104; b[4] = 32; b[5] = 108; b[6] = 101; b[7] = 102; b[8] = 116; b[9] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 65; b[1] = 112; b[2] = 112; b[3] = 108; b[4] = 121; b[5] = 32; b[6] = 116; b[7] = 97; b[8] = 110; b[9] = 110; b[10] = 105; b[11] = 110; b[12] = 103; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 111; b[1] = 105; b[2] = 108; b[3] = 32; b[4] = 119; b[5] = 105; b[6] = 116; b[7] = 104; b[8] = 32; b[9] = 114; b[10] = 105; b[11] = 103; b[12] = 104; b[13] = 116; b[14] = 0; lcd.print(x.y(b));
    DC();
    sixteen = 7;
    startTimer = millis();
    timer = millis();
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(0, 0); b[0] = 112; b[1] = 97; b[2] = 108; b[3] = 101; b[4] = 32; b[5] = 32; b[6] = 32; b[7] = 32; b[8] = 32; b[9] = 32; b[10] = 32; b[11] = 98; b[12] = 117; b[13] = 114; b[14] = 110; b[15] = 116; lcd.print(x.y(b));
    lcd.setCursor(sixteen, 1); lcd.print((char)255); lcd.print((char)255);
    if ((millis() - timer) > 1000 && sixteen != 14) {
      spooky();
      timer = millis();
      lcd.clear();
      sixteen++;
    }
    if (but[0] == HIGH && sixteen != 0) {
      lcd.clear();
      sixteen--;
    }
    if (but[2] == HIGH && sixteen != 14) {
      lcd.clear();
      sixteen++;
    }
    if ((millis() - startTimer) > 10000) {
      phaser++;
    }
  }
  if (phaser == 2) {
    lcd.clear();
    lcd.setCursor(0, 0); b[0] = 65; b[1] = 32; b[2] = 104; b[3] = 117; b[4] = 110; b[5] = 107; b[6] = 121; b[7] = 32; b[8] = 98; b[9] = 101; b[10] = 97; b[11] = 99; b[12] = 104; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 100; b[1] = 111; b[2] = 111; b[3] = 100; b[4] = 32; b[5] = 119; b[6] = 97; b[7] = 108; b[8] = 107; b[9] = 115; b[10] = 32; b[11] = 98; b[12] = 121; b[13] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 72; b[1] = 101; b[2] = 32; b[3] = 115; b[4] = 97; b[5] = 121; b[6] = 115; b[7] = 58; b[8] = 0; lcd.print(x.y(b));
    if (sixteen < 4) {
      lcd.setCursor(0, 1); b[0] = 34; b[1] = 69; b[2] = 104; b[3] = 44; b[4] = 32; b[5] = 115; b[6] = 116; b[7] = 105; b[8] = 108; b[9] = 108; b[10] = 32; b[11] = 99; b[12] = 117; b[13] = 116; b[14] = 101; b[15] = 34; lcd.print(x.y(b));
    }
    else if (sixteen < 8) {
      lcd.setCursor(0, 1); b[0] = 34; b[1] = 76; b[2] = 111; b[3] = 111; b[4] = 107; b[5] = 105; b[6] = 110; b[7] = 32; b[8] = 103; b[9] = 111; b[10] = 111; b[11] = 100; b[12] = 34; b[13] = 0; lcd.print(x.y(b));
    }
    else if (sixteen < 12) {
      lcd.setCursor(0, 1); b[0] = 34; b[1] = 78; b[2] = 105; b[3] = 99; b[4] = 101; b[5] = 32; b[6] = 116; b[7] = 97; b[8] = 110; b[9] = 32; b[10] = 109; b[11] = 105; b[12] = 115; b[13] = 115; b[14] = 121; b[15] = 34; lcd.print(x.y(b));
    }
    else {
      lcd.setCursor(0, 1); b[0] = 34; b[1] = 68; b[2] = 97; b[3] = 122; b[4] = 32; b[5] = 103; b[6] = 111; b[7] = 110; b[8] = 110; b[9] = 97; b[10] = 32; b[11] = 104; b[12] = 117; b[13] = 114; b[14] = 116; b[15] = 34; lcd.print(x.y(b));
    }
    delay(4000);
    sixteen = 100;
    progress = 14;
  }
}
void water() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 84; b[1] = 104; b[2] = 101; b[3] = 32; b[4] = 119; b[5] = 97; b[6] = 118; b[7] = 101; b[8] = 115; b[9] = 32; b[10] = 108; b[11] = 111; b[12] = 111; b[13] = 107; b[14] = 32; b[15] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 112; b[1] = 114; b[2] = 101; b[3] = 116; b[4] = 116; b[5] = 121; b[6] = 32; b[7] = 114; b[8] = 111; b[9] = 117; b[10] = 103; b[11] = 104; b[12] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 77; b[1] = 97; b[2] = 121; b[3] = 98; b[4] = 101; b[5] = 32; b[6] = 106; b[7] = 117; b[8] = 115; b[9] = 116; b[10] = 32; b[11] = 103; b[12] = 101; b[13] = 116; b[14] = 32; b[15] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 121; b[1] = 111; b[2] = 117; b[3] = 114; b[4] = 32; b[5] = 102; b[6] = 101; b[7] = 101; b[8] = 116; b[9] = 32; b[10] = 119; b[11] = 101; b[12] = 116; b[13] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 73; b[1] = 116; b[2] = 39; b[3] = 115; b[4] = 32; b[5] = 99; b[6] = 111; b[7] = 108; b[8] = 100; b[9] = 33; b[10] = 32; b[11] = 65; b[12] = 110; b[13] = 100; b[14] = 32; b[15] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 115; b[1] = 116; b[2] = 105; b[3] = 99; b[4] = 107; b[5] = 121; b[6] = 63; b[7] = 0; lcd.print(x.y(b));
    DC();
    boop2 = 1; //GET BOOP 2
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(0, 0); b[0] = 79; b[1] = 104; b[2] = 44; b[3] = 32; b[4] = 116; b[5] = 104; b[6] = 97; b[7] = 116; b[8] = 32; b[9] = 105; b[10] = 115; b[11] = 32; b[12] = 98; b[13] = 111; b[14] = 111; b[15] = 112; lcd.print(x.y(b));
    lcd.setCursor(5, 1); b[0] = boopNum; b[1] = 32; b[2] = 111; b[3] = 102; b[4] = 32; b[5] = 56; b[6] = 0; lcd.print(x.y(b));
    getBoop();
    boopNum++;
    delay(3500);
    lcd.clear();
    progress = 14;
  }
}
void grind() {
  for (byte i = 0; i < 16; i++) {
    b[i] = random(255);
  }
  lcd.setCursor(0, 0);
  lcd.print(x.y(b));
  for (byte i = 0; i < 16; i++) {
    b[i] = random(255);
  }
  lcd.setCursor(0, 1);
  lcd.print(x.y(b));
}
void coffee() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 71; b[1] = 114; b[2] = 105; b[3] = 110; b[4] = 100; b[5] = 32; b[6] = 116; b[7] = 104; b[8] = 101; b[9] = 32; b[10] = 98; b[11] = 101; b[12] = 97; b[13] = 110; b[14] = 115; b[15] = 33; lcd.print(x.y(b));
    delay(2500);
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(0, 0); b[0] = 242; b[1] = 242; b[2] = 242; b[3] = 242; b[4] = 242; b[5] = 242; b[6] = 242; b[7] = 242; b[8] = 242; b[9] = 242; b[10] = 242; b[11] = 242; b[12] = 242; b[13] = 242; b[14] = 242; b[15] = 242; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 242; b[1] = 242; b[2] = 242; b[3] = 242; b[4] = 242; b[5] = 242; b[6] = 242; b[7] = 242; b[8] = 242; b[9] = 242; b[10] = 242; b[11] = 242; b[12] = 242; b[13] = 242; b[14] = 242; b[15] = 242; lcd.print(x.y(b));
    if (but[0] == HIGH) {
      grind();
      tone(buzzer, 130, 100);
      delay(100);
      grind();
      tone(buzzer, 90, 100);
      delay(100);
      grind();
      tone(buzzer, 170, 100);
      delay(100);
      phaser++;
    }
  }
  if (phaser == 2) {
    lcd.setCursor(0, 0); b[0] = 192; b[1] = 192; b[2] = 192; b[3] = 192; b[4] = 192; b[5] = 192; b[6] = 192; b[7] = 192; b[8] = 192; b[9] = 192; b[10] = 192; b[11] = 192; b[12] = 192; b[13] = 192; b[14] = 192; b[15] = 192; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 192; b[1] = 192; b[2] = 192; b[3] = 192; b[4] = 192; b[5] = 192; b[6] = 192; b[7] = 192; b[8] = 192; b[9] = 192; b[10] = 192; b[11] = 192; b[12] = 192; b[13] = 192; b[14] = 192; b[15] = 192; lcd.print(x.y(b));
    if (but[1] == HIGH) {
      grind();
      tone(buzzer, 160, 100);
      delay(100);
      grind();
      tone(buzzer, 100, 100);
      delay(100);
      grind();
      tone(buzzer, 180, 100);
      delay(100);
      phaser++;
    }
  }
  if (phaser == 3) {
    lcd.setCursor(0, 0); b[0] = 243; b[1] = 243; b[2] = 243; b[3] = 226; b[4] = 243; b[5] = 243; b[6] = 243; b[7] = 243; b[8] = 243; b[9] = 161; b[10] = 243; b[11] = 243; b[12] = 243; b[13] = 243; b[14] = 243; b[15] = 243; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 243; b[1] = 243; b[2] = 243; b[3] = 243; b[4] = 243; b[5] = 229; b[6] = 243; b[7] = 243; b[8] = 243; b[9] = 243; b[10] = 243; b[11] = 243; b[12] = 243; b[13] = 243; b[14] = 177; b[15] = 243; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 68; b[1] = 111; b[2] = 32; b[3] = 121; b[4] = 111; b[5] = 117; b[6] = 32; b[7] = 115; b[8] = 101; b[9] = 101; b[10] = 32; b[11] = 116; b[12] = 104; b[13] = 97; b[14] = 116; b[15] = 33; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 66; b[1] = 111; b[2] = 111; b[3] = 112; b[4] = 32; b[5] = 105; b[6] = 110; b[7] = 32; b[8] = 116; b[9] = 104; b[10] = 101; b[11] = 32; b[12] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 99; b[1] = 111; b[2] = 102; b[3] = 102; b[4] = 101; b[5] = 101; b[6] = 32; b[7] = 103; b[8] = 114; b[9] = 111; b[10] = 117; b[11] = 110; b[12] = 100; b[13] = 115; b[14] = 33; b[15] = 0; lcd.print(x.y(b));
    delay(2500);
    lcd.setCursor(0, 0); b[0] = 243; b[1] = 243; b[2] = 243; b[3] = 226; b[4] = 243; b[5] = 243; b[6] = 243; b[7] = 243; b[8] = 243; b[9] = 161; b[10] = 243; b[11] = 243; b[12] = 243; b[13] = 243; b[14] = 243; b[15] = 243; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 243; b[1] = 243; b[2] = 243; b[3] = 243; b[4] = 243; b[5] = 229; b[6] = 243; b[7] = 243; b[8] = 243; b[9] = 243; b[10] = 243; b[11] = 243; b[12] = 243; b[13] = 243; b[14] = 177; b[15] = 243; lcd.print(x.y(b));
    DC();
    boop3 = 1; //GET BOOP 3
    phaser++;
  }
  if (phaser == 4) {
    lcd.setCursor(0, 0); b[0] = 73; b[1] = 116; b[2] = 32; b[3] = 105; b[4] = 115; b[5] = 32; b[6] = 116; b[7] = 97; b[8] = 115; b[9] = 116; b[10] = 121; b[11] = 32; b[12] = 98; b[13] = 111; b[14] = 111; b[15] = 112; lcd.print(x.y(b));
    lcd.setCursor(5, 1); b[0] = boopNum; b[1] = 32; b[2] = 111; b[3] = 102; b[4] = 32; b[5] = 56; b[6] = 0; lcd.print(x.y(b));
    boopNum++;
    getBoop();
    delay(3500);
    lcd.clear();
    progress = 10;
  }
}

void fl() {
  checkBeat(2);
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 73; b[1] = 116; b[2] = 39; b[3] = 115; b[4] = 32; b[5] = 97; b[6] = 32; b[7] = 115; b[8] = 117; b[9] = 110; b[10] = 110; b[11] = 121; b[12] = 32; b[13] = 100; b[14] = 97; b[15] = 121; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 97; b[1] = 110; b[2] = 100; b[3] = 32; b[4] = 121; b[5] = 111; b[6] = 117; b[7] = 32; b[8] = 97; b[9] = 114; b[10] = 101; b[11] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 105; b[1] = 110; b[2] = 32; b[3] = 116; b[4] = 104; b[5] = 101; b[6] = 32; b[7] = 103; b[8] = 114; b[9] = 97; b[10] = 115; b[11] = 115; b[12] = 32; b[13] = 98; b[14] = 121; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 80; b[1] = 104; b[2] = 121; b[3] = 115; b[4] = 105; b[5] = 99; b[6] = 115; b[7] = 0; lcd.print(x.y(b));
    delay(2500);
    phaser++;
  }
  if (boop4 == 0 && boop5 == 0) {
    lcd.setCursor(1, 0); b[0] = 68; b[1] = 111; b[2] = 32; b[3] = 119; b[4] = 104; b[5] = 97; b[6] = 116; b[7] = 32; b[8] = 102; b[9] = 105; b[10] = 114; b[11] = 115; b[12] = 116; b[13] = 63; b[14] = 0; lcd.print(x.y(b));
  }
  else {
    lcd.setCursor(1, 0); b[0] = 78; b[1] = 111; b[2] = 119; b[3] = 32; b[4] = 121; b[5] = 111; b[6] = 117; b[7] = 32; b[8] = 104; b[9] = 97; b[10] = 118; b[11] = 101; b[12] = 116; b[13] = 97; b[14] = 0; lcd.print(x.y(b));
  }
  switch (phaser) {
    default:
      phaser = 1;
      break;
    case 1:
      if (boop4 == 1) {
        phaser = 2;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 32; b[3] = 81; b[4] = 117; b[5] = 105; b[6] = 100; b[7] = 32; b[8] = 80; b[9] = 97; b[10] = 114; b[11] = 116; b[12] = 121; b[13] = 32; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 51;
      }
      break;
    case 2:
      if (boop5 == 1) {
        phaser = 1;
      }
      lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 83; b[3] = 116; b[4] = 117; b[5] = 100; b[6] = 121; b[7] = 32; b[8] = 97; b[9] = 116; b[10] = 32; b[11] = 76; b[12] = 105; b[13] = 98; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
      phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 52;
      }
      break;
  }
}
void party() {
  if (phaser == 0) {
    toggler = false;
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 114; b[5] = 111; b[6] = 108; b[7] = 108; b[8] = 32; b[9] = 117; b[10] = 112; b[11] = 32; b[12] = 119; b[13] = 105; b[14] = 116; b[15] = 104; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 97; b[1] = 32; b[2] = 100; b[3] = 111; b[4] = 117; b[5] = 98; b[6] = 108; b[7] = 101; b[8] = 32; b[9] = 98; b[10] = 111; b[11] = 116; b[12] = 116; b[13] = 108; b[14] = 101; b[15] = 0; lcd.print(x.y(b));
    delay(2500);
    lcd.setCursor(0, 0); b[0] = 69; b[1] = 118; b[2] = 101; b[3] = 114; b[4] = 121; b[5] = 111; b[6] = 110; b[7] = 101; b[8] = 32; b[9] = 101; b[10] = 108; b[11] = 115; b[12] = 101; b[13] = 32; b[14] = 105; b[15] = 115; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 116; b[1] = 97; b[2] = 107; b[3] = 105; b[4] = 110; b[5] = 103; b[6] = 32; b[7] = 115; b[8] = 104; b[9] = 111; b[10] = 116; b[11] = 115; b[12] = 46; b[13] = 46; b[14] = 46; b[15] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 46; b[1] = 46; b[2] = 46; b[3] = 111; b[4] = 102; b[5] = 32; b[6] = 116; b[7] = 101; b[8] = 113; b[9] = 117; b[10] = 105; b[11] = 108; b[12] = 108; b[13] = 97; b[14] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 83; b[1] = 111; b[2] = 32; b[3] = 121; b[4] = 111; b[5] = 117; b[6] = 32; b[7] = 115; b[8] = 116; b[9] = 97; b[10] = 114; b[11] = 116; b[12] = 32; b[13] = 111; b[14] = 110; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 116; b[1] = 104; b[2] = 101; b[3] = 32; b[4] = 100; b[5] = 111; b[6] = 117; b[7] = 98; b[8] = 108; b[9] = 101; b[10] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 80; b[1] = 111; b[2] = 117; b[3] = 114; b[4] = 32; b[5] = 119; b[6] = 105; b[7] = 110; b[8] = 101; b[9] = 32; b[10] = 119; b[11] = 105; b[12] = 116; b[13] = 104; b[14] = 32; b[15] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 99; b[1] = 101; b[2] = 110; b[3] = 116; b[4] = 101; b[5] = 114; b[6] = 32; b[7] = 98; b[8] = 117; b[9] = 116; b[10] = 116; b[11] = 111; b[12] = 110; b[13] = 33; b[14] = 0; lcd.print(x.y(b));
    DC();
    num = 0;
    timer = 0;
    phaser++;
  }
  if (phaser == 1) {
    if ((millis() - timer) > 1000) {
      num++;
      if (num == 17) {
        lcd.setCursor(0, 0); b[0] = 32; b[1] = 32; b[2] = 32; b[3] = 32; b[4] = 32; b[5] = 32; b[6] = 32; b[7] = 32; b[8] = 32; b[9] = 32; b[10] = 32; b[11] = 32; b[12] = 32; b[13] = 32; b[14] = 32; b[15] = 32; lcd.print(x.y(b));
        num = 1;
      }
      lcd.setCursor(0, 0); b[0] = 32; b[1] = 32; b[2] = 32; b[3] = 32; b[4] = 32; b[5] = 32; b[6] = 32; b[7] = 32; b[8] = 32; b[9] = 32; b[10] = 32; b[11] = 32; b[12] = 32; b[13] = 32; b[14] = 32; b[15] = 32; lcd.print(x.y(b));
      lcd.setCursor(nums[num - 1], 0); lcd.write(byte(7));
      timer = millis();
      if (num > 1) {
        toggler = true;
      } 
      if (toggler == true) {
        tone(buzzer, 500, 10);
      }
    }
    lcd.setCursor(0, 1); b[0] = 32; b[1] = 32; b[2] = 32; b[3] = 32; b[4] = 32; b[5] = 32; b[6] = 32; b[7] = 32; b[8] = 32; b[9] = 32; b[10] = 32; b[11] = 32; b[12] = 32; b[13] = 32; b[14] = 32; b[15] = 32;
    switch (level) {
      case 0:
        b[2] = 89;
        break;
      case 1:
        b[10] = 89;
        break;
      case 2:
        b[5] = 89;
        break;
      case 3:
        b[13] = 89;
        break;
    }
    if (but[1] == HIGH) {
      byte clicked = nums[num - 1];
      if (b[clicked] == 89) {
        getGood();
        b[clicked] = 92;
        level++;
        lcd.print(x.y(b));
        delay(1500);
      }
      else {
        getBad();
      }
    }
    else {
      lcd.print(x.y(b));
    }
    if (level == 4) {
      lcd.clear();
      phaser++;
    }
  }
  if (phaser == 2) {
    level = 0;
    boop4 = 1; //GET BOOP 4
    lcd.setCursor(0, 0); b[0] = 65; b[1] = 102; b[2] = 116; b[3] = 101; b[4] = 114; b[5] = 32; b[6] = 52; b[7] = 32; b[8] = 103; b[9] = 108; b[10] = 97; b[11] = 115; b[12] = 115; b[13] = 101; b[14] = 115; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 111; b[1] = 102; b[2] = 32; b[3] = 119; b[4] = 105; b[5] = 110; b[6] = 101; b[7] = 46; b[8] = 46; b[9] = 46; b[10] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 39; b[4] = 114; b[5] = 101; b[6] = 32; b[7] = 102; b[8] = 101; b[9] = 101; b[10] = 108; b[11] = 105; b[12] = 110; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 112; b[1] = 114; b[2] = 101; b[3] = 116; b[4] = 116; b[5] = 121; b[6] = 32; b[7] = 115; b[8] = 108; b[9] = 101; b[10] = 101; b[11] = 112; b[12] = 121; b[13] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 83; b[1] = 111; b[2] = 32; b[3] = 121; b[4] = 111; b[5] = 117; b[6] = 32; b[7] = 104; b[8] = 101; b[9] = 97; b[10] = 100; b[11] = 32; b[12] = 116; b[13] = 111; b[14] = 32; b[15] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 98; b[1] = 101; b[2] = 100; b[3] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 66; b[1] = 117; b[2] = 116; b[3] = 32; b[4] = 121; b[5] = 111; b[6] = 117; b[7] = 114; b[8] = 32; b[9] = 98; b[10] = 101; b[11] = 100; b[12] = 32; b[13] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 105; b[1] = 115; b[2] = 110; b[3] = 39; b[4] = 116; b[5] = 32; b[6] = 116; b[7] = 104; b[8] = 101; b[9] = 114; b[10] = 101; b[11] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 73; b[1] = 110; b[2] = 115; b[3] = 116; b[4] = 101; b[5] = 97; b[6] = 100; b[7] = 46; b[8] = 46; b[9] = 46; b[10] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 73; b[1] = 116; b[2] = 39; b[3] = 115; b[4] = 32; b[5] = 115; b[6] = 108; b[7] = 101; b[8] = 101; b[9] = 112; b[10] = 121; b[11] = 32; b[12] = 98; b[13] = 111; b[14] = 111; b[15] = 112; lcd.print(x.y(b));
    lcd.setCursor(5, 1); b[0] = boopNum; b[1] = 32; b[2] = 111; b[3] = 102; b[4] = 32; b[5] = 56; b[6] = 0; lcd.print(x.y(b));
    boopNum++;
    getBoop();
    delay(3500);
    lcd.clear();
    progress = 50;
  }
}
void study() {
  if (sixteen != 100) {
    phaser = 2;
  }
  if (sixteen == 7) {
    phaser = 3;
  }
  switch (phaser) {
    case 0:
      lcd.setCursor(0, 0); b[0] = 98; b[1] = 108; b[2] = 97; b[3] = 104; b[4] = 32; b[5] = 98; b[6] = 108; b[7] = 97; b[8] = 104; b[9] = 0; lcd.print(x.y(b));
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0); b[0] = 109; b[1] = 105; b[2] = 116; b[3] = 99; b[4] = 104; b[5] = 111; b[6] = 110; b[7] = 100; b[8] = 114; b[9] = 105; b[10] = 97; b[11] = 32; b[12] = 98; b[13] = 108; b[14] = 97; b[15] = 104; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 98; b[1] = 108; b[2] = 97; b[3] = 104; b[4] = 0; lcd.print(x.y(b));
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0); b[0] = 74; b[1] = 117; b[2] = 115; b[3] = 116; b[4] = 32; b[5] = 109; b[6] = 101; b[7] = 109; b[8] = 111; b[9] = 114; b[10] = 105; b[11] = 122; b[12] = 101; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 116; b[1] = 104; b[2] = 101; b[3] = 32; b[4] = 112; b[5] = 97; b[6] = 116; b[7] = 116; b[8] = 101; b[9] = 114; b[10] = 110; b[11] = 0; lcd.print(x.y(b));
      DC();
      lcd.setCursor(0, 0); b[0] = 97; b[1] = 110; b[2] = 100; b[3] = 32; b[4] = 121; b[5] = 111; b[6] = 117; b[7] = 39; b[8] = 108; b[9] = 108; b[10] = 32; b[11] = 98; b[12] = 101; b[13] = 32; b[14] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 102; b[1] = 105; b[2] = 110; b[3] = 101; b[4] = 46; b[5] = 46; b[6] = 46; b[7] = 0; lcd.print(x.y(b));
      DC();
      phaser++;
      break;
    case 1:
      lcd.setCursor(5, 0); b[0] = 66; b[1] = 69; b[2] = 71; b[3] = 73; b[4] = 78; b[5] = 33; b[6] = 0; lcd.print(x.y(b));
      tone(buzzer, 391, 120);
      delay(133);
      delay(266);
      tone(buzzer, 391, 120);
      delay(133);
      tone(buzzer, 523, 180);
      delay(1000);
      sixteen = 2;
      num = 0;
      phaser++;
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Chapter  ");
      lcd.print(level + 1);
      delay(1500);
      lcd.clear();
      delay(500);
      show(sixteen);
      lcd.setCursor(1, 0); b[0] = 78; b[1] = 111; b[2] = 119; b[3] = 32; b[4] = 121; b[5] = 111; b[6] = 117; b[7] = 114; b[8] = 32; b[9] = 116; b[10] = 117; b[11] = 114; b[12] = 110; b[13] = 33; b[14] = 0; lcd.print(x.y(b));
      delay(1000);
      lcd.clear();
      progress = 53;
      break;
    case 3:
      level = 0;
      boop5 = 1; //GET BOOP 5
      lcd.setCursor(0, 0); b[0] = 87; b[1] = 101; b[2] = 108; b[3] = 108; b[4] = 32; b[5] = 116; b[6] = 104; b[7] = 97; b[8] = 116; b[9] = 32; b[10] = 119; b[11] = 97; b[12] = 115; b[13] = 110; b[14] = 39; b[15] = 116; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 115; b[1] = 111; b[2] = 32; b[3] = 98; b[4] = 97; b[5] = 100; b[6] = 0; lcd.print(x.y(b));
      DC();
      lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 115; b[5] = 116; b[6] = 97; b[7] = 114; b[8] = 116; b[9] = 32; b[10] = 116; b[11] = 111; b[12] = 32; b[13] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 112; b[1] = 97; b[2] = 99; b[3] = 107; b[4] = 32; b[5] = 117; b[6] = 112; b[7] = 0; lcd.print(x.y(b));
      DC();
      lcd.setCursor(0, 0); b[0] = 66; b[1] = 117; b[2] = 116; b[3] = 32; b[4] = 121; b[5] = 111; b[6] = 117; b[7] = 114; b[8] = 32; b[9] = 98; b[10] = 97; b[11] = 103; b[12] = 32; b[13] = 105; b[14] = 115; b[15] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 102; b[1] = 117; b[2] = 108; b[3] = 108; b[4] = 32; b[5] = 111; b[6] = 102; b[7] = 46; b[8] = 46; b[9] = 46; b[10] = 0; lcd.print(x.y(b));
      DC();
      lcd.setCursor(0, 0); b[0] = 67; b[1] = 97; b[2] = 102; b[3] = 102; b[4] = 105; b[5] = 110; b[6] = 97; b[7] = 116; b[8] = 101; b[9] = 100; b[10] = 32; b[11] = 32; b[12] = 98; b[13] = 111; b[14] = 111; b[15] = 112; lcd.print(x.y(b));
      lcd.setCursor(5, 1); b[0] = boopNum; b[1] = 32; b[2] = 111; b[3] = 102; b[4] = 32; b[5] = 56; b[6] = 0; lcd.print(x.y(b));
      boopNum++;
      getBoop();
      delay(3500);
      lcd.clear();
      sixteen = 100;
      progress = 50;
  }
}
void mem(byte shift) {
  lcd.setCursor(shift, 0); lcd.print((char)255); lcd.print((char)255);
  lcd.setCursor(shift, 1); lcd.print((char)255); lcd.print((char)255);
  byte note;
  switch (shift) {
    case 1:
      note = 250;
      break;
    case 7:
      note = 750;
      break;
    case 13:
      note = 1250;
      break;
  }
  tone(buzzer, note, 100);
  delay(500);
  lcd.clear();
}
void show(byte level) {
  for (byte x = 0; x <= level; x++) {
    if (nums[x] <= 5) {
      mem(1);
    }
    else if (nums[x] <= 10) {
      mem(7);
    }
    else if (nums[x] <= 15) {
      mem(13);
    }
    delay(1000);
  }
}
void memorize() {
  if (nums[num] <= 5) {
    if (but[0] == HIGH) {
      mem(1);
      num++;
    }
    if (but[1] == HIGH || but[2] == HIGH) {
      getBad();
    }
  }
  else if (nums[num] <= 10) {
    if (but[1] == HIGH) {
      mem(7);
      num++;
    }
    if (but[0] == HIGH || but[2] == HIGH) {
      getBad();
    }
  }
  else if (nums[num] <= 15) {
    if (but[2] == HIGH) {
      mem(13);
      num++;
    }
    if (but[1] == HIGH || but[0] == HIGH) {
      getBad();
    }
  }
  if (num == (sixteen + 1)) {
    level++;
    num = 0;
    sixteen++;
    progress = 52;
  }
}

void mt() {
  if (score == 100 && phaser == 0) {
    phaser = 1;
  }
  if (phaser == 0) {
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 97; b[5] = 114; b[6] = 101; b[7] = 32; b[8] = 102; b[9] = 108; b[10] = 111; b[11] = 97; b[12] = 116; b[13] = 105; b[14] = 110; b[15] = 103; b[16] = 32; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 97; b[1] = 114; b[2] = 111; b[3] = 117; b[4] = 110; b[5] = 100; b[6] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 105; b[1] = 110; b[2] = 32; b[3] = 97; b[4] = 32; b[5] = 116; b[6] = 117; b[7] = 98; b[8] = 101; b[9] = 32; b[10] = 97; b[11] = 116; b[12] = 32; b[13] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 72; b[1] = 121; b[2] = 97; b[3] = 108; b[4] = 105; b[5] = 116; b[6] = 101; b[7] = 46; b[8] = 46; b[9] = 46; b[10] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 89; b[1] = 111; b[2] = 117; b[3] = 32; b[4] = 115; b[5] = 101; b[6] = 101; b[7] = 32; b[8] = 0; lcd.print(x.y(b)); lcd.setCursor(0, 1); b[0] = 115; b[1] = 111; b[2] = 109; b[3] = 101; b[4] = 116; b[5] = 104; b[6] = 105; b[7] = 110; b[8] = 103; b[9] = 32; b[10] = 115; b[11] = 104; b[12] = 105; b[13] = 110; b[14] = 121; b[15] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(0, 0); b[0] = 100; b[1] = 111; b[2] = 119; b[3] = 110; b[4] = 32; b[5] = 98; b[6] = 101; b[7] = 108; b[8] = 111; b[9] = 119; b[10] = 46; b[11] = 46; b[12] = 46; b[13] = 0; lcd.print(x.y(b));
    DC();
    lcd.setCursor(2, 0); b[0] = 68; b[1] = 105; b[2] = 118; b[3] = 101; b[4] = 32; b[5] = 102; b[6] = 111; b[7] = 114; b[8] = 32; b[9] = 116; b[10] = 104; b[11] = 101; b[12] = 32; b[13] = 0; lcd.print(x.y(b)); lcd.setCursor(3, 1); b[0] = 116; b[1] = 114; b[2] = 101; b[3] = 97; b[4] = 115; b[5] = 117; b[6] = 114; b[7] = 101; b[8] = 33; b[9] = 33; b[10] = 0; lcd.print(x.y(b));
    DC();
    phaser++;
  }
  if (phaser == 1) {
    shift = 14;
    lastSixteen = sixteen;
    score = 0;
    num = 0;
    level = 1;
    startTimer = millis();
    timer = millis();
    phaser++;
  }
  if (phaser == 2) {
    if (toggler == true) {
      lcd.setCursor(nums[num], level); lcd.print("0");
    }
    lcd.setCursor(shift, line); lcd.print("(8");
    if (but[0] == HIGH && shift > 0) {
      shift--;
      lcd.clear();
    }
    if (but[1] == HIGH) {
      if (line == 0) {
        line = 1;
      }
      else {
        line = 0;
      }
      lcd.clear();
    }
    if ((millis() - startTimer) > 1000 && shift < 14 && but[0] == LOW && buttonPow == true) {
      shift++;
      startTimer = millis();
      lcd.clear();
    }
    if (((millis() - timer) % 3500) == 10) {
      if (sixteen == lastSixteen) {
        tone(buzzer, 1500, 10);
        toggler = true;
        sixteen++;
        if (sixteen % 2 == 0) {
          level = 0;
        }
        else {
          level = 1;
        }
      }
      else {
        toggler = false;
        lcd.setCursor(nums[num], level); lcd.print(" ");
        lastSixteen = sixteen;
        num++;
        if (num == 2 || num == 7) { //don't hit top plz
          num++;
        }
      }
    }
    if (shift == nums[num] && line == level) {
      getGood();
      score++;
      level = 100;
      toggler = false;
    }
    if (num == 9) {
      shift = 5;
      level = 0;
      line = 0;
      sixteen = 100;
      startTimer = millis();
      timer = millis();
      phaser++;
    }
  }
  if (phaser == 3) {
    lcd.setCursor(0, 0);
    if ((millis() - startTimer) > 350) {
      startTimer = millis();
      if (toggler == true) {
        tone(buzzer, 100, 10);
      }
      else {
        tone(buzzer, 300, 10);
      }
      toggler = !toggler;
      lcd.clear();
    }
    if (toggler == true) {
      lcd.print("o_O");
    }
    if (toggler == false) {
      lcd.print("O_o");
    }
    lcd.print(" hmm "); lcd.print(score); lcd.print(" gold..");
    if ((millis() - timer) > 4000) {
      lcd.setCursor(0, 1); lcd.print("You get");
      if (score <= 2) {
        lcd.print(" 0 boops");
      }
      else if (score <= 4) {
        lcd.print(" 1 boop");
      }
      else if (score <= 6) {
        lcd.print(" 2 boops");
      }
      else if (score == 7) {
        lcd.print(" 3 boops");
      }
    }
    if ((millis() - timer) > 8000) {
      phaser++;
    }
  }
  if (phaser == 4) {
    if (score <= 2) {
      progress = 91;
    }
    else if (score <= 4) {
      if (boop6 == 0) {
        boop6 = 1;
        boopNum++;
      }
      else if (boop7 == 0) {
        boop7 = 1;
        boopNum++;
      }
      else if (boop8 == 0) {
        boop8 = 1;
        boopNum++;
      }
      getBoop();
      delay(500);
      progress = 91;
    }
    else if (score <= 6) {
      if (boop6 == 0) {
        boop6 = 1;
        boop7 = 1;
        boopNum += 2;
      }
      else if (boop7 == 0) {
        boop7 = 1;
        boop8 = 1;
        boopNum += 2;
      }
      else if (boop8 == 0) {
        boop8 = 1;
        boopNum++;
      }
      getBoop();
      delay(500);
      getBoop();
      delay(500);
      progress = 91;
    }
    else if (score == 7) {
      if (boop6 == 0) {
        boopNum += 3;
      }
      else if (boop7 == 0) {
        boopNum += 2;
      }
      else if (boop8 == 0) {
        boopNum++;
      }
      boop6 = 1;
      boop7 = 1;
      boop8 = 1;
      getBoop();
      delay(500);
      getBoop();
      delay(500);
      getBoop();
      delay(500);
      progress = 91;
    }
  }
}
void playAgain() {
  checkBeat(3);
  score = 100;
  lcd.setCursor(0, 0); b[0] = 32; b[1] = 32; b[2] = 80; b[3] = 108; b[4] = 97; b[5] = 121; b[6] = 32; b[7] = 32; b[8] = 97; b[9] = 103; b[10] = 97; b[11] = 105; b[12] = 110; b[13] = 63; b[14] = 32; b[15] = 32; lcd.print(x.y(b));
  lcd.setCursor(0, 1); b[0] = 32; b[1] = 32; b[2] = 79; b[3] = 75; b[4] = 32; b[5] = 32; b[6] = 32; b[7] = 32; b[8] = 32; b[9] = 32; b[10] = 32; b[11] = 32; b[12] = 78; b[13] = 79; b[14] = 32; b[15] = 32; lcd.print(x.y(b));
  if (but[0] == HIGH) {
    progress = 90;
  }
  if (but[2] == HIGH) {
    progress = 1;
  }
}

void piano() {
  lcd.setCursor(0, 0); b[0] = 67; b[1] = 104; b[2] = 111; b[3] = 111; b[4] = 115; b[5] = 101; b[6] = 32; b[7] = 97; b[8] = 110; b[9] = 32; b[10] = 79; b[11] = 112; b[12] = 116; b[13] = 105; b[14] = 111; b[15] = 110; b[16] = 0; lcd.print(x.y(b));
  lcd.setCursor(0, 1); b[0] = 60; b[1] = 32; b[2] = 32; b[3] = 84; b[4] = 111; b[5] = 121; b[6] = 32; b[7] = 32; b[8] = 79; b[9] = 114; b[10] = 103; b[11] = 97; b[12] = 110; b[13] = 32; b[14] = 32; b[15] = 62; lcd.print(x.y(b));
  if (but[1] == HIGH) {
    progress = 253;
  }
  if (but[0] == HIGH) {
    phaser = 3; progress = 0;
  }
  if (but[2] == HIGH) {
    phaser = 0; progress = 0;
  }
}
void playPiano() {
  lcd.setCursor(7, 0);
  if (but[0] == HIGH && but[1] == LOW && but[2] == LOW) {
    lcd.write(byte(0)); lcd.write(byte(1)); lcd.write(byte(2));
    lcd.setCursor(7, 1);
    lcd.write(byte(3)); lcd.write(byte(4)); lcd.write(byte(5));
    tone(buzzer, 100);
  }
  if (but[0] == HIGH && but[1] == HIGH && but[2] == LOW) {
    lcd.print((char)188);
    tone(buzzer, 200);
  }
  if (but[0] == LOW && but[1] == HIGH && but[2] == LOW) {
    lcd.write(byte(6));
    tone(buzzer, 300);
  }
  if (but[0] == LOW && but[1] == HIGH && but[2] == HIGH) {
    lcd.print((char)239);
    tone(buzzer, 400);
  }
  if (but[0] == LOW && but[1] == LOW && but[2] == HIGH) {
    lcd.write(byte(7));
    tone(buzzer, 500);
  }
  if (but[0] == HIGH && but[1] == LOW && but[2] == HIGH) {
    lcd.print((char)252);
    tone(buzzer, 250);
  }
  if (but[0] == LOW && but[1] == LOW && but[2] == LOW) {
    lcd.clear();
    noTone(buzzer);
  }
  if (but[0] == HIGH && but[1] == HIGH && but[2] == HIGH) {
    lcd.clear();
    lcd.setCursor(4,0);b[0]=69;b[1]=120;b[2]=105;b[3]=116;b[4]=105;b[5]=110;b[6]=103;b[7]=0;lcd.print(x.y(b));
    lcd.setCursor(5,1);b[0]=80;b[1]=105;b[2]=97;b[3]=110;b[4]=111;b[5]=0;lcd.print(x.y(b));
    noTone(buzzer);
    progress = 254;
    delay(3000);
  }
}
