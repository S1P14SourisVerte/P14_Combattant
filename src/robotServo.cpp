#include "robotServo.hpp"

MegaServo backServo;
MegaServo frontServo;

int backServoAngle;
int frontServoAngle;


void servoInit(void)
{
    Serial.begin(9600);

    // Pins are not defined yet
    backServo.attach(10);
    frontServo.attach(11, 544, 2400);

    backServoAngle = 0;
    frontServoAngle = 0;
}

void servoRotate(MegaServo s) 
{
    s.write(180);
}