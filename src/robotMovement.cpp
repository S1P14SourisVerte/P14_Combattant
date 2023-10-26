#include "robotMovement.hpp"
#include "pid.hpp"

void move(float motorSpeed, int distance_cm)
{
  resetEncoders();
  // sigmoid logistic
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;
  
  MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorSpeed);
  
  while ((float)ENCODER_Read(LEFT_MOTOR) <= PULSES_PER_WHEEL_CYCLE * distance_wheelCycles)
  {
    float expectedLeftPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
    float expectedRightPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
    
    pid(motorSpeed, motorSpeed, ENCODER_Read(RIGHT_MOTOR), ENCODER_Read(LEFT_MOTOR));
  }
  stop();
}

void stop()
{
  MOTOR_SetSpeed(LEFT_MOTOR, 0);
  MOTOR_SetSpeed(RIGHT_MOTOR, 0);
}

void resetEncoders()
{
  ENCODER_ReadReset(LEFT_MOTOR);
  ENCODER_ReadReset(RIGHT_MOTOR);
}