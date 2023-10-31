#include <Arduino.h>
#include <math.h>

#include "robotMovement.hpp"

//#define DEBUG

//>> Define <<//
#define PIN_TOF A7
#define DATA_TIMEOUT 1000 // https://learn.digilentinc.com/Documents/407#:~:text=The%20main%20reason%20to%20use,passed%2C%20and%20then%20they%20reset.
////////////////

//>> Macros <<//
#define PRINT(x) Serial.print(x);
#define PRINTLN(x) Serial.println(x);
#define PRINT_COMMA Serial.print(',')
#define PRINT_BLACKSLASH Serial.print('\n')
#define WAIT(x) delay(x);
////////////////
//>> Variables globales <<// moveAdjust()
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

unsigned long lastTime2;
int wait = 1000;

//distanceTOF_mm()

unsigned int distance = 0;
unsigned int correctionFactor = 17158;
float exponent_Factor = -0.797;

unsigned int avalogValue = 0;
unsigned long cmptData = 0;

static int last_time = 0;

// deltaDistance()
int previousWallDistance = 0;

long previousDeltaTime = 0;


//

void terminalDisplay(int cmpt, int value1, int value2);
int distanceTOF_mm();
void moveAdjust(float speedLeftWheel, float speedRightWheel);
float deltaDistance(int currentWallDistance);
int detectEndWall(int deltaDistance);

void setup() {
  
  BoardInit();
  pinMode(PIN_TOF, INPUT);

  


  
}

void loop() {


  //terminalDisplay(cmptData, avalogValue, distance);
  distance = distanceTOF_mm();

  //PRINTLN((String)distance);
  PRINTLN((String)deltaDistance(distance));
  int flag = detectEndWall(distance);

  if(flag){
    stop();
  }
  else{
    moveAdjust(1,1);
  }

  deltaDistance(distance);
  //turnSmooth(0.3);

  //moveAdjust(1,1);


}

void terminalDisplay(int cmpt, int value1, int value2){

      PRINT((String) cmpt);
      PRINT_COMMA;
      PRINT((String) value1);
      PRINT_COMMA;
      PRINT((String) value2);
      PRINT_BLACKSLASH;


}

int detectEndWall(int deltaDistance){
  if(deltaDistance > 2000){
    return 1;
  }
  else{
    return 0;
  }
}

float deltaDistance(int currentWallDistance){

  long currentDeltaTime = millis();

  if(currentDeltaTime - previousDeltaTime >= 1000){

    float delta = 0;
    

    delta = (currentWallDistance - previousWallDistance);
    
    previousDeltaTime = currentDeltaTime;
    previousWallDistance = currentWallDistance;

    //PRINTLN((String)delta);
    return delta;
  }
}

int distanceTOF_mm(){

  unsigned int distanceTOF_mm = 0;

 
  int current_time = millis();

  //if(current_time - last_time >= 1000){
    avalogValue = analogRead(PIN_TOF);
    distanceTOF_mm = (float) correctionFactor * pow((float) avalogValue, exponent_Factor);

    cmptData ++;

    last_time = current_time;
    
    return distanceTOF_mm;
  //}

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

  // #ifdef DEBUG

  //   if(millis() - lastTime2 >= wait){
  //     leftEncoder = ENCODER_ReadReset(LEFT);
  //     rightEncoder = ENCODER_ReadReset(RIGHT);
  //     PRINT(" L : ");
  //     PRINT((String)leftEncoder);
  //     PRINT(" R : ");
  //     PRINT((String)rightEncoder);
  //     PRINT(" diffLR : ");
  //     PRINT((String)diffEncodersLR);
  //     PRINT(" diffRL : ");
  //     PRINT((String)diffEncodersRL);
  //     PRINT(" L Speed : ");
  //     PRINT((String)(leftSpeed));
  //     PRINT(" R Speed : ");
  //     PRINTLN((String)(rightSpeed));
  //     lastTime2 = millis();
  //   }
  // #endif

  MOTOR_SetSpeed(LEFT, leftSpeed);
  MOTOR_SetSpeed(RIGHT, rightSpeed);

}







