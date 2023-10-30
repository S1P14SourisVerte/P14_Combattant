#include "detection.hpp"

bool cupGreen() {
    int green = digitalRead(GREEN);
    int red = digitalRead(RED);
    return((green == 1 || red == 1) ? true : false);
}

bool cupYellow() {
    float voltage = analogRead(RIGHT_SENSOR);
    return((voltage > 0.6) ? true : false);
   
}

void DetectionInit(void) {
    Serial.begin(9600);
    pinMode(GREEN, INPUT);
    pinMode(RED, INPUT);
    pinMode(RIGHT_SENSOR, INPUT);
}