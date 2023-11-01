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
  // Serial.print("current : ");
  // int test = distanceTOF_mm();
  // Serial.println(test);
  // delay(1000);
  // sharpTurn(RightTurn, 0.1, 10.0);
  // delay(10);
  stop();
    // delay(100);

    int previousDistance;
    int currentDistance;
    int direction = 1;

    currentDistance = distanceTOF_mm();
    sharpTurn(RightTurn, 0.1, 10.0);

    // delay(500);
    previousDistance = currentDistance;
    currentDistance = distanceTOF_mm();
    // delay(500);

    Serial.print("previous : ");
    Serial.println(previousDistance);
    Serial.print("current : ");
    Serial.println(currentDistance);
  
    direction = (previousDistance > currentDistance) ? direction : direction * -1;
    Serial.println(distance);
    if(direction == 1)
      correctAngle(currentDistance, previousDistance, RightTurn);
    else
      correctAngle(currentDistance, previousDistance, LeftTurn);

  while(analogRead(A1) < 50 || analogRead(A2) < 50 || analogRead(A3) < 50)
    move(0.3, 5, false, false);
  delay(1000);
}

void correctAngle(int currentDistance, int previousDistance, turnDirection d) {
  while(previousDistance > currentDistance && previousDistance/currentDistance > 1) {
    sharpTurn(d, 0.1, 10.0);
    previousDistance = currentDistance;
    currentDistance = distanceTOF_mm();

    Serial.print("previous : ");
    Serial.println(previousDistance);
    Serial.print("current : ");
    Serial.println(currentDistance);
  }
}

int distanceTOF_mm(){

  unsigned int distanceTOF_mm = 0;

  // int current_time = millis();

  // if(current_time - last_time >= 1000){
    avalogValue = analogRead(RIGHT_SENSOR);
    distanceTOF_mm = (float) correctionFactor * pow((float) avalogValue, exponent_Factor);

    cmptData ++;

    // last_time = current_time;
    
    return distanceTOF_mm;
  // }
}

void DetectionInit(void) {
    Serial.begin(9600);
    pinMode(GREEN, INPUT);
    pinMode(RED, INPUT);
    pinMode(RIGHT_SENSOR, INPUT);
}