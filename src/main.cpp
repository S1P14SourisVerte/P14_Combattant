#include <Arduino.h>
#include <LibRobus.h>
#include <turn_color.hpp>
#include <robotMovement.hpp>
#include "robotServo.hpp"
#include "detection.hpp"

#define PIN_WHISTLE A0
int detect_whistle(void);
bool active;
int current_zone = 0;
int flag = 0;

void setup() {
    Serial.begin(9600);
    pinMode(PIN_WHISTLE, INPUT);
    active = false;
    initialiser_color();
    servoInit();
    DetectionInit();
}

void loop() {
  //Départ
  if(detect_whistle() == 1){
    active = true;
  }
  if (active) {
    char start_color = Floor_Color();
    //CASE 1
    smoothTurn(0.3, start_color);
    //CASE 2
    move(0.3, 61, false, false);
    //CASE 3
    smoothTurn(0.3, start_color);
    //CASE 4-5
  }
}

int detect_whistle(void) {
    int valueIn = analogRead(PIN_WHISTLE);
    return((valueIn >= 400) ? 1 : 0);
}


void SECTION_4_5() {
 if(1/*FLOOR IS GREEN*/) {
        move(0.3, 5, false, false);
        if(cupGreen()) {
            /*TURN CLOCKWISE 90 DEGREES*/
            sweepCup();
            /*TURN COUNTER-CLOCKWISE 90 DEGREES*/
        }
    }else if(1/*FLOOR IS YELLOW*/) {
        move(0.3, 5, false, false);
        if(cupYellow()) {
            /*TURN COUNTER-CLOCKWISE 90 DEGREES*/
            sweepCup();
            /*TURN CLOCKWISE 90 DEGREES*/
        }
    }
    flag = 6;
}

void SECTION_9_0() {
    move(1, 200, true, true);

}