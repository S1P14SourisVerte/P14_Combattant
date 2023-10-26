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
      // #define KP 0.01
      // #define KI 0.1
      // #define KD 0.1
      #define KP 0.08
      #define KI 3.0
      #define KD 0.2
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
void PID(int weakerMotor, int motorID, float speed);
void adjustMotorsSpeed(float leftMotorSpeed, float rightMotorSpeed, float expectedLeftSpeed_pulses, float expectedRightSpeed_pulses);
void stop();
void resetEncoders();
#endif