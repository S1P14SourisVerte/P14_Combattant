#include "robotServo.hpp"

void servoInit(void)
{
    Serial.begin(9600);

    SERVO_Enable(SERVO_1);
    SERVO_Enable(SERVO_2);
    SERVO_SetAngle(SERVO_1, BASE_ANGLE);
}

void sweepCup(void) 
{
    int counter = 0;

    while(counter < 2) {
        SERVO_SetAngle(SERVO_2, 0);
        delay(650);
        SERVO_SetAngle(SERVO_2, 180);
        delay(650);
        counter++;
    }
}

void dropCup(void)
{
    SERVO_SetAngle(SERVO_1, 55);
}

void resetServo(void)
{
    delay(1000);
    SERVO_SetAngle(SERVO_1, BASE_ANGLE);
    delay(1000);
    SERVO_Disable(SERVO_1);
}