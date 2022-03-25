//#include "GameConfig.h"
//#include "GameCore.h"
//#include "LedLogic.h"
//#include "Arduino.h"
//#include <EnableInterrupt.h>
//
//boolean reverse = false;
//boolean freezeLeds = false;
//boolean generatedLedToPress;
//
//int positionButtonPressed = 0;
//int buttonToPress = 0;
//int pos = 0;
//
//int leds[N_GREEN_LEDS] = {LED_0, LED_1, LED_2, LED_3};
//int buttons[N_BUTTONS] = {BTN_0, BTN_1, BTN_2, BTN_3};
//
//void initPins() {
//   for (int i = 0; i < N_GREEN_LEDS; i++) {
//        pinMode(leds[i], OUTPUT);
//        enableInterrupt(buttons[i], pressedButton, CHANGE);
//   }
//}
//
//void initSerial() {
//  Serial.begin(9600); 
//}
//
//void initGame() {
//  initSerial();
//  initPins();
//}
//
//void initLeds() {
//  pos = 0;
//  
//  digitalWrite(leds[0], LOW);
//  digitalWrite(leds[1], LOW);
//  digitalWrite(leds[2], LOW);
//  digitalWrite(leds[3], LOW);
//}
//
//void pauseLed() {
//  freezeLeds = true;
//  buttonToPress = findActiveLed();
//  //memorizziamo il LED che dobbiamo premere e che rimane acceso
//  Serial.print("Premi il Tasto: ");
//  Serial.println(buttonToPress);
//}
//
//int findActiveLed() {
//  for (int i = 0; i <= 3; i++) {
//        if (digitalRead(leds[i])) {
//           return i;
//        }
//    }
//}
//
//void shiftLeds() {
//  //Right --> Left
//  if (!reverse){
//    for (int i = 0; i <= 3; i++) {
//        if (digitalRead(leds[i]) == HIGH) {
//           if (!freezeLeds) {
//              pos = i;
//              digitalWrite(leds[pos], LOW);
//           }
//        }
//    }
//
//    if (pos < 3) {
//      digitalWrite(leds[pos+1], HIGH);
//    } else {
//      reverse = true;
//    }
//  }
//
//  if (reverse) {
//    //Left --> Right
//    for (int j = 3; j >= 0; j--) {
//        if (digitalRead(leds[j]) == HIGH) {
//            if (!freezeLeds) {
//                pos = j;
//                digitalWrite(leds[pos], LOW);
//            }
//        }
//    }
//
//    if (pos > 0) {
//        digitalWrite(leds[pos-1], HIGH);
//        } else {
//        digitalWrite(leds[pos+1], HIGH);
//        reverse = false;
//      }
//    }
//}
//
////SEZIONE Bottoni
//
//void pressedButton() {
//  if (digitalRead(BTN_0)) {
//    positionButtonPressed = 0;
//  } else if (digitalRead(BTN_1)) {
//    positionButtonPressed = 1;
//  } else if (digitalRead(BTN_2)) {
//    positionButtonPressed = 2;
//  } else if (digitalRead(BTN_3)) {
//    positionButtonPressed = 3;
//  }
//}
//
//void checkPositionButton() {
//   if (positionButtonPressed == buttonToPress) {
//          Serial.println("BRAVO CONTINUA COSI'");
//      } else {
//          Serial.println("GAME-OVER");
//          restartGame();
//      }
//    generatedLedToPress = false;
//}
