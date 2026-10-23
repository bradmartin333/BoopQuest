// BoopQuest, transcribed from Arduino/BoopQuest.ino to run in the browser.
// The game logic is kept line-for-line with the sketch; blocking calls became async/await and the
// LCD byte arrays (b[0] = 87; ...) became string literals. The hardware lives in arduino.js/lcd.js.
import {
  HIGH, LOW, INPUT, OUTPUT, millis, delay, pinMode, digitalWrite, digitalRead, analogWrite,
  analogRead, random, randomSeed, tone, noTone,
} from './arduino.js';
import { LiquidCrystal } from './lcd.js';

export const lcd = new LiquidCrystal();
//Buttons and debouncing
let but = [0, 0, 0]; let debouncer = 13; let lastPressTime = 0; let pressDelay = 300;
let b = new Array(17).fill(0); //For sending text
let brightness = 2; //Default to MED
let buzzer = 10; let sound = true;
//Game organization
let progress = 0; let lastProgress = 0; let phaser = 0; let lastPhaser = 0;
let boopNum = 49; let boop1 = 0; let boop2 = 0; let boop3 = 0; let boop4 = 0; let boop5 = 0; let boop6 = 0; let boop7 = 0; let boop8 = 0; //Collect all the boops to win
let youWin = false;
//MiniGames
let sixteen = 100; let lastSixteen = 0;
let num = 0; let numOne = 48; let numTwo = 48; let shift = 6; let line = 0; let level = 0; let toggler = false; let buttonPow = false; let score = 0; //"line" is basically a boolean
let timer = 0; let startTimer = 0;
const nums = [11, 7, 14, 13, 10, 5, 6, 15, 1, 13, 4, 2, 12, 3, 8, 9];
const P1 = [0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x05, 0x05];
const P2 = [0x00, 0x00, 0x00, 0x1F, 0x00, 0x1C, 0x02, 0x09];
const P3 = [0x00, 0x00, 0x00, 0x00, 0x18, 0x04, 0x04, 0x02];
const Q1 = [0x05, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00];
const Q2 = [0x05, 0x19, 0x02, 0x1C, 0x00, 0x00, 0x00, 0x00];
const Q3 = [0x02, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00];
const cap = [0x00, 0x15, 0x0E, 0x1F, 0x0E, 0x15, 0x00, 0x00];
const wineBot = [0x0E, 0x0E, 0x0A, 0x0A, 0x0E, 0x0E, 0x04, 0x04];

// lineBuilder: copies the first 16 bytes of b into a line of text (the LCD stops at a 0 byte)
const x = {
  y(parcel) {
    return String.fromCharCode(...parcel.slice(0, 16));
  },
};
async function DC() {
  await delay(2500);
  lcd.clear();
}

//SOUNDS
async function getGood() {
  tone(buzzer, 415, 90);
  await delay(100);
  tone(buzzer, 466, 90);
  await delay(100);
  tone(buzzer, 349, 90);
  await delay(200);
  tone(buzzer, 466, 180);
}
async function getBad() {
  tone(buzzer, 415, 90);
  await delay(100);
  tone(buzzer, 350, 90);
  await delay(200);
  tone(buzzer, 349, 180);
}
async function spooky() {
  tone(buzzer, 277, 90);
  await delay(150);
  tone(buzzer, 329, 45);
}
async function getBoop() {
  tone(buzzer, 1318, 125);
  await delay(250);
  tone(buzzer, 1174, 100);
  await delay(100);
  tone(buzzer, 1108, 100);
  await delay(100);
  tone(buzzer, 1174, 100);
  await delay(100);
  tone(buzzer, 1318, 100);
  await delay(100);
  tone(buzzer, 1174, 125);
  await delay(300);
  tone(buzzer, 1760, 500);
}
async function getLocation() {
  tone(buzzer, 391, 120);
  await delay(400);
  tone(buzzer, 391, 120);
  await delay(133);
  tone(buzzer, 523, 180);
  await delay(400);
  tone(buzzer, 523, 180);
  await delay(400);
  tone(buzzer, 523, 180);
  await delay(267);
  tone(buzzer, 391, 120);
  await delay(434);
  tone(buzzer, 329, 240);
  await delay(266);
  tone(buzzer, 349, 120);
  await delay(133);
  tone(buzzer, 369, 240);
  await delay(266);
  tone(buzzer, 391, 180);
  await delay(400);
  tone(buzzer, 329, 120);
  await delay(434);
  tone(buzzer, 261, 180);
  await delay(400);
  tone(buzzer, 261, 180);
  await delay(400);
  tone(buzzer, 261, 270);
}
async function funkyTown() {
  tone(buzzer, 261, 368);
  await delay(408);
  tone(buzzer, 261, 91);
  await delay(200);
  tone(buzzer, 261, 91);
  await delay(200);
  tone(buzzer, 261, 183);
  await delay(204);
  tone(buzzer, 329, 368);
  await delay(408);
  tone(buzzer, 329, 91);
  await delay(200);
  tone(buzzer, 329, 91);
  await delay(200);
  tone(buzzer, 391, 91);
  await delay(200);
  tone(buzzer, 391, 91);
  await delay(200);
  tone(buzzer, 391, 183);
  await delay(204);
  tone(buzzer, 659, 275);
  await delay(400);
  tone(buzzer, 587, 91);
  await delay(200);
  tone(buzzer, 523, 736);
}
async function beatGame() {
  tone(buzzer, 233, 182);
  await delay(200);
  tone(buzzer, 261, 91);
  await delay(100);
  tone(buzzer, 277, 91);
  await delay(100);
  tone(buzzer, 349, 34);
  await delay(200);
  tone(buzzer, 293, 34);
  await delay(100);
  tone(buzzer, 349, 37);
  await delay(100);
  tone(buzzer, 293, 34);
  await delay(200);
  tone(buzzer, 293, 34);
  await delay(100);
  tone(buzzer, 293, 34);
  await delay(100);
  tone(buzzer, 293, 34);
  await delay(200);
  tone(buzzer, 233, 48);
  await delay(200);
  tone(buzzer, 233, 273);
  await delay(400);
  tone(buzzer, 293, 34);
  await delay(200);
  tone(buzzer, 293, 34);
  await delay(100);
  tone(buzzer, 293, 34);
  await delay(100);
  tone(buzzer, 293, 34);
  await delay(200);
  tone(buzzer, 293, 34);
  await delay(100);
  tone(buzzer, 293, 34);
  await delay(100);
  tone(buzzer, 293, 34);
  await delay(200);
  tone(buzzer, 261, 48);
  await delay(200);
  tone(buzzer, 261, 273);
  await delay(400);
  tone(buzzer, 261, 34);
  await delay(200);
  tone(buzzer, 261, 34);
  await delay(100);
  tone(buzzer, 261, 34);
  await delay(100);
  tone(buzzer, 261, 34);
  await delay(200);
  tone(buzzer, 261, 34);
  await delay(100);
  tone(buzzer, 261, 34);
  await delay(100);
  tone(buzzer, 261, 34);
  await delay(200);
  tone(buzzer, 261, 90);
  await delay(200);
  tone(buzzer, 261, 182);
  await delay(300);
  tone(buzzer, 261, 90);
  await delay(100);
  tone(buzzer, 349, 34);
  await delay(200);
  tone(buzzer, 349, 34);
  await delay(100);
  tone(buzzer, 349, 34);
  await delay(100);
  tone(buzzer, 349, 34);
  await delay(200);
  tone(buzzer, 311, 34);
  await delay(100);
  tone(buzzer, 311, 34);
  await delay(100);
  tone(buzzer, 311, 34);
  await delay(200);
  tone(buzzer, 349, 48);
  await delay(200);
  tone(buzzer, 349, 273);
}

export async function setup() {
  randomSeed(analogRead(0));
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(debouncer, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(6, OUTPUT); //Brightness control
  for (let x = 7; x <= 9; x++) {
    pinMode(x, INPUT);
  }
  //Welcome
  lcd.setCursor(3, 0); lcd.print("WELCOME TO");
  lcd.setCursor(3, 1); lcd.print("BOOP QUEST");
  analogWrite(6, 80);
  //Fade Up
  for (let x = 0; x <= 80; x++) {
    analogWrite(6, x);
    await delay(50);
  }
  analogWrite(6, 80);
  //Make customs chars
  lcd.createChar(0, P1); lcd.createChar(1, P2); lcd.createChar(2, P3);
  lcd.createChar(3, Q1); lcd.createChar(4, Q2); lcd.createChar(5, Q3);
  lcd.createChar(6, cap);
  lcd.createChar(7, wineBot);
}

export async function loop() {
  //Debouncing
  if (lastPressTime == 0 || (millis() - lastPressTime) > pressDelay) {
    digitalWrite(debouncer, HIGH);
    buttonPow = true;
  }
  let readBut = [0, 0, 0];
  for (let i = 0; i < 3; i++) {
    readBut[i] = digitalRead(i + 7);
  }
  for (let j = 0; j < 3; j++) {
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
  await progressor();
}

async function progressor() {
  switch (progress) {
    default:  await mainMenu();  break;
    case 255: await beaten();    break;
    case 254: await piano();     break;
    case 253: await playPiano(); break;
    case 252: await resetGame(); break;
    case 0:   await mainMenu();  break;
    case 1:   await startGame(); break;
    case 10:  await ca();        break;
    case 11:  await hike();      break;
    case 12:  await peak();      break;
    case 13:  await geocache();  break;
    case 14:  await beach();     break;
    case 15:  await shells();    break;
    case 16:  await sun();       break;
    case 17:  await water();     break;
    case 18:  await coffee();    break;
    case 50:  await fl();        break;
    case 51:  await party();     break;
    case 52:  await study();     break;
    case 53:  await memorize();  break;
    case 90:  await mt();        break;
    case 91:  await playAgain(); break;
  }
}
async function phaseScroller(startCycle, endCycle) {
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
async function mainMenu() {
  youWin = false;
  lcd.setCursor(0, 0); lcd.print("Choose an Option");
  switch (phaser) {
    default:
      phaser = 0;
      break;
    case 0:
      lcd.setCursor(0, 1); lcd.print("< Play da Game >");
      await phaseScroller(0, 4);
      if (but[1] == HIGH) {
        if ((await checkBeat(0)) == 1) {
          progress = 252;
        }
        else {
          lcd.clear();
          lcd.setCursor(0, 0); lcd.print("Where will your "); lcd.setCursor(0, 1); lcd.print("quest begin?");
          await funkyTown();
          await delay(2000);
          progress++;
        }
      }
      break;
    case 1:
      switch (brightness) {
        case 0:
          lcd.setCursor(0, 1); lcd.print("< Light  [OFF] >");
          analogWrite(6, LOW);
          break;
        case 1:
          lcd.setCursor(0, 1); lcd.print("< Light  [LOW] >");
          analogWrite(6, 20);
          break;
        case 2:
          lcd.setCursor(0, 1); lcd.print("< Light  [MED] >");
          analogWrite(6, 80);
          break;
        case 3:
          lcd.setCursor(0, 1); lcd.print("< Light [HIGH] >");
          analogWrite(6, 255);
          break;
      }
      if (but[1] == HIGH) {
        if (brightness == 3) {
          brightness = 0;
        }
        else (brightness++);
      }
      await phaseScroller(0, 4);
      break;
    case 2:
      if (sound == false) {
        lcd.setCursor(0, 1); lcd.print("< Sound  [OFF] >");
        buzzer = 100;
      }
      if (sound == true) {
        lcd.setCursor(0, 1); lcd.print("< Sound   [ON] >");
        buzzer = 10;
      }
      if (line == 1) {
        tone(buzzer, 800, 300);
        await delay(200);
        tone(buzzer, 1000, 150);
        line = 0;
      }
      if (but[1] == HIGH) {
        sound = !sound;
        if (sound == true) {
          line = 1;
        }
      }
      await phaseScroller(0, 4);
      break;
    case 3:
      if ((await checkBeat(0)) == 1) {
        progress = 255;
      }
      else if (but[1] == HIGH) {
        await spooky();
      }
      lcd.setCursor(0, 1); lcd.print("< ** LOCKED ** >");
      await phaseScroller(0, 4);
      break;
    case 4:
      if ((await checkBeat(0)) == 1) {
        progress = 254;
      }
      else if (but[1] == HIGH) {
        await spooky();
      }
      lcd.setCursor(0, 1); lcd.print("< ALSO  LOCKED >");
      await phaseScroller(0, 4);
      break;
  }
}
async function checkBeat(checkWin) {
  if (boop1 == 1 && boop2 == 1 && boop3 == 1 && boop4 == 1 && boop5 == 1 && boop6 == 1 && boop7 == 1 && boop8 == 1) {
    lcd.clear();
    if (checkWin == 1 || checkWin == 2 || checkWin == 3) {
      lcd.setCursor(0, 0); lcd.print("YOU FOUND ALL OF"); lcd.setCursor(0, 1); lcd.print("   THE BOOPS!");
      await getLocation();
      await delay(500);
      lcd.noDisplay();
      await delay(1500);
      lcd.clear();
      lcd.display();
      lcd.setCursor(0, 0); lcd.print("All menu options"); lcd.setCursor(0, 1); lcd.print("now unlocked");
      await getGood();
      await DC();
      youWin = true;
      progress = 0;
    }
    return 1;
  }
  if (checkWin == 1 && boop1 == 1 && boop2 == 1 && boop3 == 1) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("You found all CA"); lcd.setCursor(5, 1); lcd.print("boops!");
    await getLocation();
    await delay(3000);
    progress = 1;
  }
  if (checkWin == 2 && boop4 == 1 && boop5 == 1) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("You found all FL"); lcd.setCursor(5, 1); lcd.print("boops!");
    await getLocation();
    await delay(3000);
    progress = 1;
  }
  if (checkWin == 3 && boop6 == 1 && boop7 == 1 && boop8 == 1) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("You found all MT"); lcd.setCursor(5, 1); lcd.print("boops!");
    await getLocation();
    await delay(3000);
    progress = 1;
  }
  return 0;
}
async function beaten() {
  lcd.setCursor(0, 0); lcd.print("Choose an Option");
  lcd.setCursor(0, 1); lcd.print("<  Beat  Game  >");
  if (but[1] == HIGH) {
    lcd.clear();
    lcd.setCursor(1, 0); lcd.print("SORRY, ARDUINO"); lcd.setCursor(2, 1); lcd.print("PLAYERS ONLY");
    for (let x = 0; x < 3; x++) {
      await beatGame();
    }
  }
  if (but[0] == HIGH) {
    phaser = 2; progress = 0;
  }
  if (but[2] == HIGH) {
    phaser = 4; progress = 0;
  }
}
async function resetGame() {
  lcd.setCursor(1, 0); lcd.print("This will reset");
  lcd.setCursor(0, 1); lcd.print("OK  the game  NO");
  if (but[0] == HIGH) {
    boop1 = 0; boop2 = 0; boop3 = 0; boop4 = 0; boop5 = 0; boop6 = 0; boop7 = 0; boop8 = 0;
    for (let x = 10; x > 0; x--) {
      await grind();
      tone(buzzer, (x * 100) + 100, 100);
      await grind();
    }
    lcd.clear();
    await delay(2000);
    lcd.setCursor(0, 0); lcd.print("Where will your "); lcd.setCursor(0, 1); lcd.print("quest begin?");
    await funkyTown();
    await delay(2000);
    progress = 1;
  }
  if (but[2] == HIGH) {
    progress = 0;
  }
}
async function startGame() {
  lcd.setCursor(1, 0); lcd.print("Select a Place");
  switch (phaser) {
    default:
      phaser = 0;
      break;
    case 0:
      if (boop1 == 1 && boop2 == 1 && boop3 == 1) {
        phaser = 1;
      }
      lcd.setCursor(0, 1); lcd.print("<  California  >");
      await phaseScroller(0, 2);
      if (but[1] == HIGH) {
        progress = 10;
      }
      break;
    case 1:
      if (boop4 == 1 && boop5 == 1) {
        phaser = 2;
      }
      lcd.setCursor(0, 1); lcd.print("<   Florida    >");
      await phaseScroller(0, 2);
      if (but[1] == HIGH) {
        progress = 50;
      }
      break;
    case 2:
      if (boop6 == 1 && boop7 == 1 && boop8 == 1) {
        phaser = 0;
      }
      lcd.setCursor(0, 1); lcd.print("<   Montana    >");
      await phaseScroller(0, 2);
      if (but[1] == HIGH) {
        progress = 90;
      }
      break;
  }
}

async function ca() {
  await checkBeat(1);
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("You wake up in a"); lcd.setCursor(0, 1); lcd.print("queen sized bed");
    await DC();
    lcd.setCursor(0, 0); lcd.print("It looks nice "); lcd.setCursor(0, 1); lcd.print("outside");
    await delay(2500);
    phaser++;
  }
  lcd.setCursor(2, 0); lcd.print("Time for a:");
  switch (phaser) {
    default:
      phaser = 1;
      break;
    case 1:
      if (boop1 == 1) {
        phaser = 2;
      }
      lcd.setCursor(0, 1); lcd.print("<Quick lil hike>");
      await phaseScroller(1, 3);
      if (but[1] == HIGH) {
        progress = 11;
      }
      break;
    case 2:
      if (boop2 == 1) {
        phaser = 3;
      }
      lcd.setCursor(0, 1); lcd.print("<  Beach day   >");
      await phaseScroller(1, 3);
      if (but[1] == HIGH) {
        progress = 14;
      }
      break;
    case 3:
      if (boop3 == 1) {
        phaser = 1;
      }
      lcd.setCursor(0, 1); lcd.print("< Cup o coffee >");
      await phaseScroller(1, 3);
      if (but[1] == HIGH) {
        progress = 18;
      }
      break;
  }
}
async function hike() {
  await delay(10);
  if (sixteen == 100 || sixteen != lastSixteen) {
    lcd.setCursor(0, 0); lcd.print("                "); lcd.setCursor(0, 1); lcd.print("  \xFF    \xFF\xFF\xFF   \xFF  ");
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
    lcd.write(252);
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 1 || sixteen == 6 || sixteen == 12) {
    lcd.setCursor(sixteen, 1);
    lcd.write(252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[1] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 2 || sixteen == 13) {
    lcd.setCursor(sixteen, 0);
    lcd.write(252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 3 || sixteen == 10 || sixteen == 14) {
    lcd.setCursor(sixteen, 1);
    lcd.write(252);
    if (but[1] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 4 || sixteen == 5 || sixteen == 11) {
    lcd.setCursor(sixteen, 1);
    lcd.write(252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 7 || sixteen == 8 || sixteen == 9) {
    lcd.setCursor(sixteen, 0);
    lcd.write(252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen++;
    }
  }
  else if (sixteen == 15) {
    lcd.setCursor(sixteen, 1);
    lcd.write(252);
    if (but[0] == HIGH) {
      sixteen--;
    }
    if (but[2] == HIGH) {
      sixteen = 100;
      progress++;
    }
  }
}
async function peak() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("Whew, good hike!");
    tone(buzzer, 2800, 90);
    await delay(100);
    tone(buzzer, 2600, 90);
    await delay(100);
    tone(buzzer, 3000, 90);
    await delay(200);
    tone(buzzer, 3200, 180);
    await DC();
    lcd.setCursor(0, 0); lcd.print("Ocean view and "); lcd.setCursor(0, 1); lcd.print("geocache at top");
    await delay(2500);
    phaser++;
  }
  lcd.setCursor(2, 0); lcd.print("What to do?");
  switch (phaser) {
    case 1:
      lcd.setCursor(0, 1); lcd.print("< Go back home >"); await phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 10;
      }
      break;
    case 2:
      lcd.setCursor(0, 1); lcd.print("<Open the cache>");
      await phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 13;
      }
      break;
  }
}
async function geocache() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("Handwarmers, "); lcd.setCursor(0, 1); lcd.print("visitor log,");
    await DC();
    lcd.setCursor(0, 0); lcd.print("a squishy ball, "); lcd.setCursor(0, 1); lcd.print("and...");
    await DC();
    lcd.setCursor(0, 0); lcd.print("what's this?!");
    await DC();
    boop1 = 1; //GET BOOP 1
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(1, 0); lcd.print("You found boop");
    lcd.setCursor(5, 1); lcd.print(String.fromCharCode(boopNum) + " of 8");
    boopNum++;
    await getBoop();
    await delay(3500);
    lcd.clear();
    progress = 10;
  }
}
async function beach() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("After a nice "); lcd.setCursor(0, 1); lcd.print("canyon drive");
    await DC();
    lcd.setCursor(0, 0); lcd.print("you arrive at "); lcd.setCursor(0, 1); lcd.print("Point Dume.");
    await delay(2500);
    phaser++;
  }
  lcd.setCursor(1, 0); lcd.print("'Sup freckles?");
  switch (phaser) {
    default:
      phaser = 1;
      break;
    case 1:
      lcd.setCursor(0, 1); lcd.print("<Arrange shells>");
      if (but[1] == HIGH) {
        progress = 15;
      }
      await phaseScroller(1, 3);
      break;
    case 2:
      lcd.setCursor(0, 1); lcd.print("< Enjoy da sun >");
      if (but[1] == HIGH) {
        progress = 16;
      }
      await phaseScroller(1, 3);
      break;
    case 3:
      if (boop2 == 0) {
        lcd.setCursor(0, 1); lcd.print("<Go in da water>");
        if (but[1] == HIGH) {
          progress = 17;
        }
      }
      if (boop2 == 1) {
        lcd.setCursor(0, 1); lcd.print("< Go back home >");
        if (but[1] == HIGH) {
          progress = 10;
        }
      }
      await phaseScroller(1, 3);
      break;
  }
}
async function drawShell(shift) {
  if (phaser != 9) {
    lcd.clear();
  }
  lcd.setCursor(shift, 0);
  lcd.write(0); lcd.write(1); lcd.write(2);
  lcd.setCursor(shift, 1);
  lcd.write(3); lcd.write(4); lcd.write(5);
}
async function drawCap(shift, line) {
  lcd.clear();
  lcd.setCursor(shift, line);
  lcd.write(6);
}
async function playShell() {
  if (shift > 1) {
    if (but[0] == HIGH) {
      shift--;
      await drawShell(shift);
    }
    if (but[1] == HIGH) {
      await drawShell(shift);
    }
    if (but[2] == HIGH && shift < 9) {
      shift++;
      await drawShell(shift);
    }
  }
  else {
    shift = 6;
    numOne++;
    await getGood();
    phaser++;
  }
}
async function playCap() {
  if (shift < 12) {
    if (but[0] == HIGH && shift > 4) {
      shift--;
      await drawCap(shift, line);
    }
    if (but[1] == HIGH) {
      await drawCap(shift, line);
    }
    if (but[2] == HIGH) {
      shift++;
      await drawCap(shift, line);
    }
  }
  else {
    if (line == 1) {
      line = 0;
    }
    else (line = 1); //lollllll s/o to boolean am i rite
    shift = 6;
    numTwo++;
    await getBad();
    phaser++;
  }
}
async function shells() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("Put shells on "); lcd.setCursor(0, 1); lcd.print("the left");
    await delay(2500);
    lcd.setCursor(0, 0); lcd.print("and bottlecaps "); lcd.setCursor(0, 1); lcd.print("on the right.");
    await delay(2500);
    phaser++;
  }
  lcd.setCursor(0, 0); lcd.print(String.fromCharCode(numOne) + "  |"); lcd.setCursor(12, 0); lcd.print("|  " + String.fromCharCode(numTwo)); lcd.setCursor(0, 1); lcd.print("   |"); lcd.setCursor(12, 1); lcd.print("|   ");
  switch (phaser) {
    case 1:
      await playShell();
      break;
    case 2:
      await playCap();
      break;
    case 3:
      await playCap();
      break;
    case 4:
      await playShell();
      break;
    case 5:
      await playShell();
      break;
    case 6:
      await playShell();
      break;
    case 7:
      await playCap();
      break;
    case 8:
      await playShell();
      break;
    case 9:
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Five pretty "); lcd.setCursor(0, 1); lcd.print("shells!");
      await delay(2500);
      lcd.clear();
      for (let x = 0; x <= 12; x += 3) {
        await drawShell(x);
      }
      await delay(4000);
      line = 0;
      progress = 14;
      break;
  }
}
async function sun() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("Apply sunscreen "); lcd.setCursor(0, 1); lcd.print("with left");
    await DC();
    lcd.setCursor(0, 0); lcd.print("Apply tanning "); lcd.setCursor(0, 1); lcd.print("oil with right");
    await DC();
    sixteen = 7;
    startTimer = millis();
    timer = millis();
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(0, 0); lcd.print("pale       burnt");
    lcd.setCursor(sixteen, 1); lcd.write(255); lcd.write(255);
    if ((millis() - timer) > 1000 && sixteen != 14) {
      await spooky();
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
    lcd.setCursor(0, 0); lcd.print("A hunky beach "); lcd.setCursor(0, 1); lcd.print("dood walks by");
    await DC();
    lcd.setCursor(0, 0); lcd.print("He says:");
    if (sixteen < 4) {
      lcd.setCursor(0, 1); lcd.print("\x22Eh, still cute\x22");
    }
    else if (sixteen < 8) {
      lcd.setCursor(0, 1); lcd.print("\x22Lookin good\x22");
    }
    else if (sixteen < 12) {
      lcd.setCursor(0, 1); lcd.print("\x22Nice tan missy\x22");
    }
    else {
      lcd.setCursor(0, 1); lcd.print("\x22Daz gonna hurt\x22");
    }
    await delay(4000);
    sixteen = 100;
    progress = 14;
  }
}
async function water() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("The waves look "); lcd.setCursor(0, 1); lcd.print("pretty rough");
    await DC();
    lcd.setCursor(0, 0); lcd.print("Maybe just get "); lcd.setCursor(0, 1); lcd.print("your feet wet");
    await DC();
    lcd.setCursor(0, 0); lcd.print("It's cold! And "); lcd.setCursor(0, 1); lcd.print("sticky?");
    await DC();
    boop2 = 1; //GET BOOP 2
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(0, 0); lcd.print("Oh, that is boop");
    lcd.setCursor(5, 1); lcd.print(String.fromCharCode(boopNum) + " of 8");
    await getBoop();
    boopNum++;
    await delay(3500);
    lcd.clear();
    progress = 14;
  }
}
async function grind() {
  for (let i = 0; i < 16; i++) {
    b[i] = random(255);
  }
  lcd.setCursor(0, 0);
  lcd.print(x.y(b));
  for (let i = 0; i < 16; i++) {
    b[i] = random(255);
  }
  lcd.setCursor(0, 1);
  lcd.print(x.y(b));
}
async function coffee() {
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("Grind the beans!");
    await delay(2500);
    phaser++;
  }
  if (phaser == 1) {
    lcd.setCursor(0, 0); lcd.print("\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2"); lcd.setCursor(0, 1); lcd.print("\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2\xF2");
    if (but[0] == HIGH) {
      await grind();
      tone(buzzer, 130, 100);
      await delay(100);
      await grind();
      tone(buzzer, 90, 100);
      await delay(100);
      await grind();
      tone(buzzer, 170, 100);
      await delay(100);
      phaser++;
    }
  }
  if (phaser == 2) {
    lcd.setCursor(0, 0); lcd.print("\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0"); lcd.setCursor(0, 1); lcd.print("\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0\xC0");
    if (but[1] == HIGH) {
      await grind();
      tone(buzzer, 160, 100);
      await delay(100);
      await grind();
      tone(buzzer, 100, 100);
      await delay(100);
      await grind();
      tone(buzzer, 180, 100);
      await delay(100);
      phaser++;
    }
  }
  if (phaser == 3) {
    lcd.setCursor(0, 0); lcd.print("\xF3\xF3\xF3\xE2\xF3\xF3\xF3\xF3\xF3\xA1\xF3\xF3\xF3\xF3\xF3\xF3"); lcd.setCursor(0, 1); lcd.print("\xF3\xF3\xF3\xF3\xF3\xE5\xF3\xF3\xF3\xF3\xF3\xF3\xF3\xF3\xB1\xF3");
    await DC();
    lcd.setCursor(0, 0); lcd.print("Do you see that!");
    await DC();
    lcd.setCursor(0, 0); lcd.print("Boop in the "); lcd.setCursor(0, 1); lcd.print("coffee grounds!");
    await delay(2500);
    lcd.setCursor(0, 0); lcd.print("\xF3\xF3\xF3\xE2\xF3\xF3\xF3\xF3\xF3\xA1\xF3\xF3\xF3\xF3\xF3\xF3"); lcd.setCursor(0, 1); lcd.print("\xF3\xF3\xF3\xF3\xF3\xE5\xF3\xF3\xF3\xF3\xF3\xF3\xF3\xF3\xB1\xF3");
    await DC();
    boop3 = 1; //GET BOOP 3
    phaser++;
  }
  if (phaser == 4) {
    lcd.setCursor(0, 0); lcd.print("It is tasty boop");
    lcd.setCursor(5, 1); lcd.print(String.fromCharCode(boopNum) + " of 8");
    boopNum++;
    await getBoop();
    await delay(3500);
    lcd.clear();
    progress = 10;
  }
}

async function fl() {
  await checkBeat(2);
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("It's a sunny day"); lcd.setCursor(0, 1); lcd.print("and you are");
    await DC();
    lcd.setCursor(0, 0); lcd.print("in the grass by "); lcd.setCursor(0, 1); lcd.print("Physics");
    await delay(2500);
    phaser++;
  }
  if (boop4 == 0 && boop5 == 0) {
    lcd.setCursor(1, 0); lcd.print("Do what first?");
  }
  else {
    lcd.setCursor(1, 0); lcd.print("Now you haveta");
  }
  switch (phaser) {
    default:
      phaser = 1;
      break;
    case 1:
      if (boop4 == 1) {
        phaser = 2;
      }
      lcd.setCursor(0, 1); lcd.print("<  Quid Party  >");
      await phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 51;
      }
      break;
    case 2:
      if (boop5 == 1) {
        phaser = 1;
      }
      lcd.setCursor(0, 1); lcd.print("< Study at Lib >");
      await phaseScroller(1, 2);
      if (but[1] == HIGH) {
        progress = 52;
      }
      break;
  }
}
async function party() {
  if (phaser == 0) {
    toggler = false;
    lcd.setCursor(0, 0); lcd.print("You roll up with"); lcd.setCursor(0, 1); lcd.print("a double bottle");
    await delay(2500);
    lcd.setCursor(0, 0); lcd.print("Everyone else is"); lcd.setCursor(0, 1); lcd.print("taking shots...");
    await DC();
    lcd.setCursor(0, 0); lcd.print("...of tequilla");
    await DC();
    lcd.setCursor(0, 0); lcd.print("So you start on "); lcd.setCursor(0, 1); lcd.print("the double");
    await DC();
    lcd.setCursor(0, 0); lcd.print("Pour wine with "); lcd.setCursor(0, 1); lcd.print("center button!");
    await DC();
    num = 0;
    timer = 0;
    phaser++;
  }
  if (phaser == 1) {
    if ((millis() - timer) > 1000) {
      num++;
      if (num == 17) {
        lcd.setCursor(0, 0); lcd.print("                ");
        num = 1;
      }
      lcd.setCursor(0, 0); lcd.print("                ");
      lcd.setCursor(nums[num - 1], 0); lcd.write(7);
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
      let clicked = nums[num - 1];
      if (b[clicked] == 89) {
        await getGood();
        b[clicked] = 92;
        level++;
        lcd.print(x.y(b));
        await delay(1500);
      }
      else {
        await getBad();
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
    lcd.setCursor(0, 0); lcd.print("After 4 glasses "); lcd.setCursor(0, 1); lcd.print("of wine...");
    await DC();
    lcd.setCursor(0, 0); lcd.print("You're feelin "); lcd.setCursor(0, 1); lcd.print("pretty sleepy");
    await DC();
    lcd.setCursor(0, 0); lcd.print("So you head to "); lcd.setCursor(0, 1); lcd.print("bed");
    await DC();
    lcd.setCursor(0, 0); lcd.print("But your bed "); lcd.setCursor(0, 1); lcd.print("isn't there");
    await DC();
    lcd.setCursor(0, 0); lcd.print("Instead...");
    await DC();
    lcd.setCursor(0, 0); lcd.print("It's sleepy boop");
    lcd.setCursor(5, 1); lcd.print(String.fromCharCode(boopNum) + " of 8");
    boopNum++;
    await getBoop();
    await delay(3500);
    lcd.clear();
    progress = 50;
  }
}
async function study() {
  if (sixteen != 100) {
    phaser = 2;
  }
  if (sixteen == 7) {
    phaser = 3;
  }
  switch (phaser) {
    case 0:
      lcd.setCursor(0, 0); lcd.print("blah blah");
      await delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("mitchondria blah"); lcd.setCursor(0, 1); lcd.print("blah");
      await delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Just memorize "); lcd.setCursor(0, 1); lcd.print("the pattern");
      await DC();
      lcd.setCursor(0, 0); lcd.print("and you'll be "); lcd.setCursor(0, 1); lcd.print("fine...");
      await DC();
      phaser++;
      break;
    case 1:
      lcd.setCursor(5, 0); lcd.print("BEGIN!");
      tone(buzzer, 391, 120);
      await delay(133);
      await delay(266);
      tone(buzzer, 391, 120);
      await delay(133);
      tone(buzzer, 523, 180);
      await delay(1000);
      sixteen = 2;
      num = 0;
      phaser++;
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Chapter  ");
      lcd.print(String(level + 1));
      await delay(1500);
      lcd.clear();
      await delay(500);
      await show(sixteen);
      lcd.setCursor(1, 0); lcd.print("Now your turn!");
      await delay(1000);
      lcd.clear();
      progress = 53;
      break;
    case 3:
      level = 0;
      boop5 = 1; //GET BOOP 5
      lcd.setCursor(0, 0); lcd.print("Well that wasn't"); lcd.setCursor(0, 1); lcd.print("so bad");
      await DC();
      lcd.setCursor(0, 0); lcd.print("You start to "); lcd.setCursor(0, 1); lcd.print("pack up");
      await DC();
      lcd.setCursor(0, 0); lcd.print("But your bag is "); lcd.setCursor(0, 1); lcd.print("full of...");
      await DC();
      lcd.setCursor(0, 0); lcd.print("Caffinated  boop");
      lcd.setCursor(5, 1); lcd.print(String.fromCharCode(boopNum) + " of 8");
      boopNum++;
      await getBoop();
      await delay(3500);
      lcd.clear();
      sixteen = 100;
      progress = 50;
  }
}
async function mem(shift) {
  lcd.setCursor(shift, 0); lcd.write(255); lcd.write(255);
  lcd.setCursor(shift, 1); lcd.write(255); lcd.write(255);
  let note; // a byte on the Arduino, so 750 and 1250 wrap to 238 and 226
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
  tone(buzzer, note & 0xFF, 100);
  await delay(500);
  lcd.clear();
}
async function show(level) {
  for (let x = 0; x <= level; x++) {
    if (nums[x] <= 5) {
      await mem(1);
    }
    else if (nums[x] <= 10) {
      await mem(7);
    }
    else if (nums[x] <= 15) {
      await mem(13);
    }
    await delay(1000);
  }
}
async function memorize() {
  if (nums[num] <= 5) {
    if (but[0] == HIGH) {
      await mem(1);
      num++;
    }
    if (but[1] == HIGH || but[2] == HIGH) {
      await getBad();
    }
  }
  else if (nums[num] <= 10) {
    if (but[1] == HIGH) {
      await mem(7);
      num++;
    }
    if (but[0] == HIGH || but[2] == HIGH) {
      await getBad();
    }
  }
  else if (nums[num] <= 15) {
    if (but[2] == HIGH) {
      await mem(13);
      num++;
    }
    if (but[1] == HIGH || but[0] == HIGH) {
      await getBad();
    }
  }
  if (num == (sixteen + 1)) {
    level++;
    num = 0;
    sixteen++;
    progress = 52;
  }
}

async function mt() {
  if (score == 100 && phaser == 0) {
    phaser = 1;
  }
  if (phaser == 0) {
    lcd.setCursor(0, 0); lcd.print("You are floating"); lcd.setCursor(0, 1); lcd.print("around");
    await DC();
    lcd.setCursor(0, 0); lcd.print("in a tube at "); lcd.setCursor(0, 1); lcd.print("Hyalite...");
    await DC();
    lcd.setCursor(0, 0); lcd.print("You see "); lcd.setCursor(0, 1); lcd.print("something shiny");
    await DC();
    lcd.setCursor(0, 0); lcd.print("down below...");
    await DC();
    lcd.setCursor(2, 0); lcd.print("Dive for the "); lcd.setCursor(3, 1); lcd.print("treasure!!");
    await DC();
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
      await getGood();
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
    lcd.print(" hmm "); lcd.print(String(score)); lcd.print(" gold..");
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
      await getBoop();
      await delay(500);
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
      await getBoop();
      await delay(500);
      await getBoop();
      await delay(500);
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
      await getBoop();
      await delay(500);
      await getBoop();
      await delay(500);
      await getBoop();
      await delay(500);
      progress = 91;
    }
  }
}
async function playAgain() {
  await checkBeat(3);
  score = 100;
  lcd.setCursor(0, 0); lcd.print("  Play  again?  ");
  lcd.setCursor(0, 1); lcd.print("  OK        NO  ");
  if (but[0] == HIGH) {
    progress = 90;
  }
  if (but[2] == HIGH) {
    progress = 1;
  }
}

async function piano() {
  lcd.setCursor(0, 0); lcd.print("Choose an Option");
  lcd.setCursor(0, 1); lcd.print("<  Toy  Organ  >");
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
async function playPiano() {
  lcd.setCursor(7, 0);
  if (but[0] == HIGH && but[1] == LOW && but[2] == LOW) {
    lcd.write(0); lcd.write(1); lcd.write(2);
    lcd.setCursor(7, 1);
    lcd.write(3); lcd.write(4); lcd.write(5);
    tone(buzzer, 100);
  }
  if (but[0] == HIGH && but[1] == HIGH && but[2] == LOW) {
    lcd.write(188);
    tone(buzzer, 200);
  }
  if (but[0] == LOW && but[1] == HIGH && but[2] == LOW) {
    lcd.write(6);
    tone(buzzer, 300);
  }
  if (but[0] == LOW && but[1] == HIGH && but[2] == HIGH) {
    lcd.write(239);
    tone(buzzer, 400);
  }
  if (but[0] == LOW && but[1] == LOW && but[2] == HIGH) {
    lcd.write(7);
    tone(buzzer, 500);
  }
  if (but[0] == HIGH && but[1] == LOW && but[2] == HIGH) {
    lcd.write(252);
    tone(buzzer, 250);
  }
  if (but[0] == LOW && but[1] == LOW && but[2] == LOW) {
    lcd.clear();
    noTone(buzzer);
  }
  if (but[0] == HIGH && but[1] == HIGH && but[2] == HIGH) {
    lcd.clear();
    lcd.setCursor(4,0);lcd.print("Exiting");
    lcd.setCursor(5,1);lcd.print("Piano");
    noTone(buzzer);
    progress = 254;
    await delay(3000);
  }
}

// Not part of the sketch: lets the page start with every boop collected (?boops) to test the ending.
export function collectAllBoops() {
  boop1 = 1; boop2 = 1; boop3 = 1; boop4 = 1; boop5 = 1; boop6 = 1; boop7 = 1; boop8 = 1;
  boopNum = 57;
}

// Not part of the sketch: true while the treasure dive is being played. On the device you turn the
// box 90 degrees counterclockwise for it, so the right end of the LCD is the surface.
export function isDiving() {
  return progress == 90 && phaser == 2;
}
