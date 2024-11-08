/*
// Rafael Mejia, 679124181 rmeji3
// Lab 8
// Description - Write an Arduino program using Interrupt Service Routines (ISRs) to toggle between two states on a 16x2 
  LCD display triggered by two buttons. In State 0, the display shows "System on for X seconds" with time updating continuously, 
  while pressing Button 1 switches to State 1, displaying "Interrupt received!" and a scrolling message. 
  Pressing Button 2 in State 1 resets the timer to 0 and switches back to State 0, but pressing it in State 0 does nothing.
// References - https://docs.arduino.cc/learn/electronics/lcd-displays/
 	https://www.arduino.cc/reference/tr/language/functions/external-interrupts/attachinterrupt/
 
 */
#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 7, 6);


unsigned long previousMillis = 0;  

const long secondsInterval = 1000;  

const int stateZeroBtnPin = 2;
const int stateOneBtnPin = 3;
char bottomText[30];
char topText[21];

int shouldClear = 0;

int topScrollIndex = 0;
int bottomScrollIndex = 0;

unsigned long btnTime = 0;
int state = 0;
int scrollIndex = 0;
void setup()
{
  lcd.begin(16, 2);
  pinMode(stateZeroBtnPin, INPUT_PULLUP);
  pinMode(stateOneBtnPin, INPUT_PULLUP);
  
  
  
}

void loop()
{
  unsigned long currentMillis = millis();

  if(shouldClear){
    lcd.clear();
    shouldClear = 0;
    
  }
  attachInterrupt(digitalPinToInterrupt(stateZeroBtnPin), changeStateZero, CHANGE);
  attachInterrupt(digitalPinToInterrupt(stateOneBtnPin), changeStateOne, CHANGE);
  if(state == 0){
    lcd.setCursor(0,0);
    lcd.print("System on for:");

    lcd.setCursor(0,1);
    lcd.print((millis() - btnTime)/1000);

   
  }else if(state == 1){ 
      lcd.setCursor(0, 0);  
      sprintf(topText, "Interrupt received! ");
      sprintf(bottomText, "Press button 2 to continue ");
      
      unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 100) { 
        previousMillis = currentMillis;
        lcd.setCursor(0, 0);
        for (int i = 0; i < 16; i++) {
            lcd.print(topText[(topScrollIndex + i) % strlen(topText)]);
        }
        topScrollIndex = (topScrollIndex + 1) % strlen(topText);

        lcd.setCursor(0, 1);
        for (int i = 0; i < 16; i++) {
            lcd.print(bottomText[(bottomScrollIndex + i) % strlen(bottomText)]);
        }
        bottomScrollIndex = (bottomScrollIndex + 1) % strlen(bottomText);
   }
  }
  
}
void changeStateZero(){
  if(state != 0){
      state = 0;
      shouldClear = 1;
    btnTime = millis();
    
  }
}
void changeStateOne(){
  if(state != 1){
    state = 1;
    shouldClear = 1;
  }
}

