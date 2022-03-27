#include "Arduino.h"
#include <TimerOne.h>
#include <EnableInterrupt.h>
#include "GameConfig.h"
#include "GameCore.h"
#include <avr/sleep.h>

int gameStatus;
int frequency = 2;
int difficulty = 1;
int factor = 100;
unsigned int S = 2000; //Timer-millis per switch Leds
unsigned int T1 = 10000; //Timer-millis per pausa ogni 10s
unsigned int T2 = 3000; //Timer-millis per premere il bottone

int positionButtonPressed = 0;
int buttonToPress = 0;
int score = 0;
int fadeAmount = 5;
int currIntensity = 0;
int pos = 0;
//int conteggio = 0;
//int typePression = 0;

boolean resetInterval = true;
boolean startTimingPause = false;
boolean generatedLedToPress;

boolean reverse = false;
boolean freezeLeds = false;
boolean isGameOver = false;
boolean isSleepModeOn = false;
boolean firstTimeGameCore = true;
boolean statoDiMezzo = false;

//debounce
int  cont = 0;
int conteggio = 0;

const int ON = HIGH;
const int OFF = LOW;

long tReleaseButton;
long tPressionButton;
long t;
long debouncingDelay = 200;

//const int ON = HIGH;
//const int OFF = LOW;

//long tReleaseButton;
//long tPressionButton;

int leds[N_GREEN_LEDS] = {LED_0, LED_1, LED_2, LED_3}; 
int buttons[N_BUTTONS] = {BTN_0, BTN_1, BTN_2, BTN_3};

void disableSleepMode() {
    Serial.println("Disable Sleep Mode");
    /* First thing to do is disable sleep. */
    sleep_disable();
    statoDiMezzo = false;
    isSleepModeOn = false;
}

void debounce() {
  int tasto = digitalRead(BTN_0);
  //gameStatus = 0;

  if (tasto) {
    if ((millis() - t) > debouncingDelay) {
        Serial.println("ok");
        cont++;
    } 
  } else {
      if (cont > 5 && !isSleepModeOn && statoDiMezzo) {
        gameStatus = 2;
      }
      
      if (cont <= 5 && cont > 0 && isSleepModeOn) {
        gameStatus = 1;
        disableSleepMode();
      }

      Serial.println(gameStatus);
  }
  
  delay(200);
}

void startOrSleep() {
  debounce();

  delay(1000);
  
  //if (isSleepModeOn) {
    //statoDiMezzo = false;
    //gameStatus = 1;
  //} else if(!isSleepModeOn && statoDiMezzo) {
  //  statoDiMezzo = false;
  //  Serial.println("starting");
  //  gameStatus = 2;
  //}
}


void setUpGameIntro() {
  conteggio = OFF;

  enableInterrupt(BTN_0, startOrSleep, CHANGE);
  //pinMode(BTN_0, INPUT);
  enableInterrupt(BTN_1, disableSleepMode, RISING);
  enableInterrupt(BTN_2, disableSleepMode, RISING);
  enableInterrupt(BTN_3, disableSleepMode, RISING);
  
  pinMode(RED_LED, OUTPUT);

  isSleepModeOn = false;
}

void fadeLed() {
  analogWrite(RED_LED, currIntensity); 
  currIntensity = currIntensity + fadeAmount;
  
  if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount ;
  }
  
  delay(20);
}

//vai in SLEEP-MODE dopo 10s
int timeBeforeSleep(int timeBefore) {
  static unsigned long t1, dt; 
  int ret = 0;

  dt = millis() - t1;

  //scatta l'interrupt
  if (dt >= timeBefore) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

void deepSleepingMode() {
  isSleepModeOn = true;
  Serial.println("SLEEP MODE");
  analogWrite(RED_LED, LOW); 

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

void gameIntro() {
  fadeLed();
  statoDiMezzo = true;
  
  if (timeBeforeSleep(10000)) {
     statoDiMezzo = false;
     deepSleepingMode();
  }
}

//------------------------------
void setUpPlayGame() {
    analogWrite(RED_LED, LOW); 
    //Ad inizio partita va inserito questo led a HIGH per far partire il ciclo di shiftLeds
    if (firstTimeGameCore) {
      digitalWrite(leds[0], HIGH);
      firstTimeGameCore = false;
      
      for (int i = 0; i < N_GREEN_LEDS; i++) {
        pinMode(leds[i], OUTPUT);
        enableInterrupt(buttons[i], pressedButton, CHANGE);
      }
    }
}

void shiftLeds() {
  //Right --> Left
  if (!reverse){
    for (int i = 0; i <= 3; i++) {
        if (digitalRead(leds[i]) == HIGH) {
           if (!freezeLeds) {
              pos = i;
              digitalWrite(leds[pos], LOW);
           }
        }
    }

    if (pos < 3) {
      digitalWrite(leds[pos+1], HIGH);
    } else {
      reverse = true;
    }
  }

  if (reverse) {
    //Left --> Right
    for (int j = 3; j >= 0; j--) {
        if (digitalRead(leds[j]) == HIGH) {
            if (!freezeLeds) {
                pos = j;
                digitalWrite(leds[pos], LOW);
            }
        }
    }

    if (pos > 0) {
        digitalWrite(leds[pos-1], HIGH);
        } else {
        digitalWrite(leds[pos+1], HIGH);
        reverse = false;
      }
    }
}


int findActiveLed() {
  for (int i = 0; i <= 3; i++) {
        if (digitalRead(leds[i])) {
           return i;
        }
    }
}

void pauseLed() {
  freezeLeds = true;
  buttonToPress = findActiveLed();
  //memorizziamo il LED che dobbiamo premere e che rimane acceso
  Serial.print("Premi il Tasto: ");
  Serial.println(buttonToPress);
}

void ledInGame() {
  if (!isGameOver) {
    if (timeBetweenLeds(S)) {
      if (!freezeLeds) {
        Serial.println("SWITCH");
        shiftLeds();
      }
    }

    // vai in pausa
    if (timeBeforePause(T1)) {
      startTimingPause = false;
      Serial.println("Vai in PAUSA");
      pauseLed();
      //Timer1 usato come tempo per premere il Bottone
      //Timer1.initialize(3000000);
      //Timer1.attachInterrupt(timeToPressButtonFinished);

      delay(T2);
      timeToPressButtonFinished();
    }
  }
}
 
void playGame() {
   setUpPlayGame();
   ledInGame();
}

//#define LED_PIN 3
//#define BTN_0 4
//#define BTN_1 5
//#define BTN_2 6
//#define BTN_3 7
//

//DA INSERIRE IN FILE .ino
////void setup() {
////  Serial.begin(9600);
////  pinMode(led_0, OUTPUT);
////  pinMode(led_1, OUTPUT);
////  pinMode(led_2, OUTPUT);
////  pinMode(led_3, OUTPUT);
////
////  enableInterrupt(btn_0, pressedButton, CHANGE);
////  enableInterrupt(btn_1, pressedButton, CHANGE);
////  enableInterrupt(btn_2, pressedButton, CHANGE);
////  enableInterrupt(btn_3, pressedButton, CHANGE);
////}

void initLeds() {
  pos = 0;
  
  digitalWrite(leds[0], LOW);
  digitalWrite(leds[1], LOW);
  digitalWrite(leds[2], LOW);
  digitalWrite(leds[3], LOW);
}

void restartGame() {
  isGameOver = true;
  Serial.print("Game Over. Final Score: ");
  Serial.println(score);
  delay(10000);

  initLeds();
  gameStatus = 1;
}

void timeToPressButtonFinished() {
  Serial.println("FINE PAUSA");
  Timer1.stop();
  freezeLeds = false;
  //Controllo se è stato premuto il bottone giusto o no
  
  if (positionButtonPressed == buttonToPress) {
      score += 100;
      Serial.print("New point! Score: ");
      Serial.println(score);
  }

  //Resettiamo il bottone premuto precedentemente
  positionButtonPressed = -1;
  
  //Ogni volta che viene premuto il pulsante diminuiamo la velocità dei LEDS
  reduceTimesGame();
  //Serial.println(S);
}

//vai in pausa dopo 10s che è iniziato il gioco poi ricomincia.
int timeBeforePause(int timeBefore) {
  static unsigned long t1, dt; 
  int ret = 0;

  dt = millis() - t1;

  //scatta l'interrupt
  if (dt >= timeBefore) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

//Timer-millis intervallo tra 1 Led e un altro: parte da 4s
int timeBetweenLeds(int timerInterval) {
  static unsigned long t1, dt; 
  int ret = 0;

  dt = millis() - t1;

  //scatta l'interrupt
  if (dt >= timerInterval) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

void reduceTimesGame() {
  if (S > 200) {
    S -= (factor * difficulty);
  }

  if (T2 > 50) {
    T2 -= (factor * difficulty);
  }
}

int timerPause(int timerPause) {
  static unsigned long t1, dt; 
  int ret = 0;

  if (startTimingPause) {
    dt = millis() - t1;
  }

  //scatta l'interrupt
  if (dt >= timerPause) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

//////////////////////////////////////
void initPins() {
   for (int i = 0; i < N_GREEN_LEDS; i++) {
        pinMode(leds[i], OUTPUT);
        enableInterrupt(buttons[i], pressedButton, CHANGE);
   }
}

void initSerial() {
  Serial.begin(9600); 
}

void initGame() {
  initSerial();
  initPins();
}

//SEZIONE Bottoni

void pressedButton() {
  if (digitalRead(BTN_0)) {
    positionButtonPressed = 0;
  } else if (digitalRead(BTN_1)) {
    positionButtonPressed = 1;
  } else if (digitalRead(BTN_2)) {
    positionButtonPressed = 2;
  } else if (digitalRead(BTN_3)) {
    positionButtonPressed = 3;
  }
}

void checkPositionButton() {
   if (positionButtonPressed == buttonToPress) {
          Serial.println("BRAVO CONTINUA COSI'");
      } else {
          Serial.println("GAME-OVER");
          restartGame();
      }
    generatedLedToPress = false;
}
