#ifndef ROBOTSERVO_HPP
#define ROBOTSERVO_HPP

#include <Arduino.h>
#include <LibRobus.h>

#define BASE_ANGLE 40

void servoInit(void);
void sweepCup(void);
void dropCup(void);
void resetServo(void);
#endif