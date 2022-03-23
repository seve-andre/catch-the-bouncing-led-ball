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
int test = 0;
int frequency = 2;
int difficulty = 1;
int factor = 50;
unsigned int S = 1000; //Timer-millis per switch Leds

int positionButtonPressed = 0;
int buttonToPress = 0;

boolean resetInterval;
boolean startTimingPause;
boolean reverse = false;
boolean freezeLeds = false;
  
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
  if (timeBetweenLeds(S)) {
    if (!freezeLeds) {
      Serial.println("SWITCH");
      shiftLeds();
    }
  }

  // vai in pausa
  if (timeBeforePause(10000)) {
    startTimingPause = false;
    Serial.println("Vai in PAUSA");
    pauseLed();
    //Timer1 usato come tempo per premere il Bottone
    Timer1.initialize(3000000);
    Timer1.attachInterrupt(timeToPressButtonFinished);
  }
}

void pauseLed() {
  freezeLeds = true;
}

void timeToPressButtonFinished() {
  Serial.println("FINE PAUSA");
  Timer1.stop();
  freezeLeds = false;
  //Controllo se hai premuto il bottone giusto o no
  //....
  //Ogni volta che viene premuto il pulsante diminuiamo la velocità dei LEDS
  //reduceTimesGame();
  //Serial.println(S);
}

void pressedButton() {  
  if (digitalRead(btn_0) == HIGH) {
    positionButtonPressed = 0;
  } else if (digitalRead(btn_1) == HIGH) {
    positionButtonPressed = 1;
  } else if (digitalRead(btn_2) == HIGH) {
    positionButtonPressed = 2;
  } else if (digitalRead(btn_3) == HIGH) {
    positionButtonPressed = 3;
  }
}

//vai in pausa dopo 10s che è iniziato il gioco poi ricomincia.
int timeBeforePause(int timeBefore) {
  static unsigned long t1, dt; 
  int ret = 0;

  //if (resetInterval) {
    dt = millis() - t1;
  //}

  //scatta l'interrupt
  if (dt >= timeBefore) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

void resumeLed() {
  if (positionButtonPressed == buttonToPress) {
      //Serial.println("CONTINUA");
  } else {
      //Serial.println("GAME-OVER");
  }
  
  Serial.println("RESUME");
  //freezeLedss = false;
  //Timer1.stop();
  //startTimingPause = true;
  //generatedLedToPress = false;
}

//Timer-millis intervallo tra 1 Led e un altro: parte da 4s
int timeBetweenLeds(int timerInterval) {
  static unsigned long t1, dt; 
  int ret = 0;

  //if (resetInterval) {
    dt = millis() - t1;
  //}

  //scatta l'interrupt
  if (dt >= timerInterval) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

void buttonPressed() {
  //Serial.println("PREMUTO");
}

void reduceTimesGame() {
  if (S > 50) {
    S -= (factor * difficulty);
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
  //if(!freezeLeds) {
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
  //}
}
