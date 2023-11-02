#include "detection.hpp"
#include "robotMovement.hpp"

float previousDistance;
float currentDistance;
float rightCorrection = 1;
float leftCorrection = 1;

unsigned int distance = 0;
unsigned int correctionFactor = 17158;
float exponent_Factor = -0.797;

unsigned int avalogValue = 0;
unsigned long cmptData = 0;

static int last_time = 0;

bool cupGreen() {
    int green = digitalRead(GREEN);
    int red = digitalRead(RED);
 
    return((green == 0 || red == 0) ? true : false);
}

bool cupYellow() {
    float distance = analogRead(A7);
    Serial.println(" distance: ");
    Serial.println(distance);
    return((distance > 400) ? true : false);
}

void followWall() {
    previousDistance = distanceTOF_mm();

    #ifdef DEBUG
        Serial.println(previousDistance);
    #endif

    MOTOR_SetSpeed(LEFT, 0.3 * leftCorrection);
    MOTOR_SetSpeed(RIGHT, 0.3 * rightCorrection);
    currentDistance = analogRead(RIGHT_SENSOR);
    if(currentDistance != TARGET_DISTANCE && previousDistance/currentDistance != 1.0f)
        orientationFactor();
}

void orientationFactor() {
    if(previousDistance/currentDistance > 1) {
        leftCorrection = 0.9;
        rightCorrection = 1.1;
    }
    else if (previousDistance/currentDistance < 1) {
        leftCorrection = 1.1;
        rightCorrection = 0.9;
    }
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