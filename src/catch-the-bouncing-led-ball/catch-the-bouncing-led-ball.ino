#include <TimerOne.h>
#include <PinChangeInterrupt.h>
#include <EnableInterrupt.h>

int led_0 = 10;
int led_1 = 11;
int led_2 = 12;
int led_3 = 13;

int btn_0 = 4;
int btn_1 = 5;
int btn_2 = 6;
int btn_3 = 7;

int pos = 0;
int frequency = 2;
int difficulty = 1;
int factor = 100;
unsigned int S = 2000; //Timer-millis per switch Leds
unsigned int T1 = 10000; //Timer-millis per pausa ogni 10s
unsigned int T2 = 3000; //Timer-millis per premere il bottone

int positionButtonPressed = 0;
int buttonToPress = 0;
int score = 0;

boolean resetInterval;
boolean startTimingPause;
boolean reverse = false;
boolean freezeLeds = false;
boolean isGameOver = false;
  
int leds[4] = {led_0, led_1, led_2, led_3};
int buttons[4] = {btn_0, btn_1, btn_2, btn_3};

void setup() {
  Serial.begin(9600);
  pinMode(led_0, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);

  enableInterrupt(btn_0, pressedButton, CHANGE);
  enableInterrupt(btn_1, pressedButton, CHANGE);
  enableInterrupt(btn_2, pressedButton, CHANGE);
  enableInterrupt(btn_3, pressedButton, CHANGE);

  //Ad inizio partita va inserito questo led a HIGH per far partire
  //il ciclo di shiftLeds
  digitalWrite(leds[0], HIGH);
  
  startTimingPause = false;
  resetInterval = true;
}

void loop() {
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

void pauseLed() {
  freezeLeds = true;
  buttonToPress = findActiveLed();
  //memorizziamo il LED che dobbiamo premere e che rimane acceso
  Serial.print("Premi il Tasto: ");
  Serial.println(buttonToPress);
}

int findActiveLed() {
  for (int i = 0; i <= 3; i++) {
        if (digitalRead(leds[i])) {
           return i;
        }
    }
}

void restartGame() {
  isGameOver = true;
  Serial.print("Game Over. Final Score: ");
  Serial.println(score);
  delay(10000);

  digitalWrite(leds[0], LOW);
  digitalWrite(leds[1], LOW);
  digitalWrite(leds[2], LOW);
  digitalWrite(leds[3], LOW);
  
  S = 2000; //Timer-millis per switch Leds
  T1 = 10000; //Timer-millis per pausa ogni 10s
  T2 = 3000; //Timer-millis per premere il bottone

  pos = 0;
  positionButtonPressed = 0;
  buttonToPress = 0;
  score = 0;
  digitalWrite(leds[0], HIGH);
  
  startTimingPause = false;
  resetInterval = true;
  reverse = false;
  isGameOver = false;
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
  } else {
      //restartGame();
  }

  //Resettiamo il bottone premuto precedentemente
  positionButtonPressed = -1;
  
  //Ogni volta che viene premuto il pulsante diminuiamo la velocità dei LEDS
  reduceTimesGame();
  //Serial.println(S);
}

void pressedButton() {  
  if (digitalRead(btn_0)) {
    positionButtonPressed = 0;
  } else if (digitalRead(btn_1)) {
    positionButtonPressed = 1;
  } else if (digitalRead(btn_2)) {
    positionButtonPressed = 2;
  } else if (digitalRead(btn_3)) {
    positionButtonPressed = 3;
  }
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
