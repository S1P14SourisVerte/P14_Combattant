#ifndef DETECTION_H
#define DETECTION_H

#include <Arduino.h>

#define YES 1
#define NO 0
#define GREEN 28
#define RED 26
#define RIGHT_SENSOR A7
#define TARGET_DISTANCE 34.00
// -> 0.86 V

bool cupGreen(void);
void DetectionInit(void);
bool cupYellow(void);
#endif