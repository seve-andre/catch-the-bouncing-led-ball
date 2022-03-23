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
unsigned int S = 4000; //Timer-millis per switch Leds

int positionButtonPressed = 0;
int buttonToPress = 0;

boolean startTimingPause = false;
boolean reverse = false;
boolean freezeLeds = false;
boolean generatedLedToPress = false;
  
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

  //Pausa LED di  3s
  Timer1.initialize(7000000);
  Timer1.attachInterrupt(resumeLed);
  startTimingPause = true;
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

void resumeLed() {
  if (positionButtonPressed == buttonToPress) {
      Serial.println("CONTINUA");
  } else {
      Serial.println("GAME-OVER");
  }
  
  Serial.println("RESUME");
  freezeLeds = false;
  //Timer1.stop();
  startTimingPause = true;
  generatedLedToPress = false;
}

//Timer-millis intervallo tra 1 Led e un altro
int intervalLeds(int timerInterval) {
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

void loop() {  
  if (intervalLeds(S)) {
    Serial.println("SWITCH");
    shiftLeds();
  }

  //Pausa di 4s
//  if (myTimerStop(S)) {
//      if (!generatedLedToPress) {
//          buttonToPress = random(3); 
//          generatedLedToPress = true;
//      }
//        
//      Serial.println(buttonToPress);
//      Serial.println("STOP");
//      startTimingPause = false;
//      freezeLeds = true;
      
      //Ogni volta che viene premuto il pulsante diminuiamo la velocità dei LEDS
      //reduceTimesGame();
      //Serial.println(S);
 // }
}

void buttonPressed() {
  Serial.println("PREMUTO");
}

void reduceTimesGame() {
  if (S > 50) {
    S -= (factor * difficulty);
  }  
}

int myTimerStop(int timerStop) {
  static unsigned long t1, dt; 
  int ret = 0;

  //if (startTimingPause) {
    dt = millis() - t1;
  //}

  //scatta l'interrupt
  if (dt >= timerStop) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

void shiftLeds() {
  if(!freezeLeds) {
  //Right --> Left
  if (!reverse){
    for (int i = 0; i <= 3; i++) {
        if (digitalRead(leds[i]) == HIGH) {
           pos = i;
           digitalWrite(leds[pos], LOW);
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
            pos = j;
            digitalWrite(leds[pos], LOW);
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
}
