#include <TimerOne.h>
#include <PinChangeInterrupt.h>

int btn_0 = 7;
int btn_1 = 8;
int btn_2 = 9;
int btn_3 = 10;

int positionButtonPressed = 0;
int buttonToPress = 0;

boolean generatedLedToPress = false;
boolean startPause = false;

int buttons[4] = {btn_0, btn_1, btn_2, btn_3};

void setup() {
  Serial.begin(9600);
  
  pinMode(btn_0, INPUT);
  pinMode(btn_1, INPUT);
  pinMode(btn_2, INPUT);
  pinMode(btn_3, INPUT);
}

void loop() {
  //Vengono fermati i LED per permettere la pressione
  if (pauseLeds(4000)) {
      if (!generatedLedToPress) {
        buttonToPress = random(3); 
        generatedLedToPress = true;
        //delay(30);
      }
            
      //Serial.println(buttonToPress);
  }
  
  Serial.println(positionButtonPressed);
  
  if (digitalRead(7) == HIGH) {
    positionButtonPressed = 0;
  } else if(digitalRead(8) == HIGH) {
    positionButtonPressed = 1;
  } else if(digitalRead(9) == HIGH) {
    positionButtonPressed = 2;
  } else if(digitalRead(10) == HIGH) {
    positionButtonPressed = 3;
  }

  //Pausa di 5s per premere il bottone giusto
  if (timeToPress(5000)) {
      if (positionButtonPressed == buttonToPress) {
          //Serial.println("BRAVO CONTINUA COSI'");
      } else {
          //Serial.println("GAME-OVER");
      }
      generatedLedToPress = false;
   }

   delay(300);
}

int pauseLeds(int timerPause) {
  static unsigned long t1, dt; 
  int ret = 0;
  dt = millis() - t1;

  //scatta l'interrupt
  if (dt >= timerPause) {
      //Serial.println("PAUSE");
      t1 = millis();
      ret = 1;
      startPause = true;
  }
  
  return ret;
}

int timeToPress(int tPress) {
  static unsigned long t1, dt; 
  int ret = 0;

  if(startPause) {
    dt = millis() - t1;
  }
  
  //scatta l'interrupt
  if (dt >= tPress) {
      //Serial.println("STOP");
      t1 = millis();
      ret = 1;
      startPause = false;
  }
  
  return ret;
}
