#pragma once
#ifndef _ROBOTMOVEMENT_HPP_
#define _ROBOTMOVEMENT_HPP_

#include <math.h>
#include <LibRobus.h>

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define WHEEL_CIRCONFERENCE_CM 23.93f
#define DISTANCE_BETWEEN_WHEELS_CM 19.3055
#define SELF_TURN_CIRCONFERENCE_CM 60.65f
#define PULSES_PER_WHEEL_CYCLE 3200.0f

#define FOOT_TO_CENTIMETER 30.48

typedef enum {
    RightTurn = 1,
    LeftTurn = -1 
} turnDirection;

extern float leftMotorPrevErr;
extern float rightMotorPrevErr;

extern float leftMotorIntegral;
extern float rightMotorIntegral;

extern unsigned long previousTime;

void initializeAccelerationParams(float maxSpeed, 
                                  float baseSpeed = 0.1, 
                                  float accelerationSpeed = 1,
                                  float horizontalOffset = 0, 
                                  float verticalOffset = 0);
void move(float motorSpeed, int distance_cm, bool hasAcceleration = true, bool hasDeceleration = false);
float getAcceleration(unsigned int startTime, bool isAcceleration = true);
void stop();
void resetEncoders();
void turnSmooth(float motorSpeed);
#endif