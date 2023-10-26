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

#ifdef ROBOT_B
  #define KP 2.25
  #define KI 0.01
  #define KD 0.005
#else
    #ifdef ROBOT_A
      #define KP 0.0005
      #define KI 0.00000002
      #define KD 2.0
    #endif
#endif

typedef enum {
    RightTurn = 1,
    LeftTurn = -1 
} turnDirection;

extern float leftMotorPrevErr;
extern float rightMotorPrevErr;

extern float leftMotorIntegral;
extern float rightMotorIntegral;

extern unsigned long previousTime;

void move(float motorSpeed, int distance_cm);
void turnSmooth();
void stop();
void resetEncoders();
#endif