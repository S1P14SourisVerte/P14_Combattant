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

uint16_t leftEncoder = 0;
uint16_t rightEncoder = 0;

int16_t diffEncodersLR = 0;
int16_t diffEncodersRL = 0;

float speed = 0.3;

const float Kp = 0.0025;
// const float Ki = 0.01;
// const float Kd = 0.001;
const int Set_Point = 0;

float err_leftEncoder = 0;
float err_rightEncoder = 0;


float leftAdjust = 0;
float rightAdjust = 0;

float leftSpeed = 0;
float rightSpeed = 0;

void moveAdjust(float speedLeftWheel, float speedRightWheel);

void setup() {
  BoardInit();
  pinMode(PIN_WHISTLE, INPUT);
  active = false;
  initialiser_color();
  servoInit();
  DetectionInit();
}

void loop() {
  //Départ
  if((ROBUS_IsBumper(1)) || (detect_whistle())){
    active = true;
  }
  if (active) {
    start_color = Floor_Color();
    Serial.println(start_color);

    move(1.0, 5, false, false); //pour détecter la couleur puis avancer au tape

    //CASE 1
    smoothTurn(0.5, start_color);
    //CASE 2
    if (start_color == 'J')
      move(1.0, 60, false, false);
    else if (start_color == 'V')
      move(1.0, 55, false, false);
    //CASE 3
    smoothTurn(0.5, start_color);
    //CASE 4-5
    SECTION_4_5();
    // move(0.5, 243, false, false);
    Serial.println("FIN SECTION 4-5");
    //CASE 6-7-8
    move(1.0, 8, false, false);
    sharpTurn(RightTurn);
    move (1.0, 155, false, false);
    sharpTurn(RightTurn);

    //CASE 9-0
    move(1.0, 280, true, true);
    
    //!!! DEUXIÈME TOUR !!!
    //CASE 1
    smoothTurn(0.3, start_color);
    //CASE 2
    move(1.0, 61, false, false);
    //CASE 3
    smoothTurn(0.3, start_color);
    //CASE 4-5-shortcut-0
    move(1.0, 183, true, true);
    sharpTurn(RightTurn);
    move(1.0, 91, true, true);
    sharpTurn(RightTurn);
    move(1.0, 244, true, true);
    //Fin de la course
  }
}

int detect_whistle(void) {
    int valueIn = analogRead(PIN_WHISTLE);
    Serial.println(valueIn);
    return((valueIn >= 600) ? 1 : 0);
}

void SECTION_4_5() {
  // int i = 0;
  while (Floor_Color() != 'W'){
    if(Floor_Color() == 'V') {
      //flagZone45 = 1;
      robotMovementSetFlagZone(flagZone45);

      // nonStopMove(0.3, 5, false, false);
      // moveAdjust(1, 1);
      move2(1.0, 5);

      //flagZone45 = 0;
      robotMovementSetFlagZone(flagZone45);
      // nonStopMove(0.3, 5, false, false);
      if(cupGreen() /*&& i<2*/) {
        sharpTurn(RightTurn);
        sweepCup();
        sharpTurn(LeftTurn);
      }
    }  
    else if(Floor_Color() == 'J' || 'R') {

      flagZone45 = 1;
      robotMovementSetFlagZone(flagZone45);

      // moveAdjust(1, 1);
      move2(1.0, 5);

      flagZone45 = 0;
      robotMovementSetFlagZone(flagZone45);
      if(cupYellow() /*&& i<2*/) {
        sharpTurn(LeftTurn);
        sweepCup();
        sharpTurn(RightTurn);
      //   i++;
      }
    }
  }
}

void SECTION_9_0() {
    move(1.0, 200, true, true);
}

void moveAdjust(float speedLeftWheel, float speedRightWheel){

  leftEncoder = ENCODER_Read(LEFT);
  rightEncoder = ENCODER_Read(RIGHT);

  diffEncodersLR = leftEncoder - (rightEncoder * (speedLeftWheel / speedRightWheel));
  diffEncodersRL = rightEncoder - (leftEncoder * (speedRightWheel / speedLeftWheel));

  err_leftEncoder = Set_Point - (diffEncodersLR/2);
  err_rightEncoder = Set_Point - (diffEncodersRL/2);

  leftAdjust = Kp * err_leftEncoder;
  rightAdjust = Kp * err_rightEncoder;

  leftSpeed = (speed + leftAdjust);
  rightSpeed = (speed + rightAdjust);

  MOTOR_SetSpeed(LEFT, leftSpeed);
  MOTOR_SetSpeed(RIGHT, rightSpeed);

}