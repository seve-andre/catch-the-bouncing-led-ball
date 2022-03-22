#include <TimerOne.h>
#include <PinChangeInterrupt.h>

int led_0 = 3;
int led_1 = 4;
int led_2 = 5;
int led_3 = 6;

int pos = 0;
int test = 0;
int frequency = 2;
int difficulty = 1;
int factor = 50;
unsigned int S = 4000;

boolean reverse = false;
boolean freezeLeds = false;
boolean generatedLedToPress = false;

//timer locale non bloccante
unsigned long t1 = 0;  // il tempo che scorre
unsigned long dt = 0; // differenza di tempo
  
int leds[4] = {led_0, led_1, led_2, led_3};

void setup() {
  Serial.begin(9600);
  pinMode(led_0, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);

  //Ad inizio partita va inserito questo led a HIGH per far partire
  //il ciclo di shiftLeds
  digitalWrite(leds[0], HIGH);

  //ogni 1s
  //Timer1.initialize(1000000);
  //Timer1.attachInterrupt(shiftLeds);
}

void loop() {  
  //Vengono fermati i LED per permettere la pressione
  //if (myTimer(4000)) {
      //noInterrupts();
      //freezeLeds = true;
      //interrupts();
      //Timer1.stop();
      //Premere il bottone giusto
      //if (!generatedLedToPress) {
        //  buttonToPress = random(3); 
        //  generatedLedToPress = true;
      //}
        
      //Serial.println(buttonToPress);
      delay(30);
  //}

  //Serial.println(positionButtonPressed);
     
  //Pausa di 4s
  if (myTimerStop(S)) {
      //if (positionButtonPressed == buttonToPress) {
       //   Serial.println("BRAVO CONTINUA COSI'");
      //} else {
         // Serial.println("GAME-OVER");
      //}
      //generatedLedToPress = false;

      shiftLeds();
      //Ogni volta che viene premuto il pulsante diminuiamo la velocità dei LEDS
      reduceTimesGame();
      Serial.println(S);
  }
}

void buttonPressed() {
  Serial.println("PREMUTO");
}

void reduceTimesGame() {
  if (S > 50) {
    S -= (factor * difficulty);
  }  
}

int myTimer(int timer1) {
  static unsigned long t1, dt; 
  int ret = 0;
  dt = millis() - t1;

  //scatta l'interrupt
  if (dt >= timer1) {
      t1 = millis();
      ret = 1;
  }
  
  return ret;
}

int myTimerStop(int timerStop) {
  static unsigned long t1, dt; 
  int ret = 0;
  dt = millis() - t1;

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
