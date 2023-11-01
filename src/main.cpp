#include <Arduino.h>
#include <LibRobus.h>
#include <turn_color.hpp>
#include <robotMovement.hpp>
#include "robotServo.hpp"
#include "detection.hpp"

#define PIN_WHISTLE A0
int detect_whistle(void);
void SECTION_4_5() ;
bool active;
int current_zone = 0;
char start_color;

void setup() {
  BoardInit();
  pinMode(PIN_WHISTLE, INPUT);
  active = false;
  initialiser_color();
  servoInit();
  DetectionInit();
}

void loop() {
  Serial.println(Floor_Color());
  //Départ
  if(detect_whistle() == 1 || ROBUS_IsBumper(1)){
    active = true;
  }
  if (active) {
    start_color = Floor_Color();
    move(0.3, 5, false, false) //pour détecter la couleur puis avancer au tape
    //CASE 1
    smoothTurn(0.3, start_color);
    //CASE 2
    move(0.3, 61, false, false);
    //CASE 3
    smoothTurn(0.3, start_color);
    //CASE 4-5
    SECTION_4_5();
    //CASE 6-7-8
      //CODE À AJOUTER
    //CASE 9-0
      //CODE À AJOUTER
    
    //!!! DEUXIÈME TOUR !!!
    //CASE 1
    smoothTurn(0.3, start_color);
    //CASE 2
    move(0.3, 61, false, false);
    //CASE 3
    smoothTurn(0.3, start_color);
    //CASE 4-5-shortcut-0
    move(0.6, 183, true, true);
    sharpTurn(RightTurn);
    move(0.6, 91, true, true);
    sharpTurn(RightTurn);
    move(1, 244, true, true);
    //Fin de la course
  }
}

int detect_whistle(void) {
    int valueIn = analogRead(PIN_WHISTLE);
    return((valueIn >= 400) ? 1 : 0);
}

void SECTION_4_5() {
  int i = 0;
  while (Floor_Color() == start_color){
    if(Floor_Color() == 'V') {
      move(0.3, 5, false, false);
      if(cupGreen() && i<2) {
        sharpTurn(RightTurn);
        sweepCup();
        sharpTurn(LeftTurn);
        i++;
      }
    }  
    else if(Floor_Color() == 'Y') {
      move(0.3, 5, false, false);
      if(cupYellow()&& i<2) {
        sharpTurn(LeftTurn);
        sweepCup();
        sharpTurn(RightTurn);
        i++;
      }
    }
  }
}

void SECTION_9_0() {
    move(1, 200, true, true);
}