#include <Arduino.h>

#include <LibRobus.h>
// #include <math.h>

#include "robotMovement.hpp"
// #include "pid.hpp"

void setup() 
{
  BoardInit();
}

void loop() 
{
  if(ROBUS_IsBumper(3))
  {
    // move(0.5, 500);
    turnSmooth();
    exit(EXIT_SUCCESS);
  }
}