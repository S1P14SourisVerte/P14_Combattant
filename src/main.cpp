#include <Arduino.h>
#include <LibRobus.h>

//>> DEFINE <<//

////////////////

//>> MACROS <<//
#define PRINT(x) Serial.print(x)
#define PRINTLN(x) Serial.println(x)
////////////////

//>> Variables globales <<//
uint16_t leftEncoder = 0;
uint16_t rightEncoder = 0;

int16_t diffEncodersLR = 0;
int16_t diffEncodersRL = 0;

float speed = 0.5;

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

unsigned long previousTime;
int wait = 1000;

////////////////////////////

//>> Fonctions <<//
void move();
///////////////////

void setup(){
  BoardInit();
}

void loop(){
  move();
}

void move(){

  MOTOR_SetSpeed(LEFT, speed);
  MOTOR_SetSpeed(RIGHT,speed);

  leftEncoder = ENCODER_Read(LEFT);
  rightEncoder = ENCODER_Read(RIGHT);

  diffEncodersLR = leftEncoder - rightEncoder;
  diffEncodersRL = rightEncoder - leftEncoder;

  err_leftEncoder = Set_Point - (diffEncodersLR/2);
  err_rightEncoder = Set_Point - (diffEncodersRL/2);

  leftAdjust = Kp * err_leftEncoder;
  rightAdjust = Kp * err_rightEncoder;

  
  if(millis() - previousTime >= wait){
    leftEncoder = ENCODER_ReadReset(LEFT);
    rightEncoder = ENCODER_ReadReset(RIGHT);
    PRINT(" L : ");
    PRINT((String)leftEncoder);
    PRINT(" R : ");
    PRINT((String)rightEncoder);
    PRINT(" diffLR : ");
    PRINT((String)diffEncodersLR);
    PRINT(" diffRL : ");
    PRINT((String)diffEncodersRL);
    PRINT(" L Speed : ");
    PRINT((String)(leftAdjust));
    PRINT(" R Speed : ");
    PRINTLN((String)(rightAdjust));
    previousTime = millis();
  }

  leftSpeed = speed + leftAdjust;
  rightSpeed = speed + rightAdjust;
  
  MOTOR_SetSpeed(LEFT, leftSpeed);
  MOTOR_SetSpeed(RIGHT, rightSpeed);


  

}
