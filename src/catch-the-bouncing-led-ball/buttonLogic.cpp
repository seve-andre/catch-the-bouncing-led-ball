//#include <TimerOne.h>
//#include <PinChangeInterrupt.h>
//#include <EnableInterrupt.h>




//int btn_0 = 3;
//int btn_1 = 4;
//int btn_2 = 5;
//int btn_3 = 6;
//
//int positionButtonPressed = 0;
//int buttonToPress = 0;
//
//boolean generatedLedToPress = false;
//boolean startPause = false;
//
//
//void setup() {
//  Serial.begin(9600);
//  
//  pinMode(btn_0, INPUT);
//  pinMode(btn_1, INPUT);
//  pinMode(btn_2, INPUT);
//  pinMode(btn_3, INPUT);
//
//  //Interrupts Buttons
//  enableInterrupt(btn_0, pressedButton, CHANGE);
//  enableInterrupt(btn_1, pressedButton, CHANGE);
//  enableInterrupt(btn_2, pressedButton, CHANGE);
//  enableInterrupt(btn_3, pressedButton, CHANGE);
//
//  Timer1.initialize(5000000);
//  Timer1.attachInterrupt(checkPositionButton);
//}



//void loop() {
//  if (!generatedLedToPress) {
//    Timer1.stop();  
//  } else {
//    Timer1.resume();
//  }
//  
//  //Vengono fermati i LED per permettere la pressione
//  if (pauseLeds(4000)) {
//      if (!generatedLedToPress) {
//        buttonToPress = random(4); 
//        Serial.println("Generato Numero");
//        generatedLedToPress = true;
//        Serial.println(buttonToPress);
//      }
//  }
//  
//    Serial.println(positionButtonPressed);
//    delay(300);
//}


//CAVARE!!!
//int pauseLeds(int timerPause) {
//  static unsigned long t1, dt; 
//  int ret = 0;
//  dt = millis() - t1;
//
//  //scatta l'interrupt
//  if (dt >= timerPause) {
//      t1 = millis();
//      ret = 1;
//      startPause = true;
//  }
//  
//  return ret;
//}
