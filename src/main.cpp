#include "robotServo.hpp"
#include "robotMovement.hpp"
#include "detection.hpp"

int flag = 0;

void setup()
{
    BoardInit();
    servoInit();
    DetectionInit();
}

void SECTION_4_5() {
 if(1/*FLOOR IS GREEN*/) {
        move(0.3, 5, false, false);
        if(cupGreen()) {
            /*TURN CLOCKWISE 90 DEGREES*/
            sweepCup();
            /*TURN COUNTER-CLOCKWISE 90 DEGREES*/
        }
    }else if(1/*FLOOR IS YELLOW*/) {
        move(0.3, 5, false, false);
        if(cupYellow()) {
            /*TURN COUNTER-CLOCKWISE 90 DEGREES*/
            sweepCup();
            /*TURN CLOCKWISE 90 DEGREES*/
        }
    }
    flag = 6;
}

void SECTION_9_0() {
    move(1, 200, true, true);

}

void loop()
{
    followWall();
}