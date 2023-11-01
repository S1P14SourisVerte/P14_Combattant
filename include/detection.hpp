#ifndef DETECTION_H
#define DETECTION_H

#include <Arduino.h>
#include <LibRobus.h>
#include <math.h>

#define YES 1
#define NO 0
#define GREEN 28
#define RED 26
#define RIGHT_SENSOR A7
#define CUP_DISTANCE 150.0
#define TARGET_DISTANCE 340.0
// -> 0.86 V

bool cupGreen(void);
void DetectionInit(void);
bool cupYellow(void);
void followWall();
void orientationFactor(float, float);
int distanceTOF_mm();
#endif