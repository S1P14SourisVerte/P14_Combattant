#pragma once
#ifndef _PID_HPP_
#define _PID_HPP_

#include <LibRobus.h>

extern unsigned long previousTime;

void pidInit(float kp, float ki, float kd, float alpha, float satMax, float satMin);
void pid(float leftMotorSpeed, float rightMotorSpeed, int32_t expectedLeftMotorPulses, int32_t expectedRightMotorPulses);

#endif