#include <Arduino.h>
#include <LibRobus.h>

#include "robotMovement.hpp"

void setup() 
{
  BoardInit();
}

void loop()
{
  if(ROBUS_IsBumper(3))
  {
    // move(0.5, 500);
    // move(1, 250, true, true);
    // smoothTurn(0.3);
    sharpTurn(0.2, LeftTurn);
    delay(1000);
    // Serial.print(", CPL: ");
    // Serial.print(ENCODER_Read(LEFT_MOTOR));
    // Serial.print(", CPR: ");
    // Serial.println(ENCODER_Read(RIGHT_MOTOR));
    // exit(EXIT_SUCCESS);
  }
}