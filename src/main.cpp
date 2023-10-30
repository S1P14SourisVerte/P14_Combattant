#include "robotServo.hpp"

bool drop;

void setup() 
{
  BoardInit();
  servoInit();
  drop = false;
}

void loop() 
{
  if(drop == false) {
    dropCup();
    resetServo();
    drop = true;
  }
  // sweepCup();
}