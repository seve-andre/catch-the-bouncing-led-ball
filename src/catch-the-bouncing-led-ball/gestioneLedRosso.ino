#include <TimerOne.h>
#include <PinChangeInterrupt.h>
#include <EnableInterrupt.h>
#include <avr/sleep.h>

#define LED_PIN 3
#define T1 8

int brightness;
int fadeAmount;
int currIntensity;

void setup(){
  Serial.begin(9600);
  currIntensity = 0; 
  fadeAmount = 5; 
  
  pinMode(LED_PIN, OUTPUT);

  enableInterrupt(T1, startGame, CHANGE);

  Timer1.initialize(10000000); //10s
  Timer1.attachInterrupt(deepSleepingMode);
}

void deepSleepingMode() {
  Serial.println("SLEEP MODE");
  analogWrite(LED_PIN, 0); 

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  
}

void startGame() {
  /* First thing to do is disable sleep. */
  sleep_disable();
  Serial.println("starting");
}

void loop(){
  fadeLed();

  if (digitalRead(T1) == HIGH) {
      startGame();
  }
}

void fadeLed() {
  analogWrite(LED_PIN, currIntensity); 
  currIntensity = currIntensity + fadeAmount;
  
  if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount ;
  }
  
  delay(20);
}
