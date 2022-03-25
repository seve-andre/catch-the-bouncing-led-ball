//#include <TimerOne.h>
//
////LEDS
//int led_0 = 3;
//int led_1 = 4;
//int led_2 = 12;
//int led_3 = 13;
//
////BUTTONS
//int btn_0 = 7;
//int btn_1 = 8;
//int btn_2 = 9;
//int btn_3 = 10;
//
//int leds[4] {1, 0 , 0, 0};
//int ledsArduino[4] = {led_0, led_1, led_2, led_3};
//int buttonsArduino[4] = {btn_0, btn_1, btn_2, btn_3};
//
//int pos = 0;
//int ledDaPremere = 0;
//int ledPremuto = 0;
//
//boolean reverse = false;
//boolean freezeLeds = false;
//boolean reset = false;
//boolean isTimePressButton = false;
//
//static unsigned long t0 = 0;
//static unsigned long timeDelay = 5000;
//
//int timer = 0;
//bool state = 0;
//volatile byte count;
//
//void setup() {
//  Serial.print("Start");
//  Serial.begin(9600);
//
//  pinMode(led_0, OUTPUT);
//  pinMode(led_1, OUTPUT);
//  pinMode(led_2, OUTPUT);
//  pinMode(led_3, OUTPUT);
//
//  pinMode(7, INPUT);
//  pinMode(btn_1, INPUT);
//  pinMode(btn_2, INPUT);
//  pinMode(btn_3, INPUT);
//  attachInterrupt(digitalPinToInterrupt(7), btnPressed, CHANGE);
//  
//  digitalWrite(leds[0], HIGH);
//  
//  //Interrupt ogni 4s per Pausa Bottoni
//  //Timer1.initialize(4000000);
//  //Timer1.attachInterrupt(stopLeds);
//}
//
//void aggiornaLedDaPremere() {
//  
//}
//
////void btnPressed() {
//  //Serial.println("PREMUTO!!!");
////}
//
//void loop() {
//  //Timer-0 per intervallo di tempo tra i Led 
//  //shiftLeds();
//  //delay(300);
//
//  
//  Serial.println(digitalRead(7));
//
//  if (pauseButton(3000)) {
//    Serial.println("STOP");
//    reset = false;
//    freezeLeds = true;
//    isTimePressButton = true;
//  }
//  
//  if (resumeButton(5000)) {
//      Serial.println("RESUME");
//      freezeLeds = false;
//      reset = true;
//  } 
//}
//
//int timeToPressButton(int timerPressButton) {
//  static unsigned long tPause, dtPause;
//  int ret = 0;
//
//  if (isTimePressButton) {
//      dtPause = millis() - tPause;
//  }
//  if (dtPause >= timerPressButton) {
//      
//      tPause = millis();
//      ret = 1; 
//  }
//  
//  return ret;
//}
//
//int pauseButton(int timerPause) {
//  static unsigned long tPause, dtPause;
//  int ret = 0;
//
//  dtPause = millis() - tPause;
//  if (dtPause >= timerPause) {
//      tPause = millis();
//      ret = 1;  
//  }
//  
//  return ret;
//}
//
//int resumeButton (int timerResume) {
//  static unsigned long tResume, dtResume;
//  int ret = 0;
//
//  dtResume = millis() - tResume;
//  if (dtResume >= timerResume) {
//      tResume = millis();
//      ret = 1;  
//  }
//  
//  return ret;
//}
//
////Right --> Left
//void shiftLeds() {
//    for (int i = 0; i <= 3; i++) {
//        if (digitalRead(ledsArduino[i]) == HIGH) {
//            if (digitalRead(ledsArduino[3]) == HIGH && !freezeLeds) {
//               reset = true;
//            }
//          
//            if (!freezeLeds) {
//              pos = i;
//              digitalWrite(ledsArduino[pos], LOW);
//              //leds[pos] = 0;
//            }
//        }
//    }
//
//    if (pos < 3) {
//      //leds[pos+1] = 1;
//      digitalWrite(ledsArduino[pos+1], HIGH);
//    } else {
//      if (reset && !freezeLeds) {
//      //Reset del ciclo LEDS
//      digitalWrite(ledsArduino[0], HIGH);
//      digitalWrite(ledsArduino[1], LOW);
//      digitalWrite(ledsArduino[2], LOW);
//      digitalWrite(ledsArduino[3], LOW);
//
//      //leds[0] = 1;
//      //leds[1] = 0;
//      //leds[2] = 0;
//      //leds[3] = 0;
//      reset = false;
//    }
//  }
//
//  for (int i = 0; i <= 3; i++) {   
//    Serial.print(leds[i]);
//  }
//  
//  Serial.println();
//}
//
//void accendiLed(int pos) {
//  leds[pos] = 1;
//}
//
//void spegniLed(int pos) {
//  leds[pos] = 0;
//}
//
//int ultimoLedAcceso() {
//  for (int i = 0; i <= 3; i++) {
//    if (leds[i] == 1) {
//        return i;
//    }
//  }
//}
