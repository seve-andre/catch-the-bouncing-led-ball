#define N_GREEN_LEDS 4
#define N_BUTTONS 4

#define LED_0 10
#define LED_1 11
#define LED_2 12
#define LED_3 13

#define BTN_0 4
#define BTN_1 5
#define BTN_2 6
#define BTN_3 7

#define RED_LED 3
#define POT_PIN A0

#include <EnableInterrupt.h>
#include <avr/sleep.h>

const int ON = HIGH;
const int OFF = LOW;
unsigned int S = 2000; //Timer-millis per switch Leds
unsigned int T1 = 10000; //Timer-millis per pausa ogni 10s
unsigned int T2 = 3000; //Timer-millis per premere il bottone

long tPressed;
long dtPressed;
long debouncingDelay = 200;
int cont = 0;
int conteggio = OFF;
int gameStatus;
int frequency = 2;
int difficulty = 1;
int factor = 250;
int buttonToPress = 0;
int score = 0;

int fadeAmount = 5;
int currIntensity = 0;
int positionButtonPressed = 0;
int pos = 0;

unsigned long tBeforeSleep;
unsigned long dtBeforeSleep;
unsigned long t1, dt; 
unsigned long tSwitch;
unsigned long dtSwitch;

boolean isSleepModeOn;
boolean firstTimeGameCore = true;
boolean isGameOver = false;
boolean freezeLeds = false;
boolean startTimingPause = false;
boolean reverse = false;
boolean isSetUpGameInit = false;
boolean isSetUpGameIntro = false;
boolean startTheGame = false;

int leds[N_GREEN_LEDS] = {LED_0, LED_1, LED_2, LED_3}; 
int buttons[N_BUTTONS] = {BTN_0, BTN_1, BTN_2, BTN_3};
 
void setup() {
  Serial.begin(9600);
  
  Serial.println("Welcome to the Catch the Bouncing Led Ball Game. Press Key T1 to Start");
  
  enableInterrupt(BTN_0, startGame, CHANGE);
  enableInterrupt(BTN_1, disableSleepMode, RISING);
  enableInterrupt(BTN_2, disableSleepMode, RISING);
  enableInterrupt(BTN_3, disableSleepMode, RISING);

  difficulty = 1;
  conteggio = OFF;
  isSleepModeOn = false;
  gameStatus = 1;
  isSetUpGameInit = false;
  isSetUpGameIntro = false;
}

void loop() {
  switch(gameStatus) {
    case 1:
      setUpGameIntro();
      gameIntro();
      break;

    case 2:
      playGame();
      break;
  }
}

void setUpGameIntro() {
  if (!isSetUpGameIntro) {
    delay(200);
    enableInterrupt(BTN_0, startGame, RISING);
    isSetUpGameIntro = true;
  }
}

void gameIntro() {
    fadeLed();
    
    if (timeBeforeSleep(10000)) {
       deepSleepingMode();
    }
}

void playGame() {
  setUpPlayGame();
  initPins();
  ledInGame();
}

void setUpPlayGame() {
    if (!isSetUpGameInit) {
    Serial.println("GO!");
    analogWrite(RED_LED, LOW); 
    //Ad inizio partita va inserito questo led a HIGH per far partire il ciclo di shiftLeds
    if (firstTimeGameCore) {
      digitalWrite(leds[0], HIGH);
      firstTimeGameCore = false;
      difficulty = map(analogRead(POT_PIN),0, 1023, 1, 8);
      
      for (int i = 0; i < N_GREEN_LEDS; i++) {
        pinMode(leds[i], OUTPUT);
        enableInterrupt(buttons[i], pressedButton, RISING);
      }
    }

    S = 2000;   //Timer-millis per switch Leds
    T1 = 10000; //Timer-millis per pausa ogni 10s
    T2 = 3000;  //Timer-millis finestra temporale per premere bottone giusto

    tSwitch = millis();
    isSetUpGameInit = true;
    startTheGame = true;
    }
}

void initPins() {
   disableInterrupt(BTN_0);
   
   for (int i = 0; i < N_GREEN_LEDS; i++) {
        pinMode(leds[i], OUTPUT);
        enableInterrupt(buttons[i], pressedButton, CHANGE);
   }
}

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

void ledInGame() {
  if (!isGameOver) {
    if (timeBetweenLeds(S)) {
      if (!freezeLeds) {
        shiftLeds();
      }
    }

    // vai in pausa
    if (timeBeforePause(T1)) {
      startTimingPause = false;
      pauseLed();

      delay(T2);
      timeToPressButtonFinished();
    }
  }
}

void timeToPressButtonFinished() {
  freezeLeds = false;
  
  //Controllo se è stato premuto il bottone giusto o no.
  if (positionButtonPressed == buttonToPress) {
      score += 100;
      Serial.print("New point! Score: ");
      Serial.println(score);
  } else {
    restartGame();
  }

  //Resettiamo il bottone premuto precedentemente
  positionButtonPressed = -1;
  
  //Ogni volta che viene premuto il pulsante diminuiamo la velocità dei LEDS
  reduceTimesGame();
}

void reduceTimesGame() {
  unsigned int resultS;
  unsigned int resultT2;

  resultS = S - (factor * difficulty);
  if (resultS > 200 && resultS <= 2000) {
    S -= (factor * difficulty);
  } else {
    S = 50;
  }

  resultT2 = T2 - (factor * difficulty);
  if (resultT2 > 50 && resultT2 <= 3000) {
    T2 -= (factor * difficulty);
  } else {
    T2 = 50;
  }
}

void initLeds() {
  pos = 0;
  
  digitalWrite(leds[0], LOW);
  digitalWrite(leds[1], LOW);
  digitalWrite(leds[2], LOW);
  digitalWrite(leds[3], LOW);
}

void restartGame() {
  initLeds();
  isGameOver = true;
  
  Serial.print("Game Over. Final Score: ");
  Serial.println(score);
  delay(10000);
  isGameOver = false;
  isSetUpGameInit = false;
  isSetUpGameIntro = false;
  startTheGame = false;
  
  score = 0;
  gameStatus = 1;
  currIntensity = 0;
  fadeAmount = 5;
  
  tBeforeSleep = millis();
  t1 = millis();
}

void pauseLed() {
  freezeLeds = true;
  //memorizziamo il LED che dobbiamo premere e che rimane acceso
  buttonToPress = findActiveLed();
}

int findActiveLed() {
  for (int i = 0; i <= 3; i++) {
        if (digitalRead(leds[i])) {
           return i;
        }
    }
}

//vai in pausa dopo 10s che è iniziato il gioco poi ricomincia.
int timeBeforePause(int timeBefore) {
  int ret = 0;

  dtSwitch = millis() - tSwitch;

  //scatta l'interrupt
  if (dtSwitch >= timeBefore) {
      tSwitch = millis();
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

void middleGap() {
  delay(200);
  enableInterrupt(BTN_0, startGame, RISING);
}

void deepSleepingMode() {
  enableInterrupt(BTN_0, disableSleepMode, RISING);
  isSleepModeOn = true;
  analogWrite(RED_LED, LOW);
  //Se delay < 50 stampa 2 volte il messaggio iniziale e NON stampa SLEEP MODE ON
  delay(50);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

//vai in SLEEP-MODE dopo 10s
int timeBeforeSleep(int timer) {
  int ret = 0;

  dtBeforeSleep = millis() - tBeforeSleep;

  //scatta l'interrupt
  if (dtBeforeSleep >= timer) {
      tBeforeSleep = millis();
      ret = 1;
  }
  
  return ret;
}

void fadeLed() {  
  analogWrite(RED_LED, currIntensity); 
  currIntensity = currIntensity + fadeAmount;
  
  if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount;
  }
  
  delay(20);
}

void startGame() {
  delay(1000);
  //if (!startTheGame) {
    //disableInterrupt(BTN_0);
  //}
  
  if (digitalRead(BTN_0) == HIGH) {
     gameStatus = 2;
  }
}

void disableSleepMode() {
    isSleepModeOn = false;
    gameIntro();
    middleGap();
}
