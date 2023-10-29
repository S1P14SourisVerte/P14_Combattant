#pragma once
#ifndef _ROBOTMOVEMENT_HPP_
#define _ROBOTMOVEMENT_HPP_

#include <math.h>
#include <LibRobus.h>

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define WHEEL_CIRCONFERENCE_CM 23.93f
#define SELF_TURN_CIRCONFERENCE_CM 60.65f
#define PULSES_PER_WHEEL_CYCLE 3200.0f

typedef enum {
    RightTurn = 1,
    LeftTurn = -1 
} turnDirection;

extern float leftMotorPrevErr;
extern float rightMotorPrevErr;

extern float leftMotorIntegral;
extern float rightMotorIntegral;

extern unsigned long previousTime;

void initializeAccelerationParams(float maxSpeed, float baseSpeed = 0.1, float accelerationSpeed = 1);
void move(float motorSpeed, int distance_cm);
float getAcceleration(unsigned int startTime);
void stop();
void resetEncoders();
#endif