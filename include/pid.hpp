#pragma once
#ifndef _PID_HPP_
#define _PID_HPP_

#include <LibRobus.h>

#ifdef ROBOT_B
  #define KP 2.25
  #define KI 0.01
  #define KD 0.005
#else
    #ifdef ROBOT_A
      #define KP 0.0005
      #define KI 0.00000008
      #define KD 2.0
      #define ALPHA 0.5
      #define SATURATION_MAX 0.04
      #define SATURATION_MIN -0.04
    #endif
#endif

extern unsigned long previousTime;


void pidInit(float kp, float ki, float kd, float alpha, float satMax, float satMin);
void pid(float leftMotorSpeed, float rightMotorSpeed, int32_t expectedLeftMotorPulses, int32_t expectedRightMotorPulses);

#endif