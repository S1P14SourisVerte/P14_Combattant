#include "robotMovement.hpp"

float leftMotorPrevErr = 0;
float rightMotorPrevErr = 0;
float leftMotorIntegral = 0;
float rightMotorIntegral = 0;
unsigned long previousTime = millis();

void move(float motorSpeed, int distance_cm)
{
  resetEncoders();
  MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorSpeed);
  // sigmoid logistic
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;

  while ((float)ENCODER_Read(LEFT_MOTOR) <= PULSES_PER_WHEEL_CYCLE * distance_wheelCycles)
  {
    float expectedLeftPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
    float expectedRightPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
    adjustMotorsSpeed(expectedLeftPulses, expectedRightPulses);
  }
  stop();
}

void adjustMotorsSpeed(float expectedLeftSpeed_pulses, float expectedRightSpeed_pulses)
{
  float currentLeftSpeed_pulses = ENCODER_Read(LEFT_MOTOR);
  float currentRightSpeed_pulses = ENCODER_Read(RIGHT_MOTOR);

  unsigned long currentTime = millis();
  double timeDifference = (currentTime - previousTime) / 1000.0f;

  float leftMotorError = expectedLeftSpeed_pulses - currentLeftSpeed_pulses;
  float rightMotorError = expectedRightSpeed_pulses - currentRightSpeed_pulses;

  leftMotorIntegral += leftMotorError * timeDifference;
  rightMotorIntegral += rightMotorError * timeDifference;

  float leftMotorDerivative = (leftMotorError - leftMotorPrevErr) / timeDifference;
  float rightMotorDerivative = (rightMotorError - rightMotorPrevErr) / timeDifference;

  float leftNewSpeed = (KP * leftMotorError) + (KI * leftMotorIntegral) + (KD * leftMotorDerivative);
  float rightNewSpeed = (KP * rightMotorError) + (KI * rightMotorIntegral) + (KD * rightMotorDerivative);

  MOTOR_SetSpeed(LEFT_MOTOR, leftNewSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, rightNewSpeed);

  leftMotorPrevErr = leftMotorError;
  rightMotorPrevErr = rightMotorError;
  previousTime = currentTime;
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