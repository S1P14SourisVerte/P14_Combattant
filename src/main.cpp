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

float speed = 0.2;

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
void move(float turnLeft, float turnRight);
///////////////////

void setup(){
  BoardInit();
}

void loop(){
  move(1,1);
}

void move(float turnLeft, float turnRight){

  //if(turnRight != 1 || turnLeft != 1){

    leftEncoder = ENCODER_Read(LEFT);
    rightEncoder = ENCODER_Read(RIGHT);

    leftSpeed = speed * turnLeft; 
    rightSpeed = speed * turnRight;

    float ratioSpeedLR = leftSpeed / rightSpeed;
    float ratioSpeedRL = rightSpeed / leftSpeed;

    float ratioEncoderLR = leftEncoder / rightEncoder;
    float ratioEncoderRL = rightEncoder / leftEncoder;

    diffEncodersLR = ratioSpeedLR - ratioEncoderLR;
    diffEncodersRL = ratioSpeedRL - ratioEncoderRL;

    err_leftEncoder = Set_Point - (diffEncodersLR/2);
    err_rightEncoder = Set_Point - (diffEncodersRL/2);

    leftAdjust = Kp * err_leftEncoder;
    rightAdjust = Kp * err_rightEncoder;

    leftSpeed = (leftSpeed + leftAdjust);
    rightSpeed = (rightSpeed + rightAdjust);
  //}
  // else{

  //   leftEncoder = ENCODER_Read(LEFT);
  //   rightEncoder = ENCODER_Read(RIGHT);

  //   diffEncodersLR = leftEncoder - rightEncoder;
  //   diffEncodersRL = rightEncoder - leftEncoder;
    
  //   err_leftEncoder = Set_Point - (diffEncodersLR/2);
  //   err_rightEncoder = Set_Point - (diffEncodersRL/2);

  //   leftAdjust = Kp * err_leftEncoder;
  //   rightAdjust = Kp * err_rightEncoder;

  //   leftSpeed = (speed + leftAdjust);
  //   rightSpeed = (speed + rightAdjust);
  // }
  

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
    PRINT((String)(leftSpeed));
    PRINT(" R Speed : ");
    PRINTLN((String)(rightSpeed));
    previousTime = millis();
  }

  MOTOR_SetSpeed(LEFT, leftSpeed);
  MOTOR_SetSpeed(RIGHT, rightSpeed);

}
