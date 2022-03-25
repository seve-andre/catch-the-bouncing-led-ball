#include <TimerOne.h>
#include "GameConfig.h"
#include "GameCore.h"
#include "StatusGame.h"

//int pos = 0;
//int frequency = 2;
//int difficulty = 1;
//int factor = 100;
//unsigned int S = 2000; //Timer-millis per switch Leds
//unsigned int T1 = 10000; //Timer-millis per pausa ogni 10s
//unsigned int T2 = 3000; //Timer-millis per premere il bottone
//
//boolean resetInterval;
//boolean startTimingPause;


void setup() {
  initGame();
  gameStatus = GAME_INTRO;
}

void loop() {
  switch(gameStatus) {
    case GAME_INTRO:
      gameIntro();
      break;

    case PLAY_GAME:
      playGame();
      break;
  }
}
//
//void restartGame() {
//  isGameOver = true;
//  Serial.print("Game Over. Final Score: ");
//  Serial.println(score);
//  delay(10000);
//
//  digitalWrite(leds[0], LOW);
//  digitalWrite(leds[1], LOW);
//  digitalWrite(leds[2], LOW);
//  digitalWrite(leds[3], LOW);
//  
//  S = 2000; //Timer-millis per switch Leds
//  T1 = 10000; //Timer-millis per pausa ogni 10s
//  T2 = 3000; //Timer-millis per premere il bottone
//
//  pos = 0;
//  positionButtonPressed = 0;
//  buttonToPress = 0;
//  score = 0;
//  digitalWrite(leds[0], HIGH);
//  
//  startTimingPause = false;
//  resetInterval = true;
//  reverse = false;
//  isGameOver = false;
//}
////
//void timeToPressButtonFinished() {
//  Serial.println("FINE PAUSA");
//  Timer1.stop();
//  freezeLeds = false;
//  //Controllo se è stato premuto il bottone giusto o no
//  
//  if (positionButtonPressed == buttonToPress) {
//      score += 100;
//      Serial.print("New point! Score: ");
//      Serial.println(score);
//  }
//
//  //Resettiamo il bottone premuto precedentemente
//  positionButtonPressed = -1;
//  
//  //Ogni volta che viene premuto il pulsante diminuiamo la velocità dei LEDS
//  reduceTimesGame();
//  //Serial.println(S);
//}
//
////vai in pausa dopo 10s che è iniziato il gioco poi ricomincia.
//int timeBeforePause(int timeBefore) {
//  static unsigned long t1, dt; 
//  int ret = 0;
//
//  dt = millis() - t1;
//
//  //scatta l'interrupt
//  if (dt >= timeBefore) {
//      t1 = millis();
//      ret = 1;
//  }
//  
//  return ret;
//}
//
////Timer-millis intervallo tra 1 Led e un altro: parte da 4s
//int timeBetweenLeds(int timerInterval) {
//  static unsigned long t1, dt; 
//  int ret = 0;
//
//  dt = millis() - t1;
//
//  //scatta l'interrupt
//  if (dt >= timerInterval) {
//      t1 = millis();
//      ret = 1;
//  }
//  
//  return ret;
//}
//
//void reduceTimesGame() {
//  if (S > 200) {
//    S -= (factor * difficulty);
//  }
//
//  if (T2 > 50) {
//    T2 -= (factor * difficulty);
//  }
//}
//
//int timerPause(int timerPause) {
//  static unsigned long t1, dt; 
//  int ret = 0;
//
//  if (startTimingPause) {
//    dt = millis() - t1;
//  }
//
//  //scatta l'interrupt
//  if (dt >= timerPause) {
//      t1 = millis();
//      ret = 1;
//  }
//  
//  return ret;
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
