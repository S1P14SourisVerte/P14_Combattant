#include "robotServo.hpp"

void setup() 
{
  BoardInit();
  servoInit();
}

void loop() 
{
 servoRotate(backServo);
 delay(2000);
 servoRotate(frontServo);
}