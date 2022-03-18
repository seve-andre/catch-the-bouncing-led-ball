//#include "Led.h"
#include "TimerOne.h"
 
int leds[4] = {2, 3, 4, 5};
int difficulty;
int factor;
int pos;
boolean reverse;
boolean freezeLeds;

void setup() {
  freezeLeds = false;
  reverse = false;
  pos = 0;
  Serial.begin(9600);
  // difficulty = analogRead(POT_PIN);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, HIGH);
}

void switchLed() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
    digitalWrite(leds[i], LOW);
  }
}

void Test() {
  int stepper = 1;
  int pos = 0;

    for (int i = 0; i < 4; i += stepper) {

      if (i == 0) {
        stepper = 1;
      }
      
      pos = i;
      if (i == 3) {
        stepper = -1;
      }
      
      if (digitalRead(leds[i]) == HIGH) {
        digitalWrite(leds[i], LOW);
      }
    }

    digitalWrite(leds[pos + stepper], HIGH);
}


void TestRightLeft() {
  int pos = 0;
  for (int i = 0; i < 4; i++) {        
    if (digitalRead(leds[i]) == HIGH) {
        digitalWrite(leds[i], LOW);
        pos = i;
    }
  }

  if (pos < 3) {
    digitalWrite(leds[pos + 1], HIGH); 
  } else {
    digitalWrite(leds[0], HIGH);
  }

}

void atomicTesting() {
  int stepper = 1;

  for (int i = 0; i < 4; i += stepper) {
    if (i == 3) {
      stepper = -1;
      // reverse = true;
    } else if (i == 0) {
      stepper = 1;
    }

    Serial.println(i);
  }
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
      //digitalWrite(leds[pos-1], HIGH);
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

void loop() {
  //switchLed();
  //Timer1.initialize(5000000 / (factor + difficulty));
  Timer1.initialize(2000000);
  Timer1.attachInterrupt(shiftLeds);
}
