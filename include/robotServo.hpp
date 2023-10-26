#ifndef ROBOTSERVO_HPP
#define ROBOTSERVO_HPP

#include <Arduino.h>
#include <LibRobus.h>

extern MegaServo backServo;
extern MegaServo frontServo;

extern int backServoAngle;
extern int frontServoAngle;

void servoInit(void);
void servoRotate(MegaServo);
#endif