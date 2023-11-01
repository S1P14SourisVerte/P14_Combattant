#include "detection.hpp"

unsigned int distance = 0;
unsigned int correctionFactor = 17158;
float exponent_Factor = -0.797;

unsigned int avalogValue = 0;
unsigned long cmptData = 0;

static int last_time = 0;

bool cupGreen() {
    int green = digitalRead(GREEN);
    int red = digitalRead(RED);
    return((green == 1 || red == 1) ? true : false);
}

bool cupYellow() {
    float distance = distanceTOF_mm();
    return((distance > CUP_DISTANCE) ? true : false);
}

void followWall() {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    delay(100);

    float previousDistance;
    float currentDistance;
    turnDirection direction = RightTurn;

    currentDistance = distanceTOF_mm();
    
    delay(100);

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    previousDistance = currentDistance;
    currentDistance = distanceTOF_mm();
    delay(100);
    
    direction = (previousDistance < currentDistance) ? RIGHT : LEFT;
    do{
         MOTOR_SetSpeed(direction, 0.3);
         previousDistance = currentDistance;
         currentDistance = distanceTOF_mm();
    }while(previousDistance < currentDistance && previousDistance/currentDistance < 1);
   

}

int distanceTOF_mm(){

  unsigned int distanceTOF_mm = 0;

 
  int current_time = millis();

  if(current_time - last_time >= 1000){
    avalogValue = analogRead(RIGHT_SENSOR);
    distanceTOF_mm = (float) correctionFactor * pow((float) avalogValue, exponent_Factor);

    cmptData ++;

    last_time = current_time;
    
    return distanceTOF_mm;
  }
}

void DetectionInit(void) {
    Serial.begin(9600);
    pinMode(GREEN, INPUT);
    pinMode(RED, INPUT);
    pinMode(RIGHT_SENSOR, INPUT);

    leftCorrection = 1;
    rightCorrection = 1;
}