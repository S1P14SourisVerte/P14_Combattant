#include <Arduino.h>
#include <Librobus.h>
#include "robotMovement.hpp"
#include "pid.hpp"

#define FRONT_SENSOR1 A7
#define FRONT_SENSOR2 0
#define BOX_DIMENSION 200
#define motorSpeed 0.40


// Code pour observer les obstacles en avant (test 1)
bool FrontObject1() {
  float distance = analogRead(FRONT_SENSOR1);
  return((distance > 400) ? true : false);
}

// Code pour observer les obstacles en avant (test 2)
bool FrontObject2() {
  float value = ROBUS_ReadIR(FRONT_SENSOR2);
  return((value > 400) ? true : false);
}

// Code pour faire la partie en zigzag
void Zigzag_Movement(){
  
  int distance = 20;

  if (distance < (BOX_DIMENSION - 20)){
    MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed * 0.979);
    MOTOR_SetSpeed(RIGHT_MOTOR, motorSpeed);
  }

  else{
    delay(100);
    resetEncoders();
  }

}
void setup(){
  BoardInit();
}

void loop(){
  int b;
}