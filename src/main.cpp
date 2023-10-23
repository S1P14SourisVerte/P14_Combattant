#include <Arduino.h>

#include "robotMovement.hpp"

void setup()
{
  BoardInit();
}

void loop()
{
  if (ROBUS_IsBumper(3))
  {
    move(0.5, 250);
  }
}