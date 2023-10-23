#include "robotMovement.hpp"

float leftMotorPrevErr = 0;
float rightMotorPrevErr = 0;
float leftMotorIntegral = 0;
float rightMotorIntegral = 0;
unsigned long previousTime = millis();

void move(float motorSpeed, int distance_cm)
{
  resetEncoders();
  // float accelerationMotorSpeed = 
  // MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed);
  // MOTOR_SetSpeed(RIGHT_MOTOR, motorSpeed);
  // sigmoid logistic
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;
  bool accelerationNeeded = true;
  unsigned long startTime_sec = millis();
  float accelerationSpeed = 1;
  float accelerationTime_sec = startTime_sec / accelerationSpeed;
  float motorBaseSpeed;

  if (motorSpeed > 0.1)
  {
    motorBaseSpeed = 0.1;
  }
  else
  {
    accelerationNeeded = false;
  }
  
  while ((float)ENCODER_Read(LEFT_MOTOR) <= PULSES_PER_WHEEL_CYCLE * distance_wheelCycles)
  {
    if (accelerationNeeded && (millis() - startTime_sec < accelerationTime_sec))
    {

    }
    else
    {

    }

    float expectedLeftPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
    float expectedRightPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
    adjustMotorsSpeed(motorSpeed, motorSpeed, expectedLeftPulses, expectedRightPulses);
  }
  stop();
}

float getAccelerationSpeed(float motorMaxSpeed)
{
  float accelerationSpeed = 1;
  float motorBaseSpeed;

  motorMaxSpeed > 0.1 ? motorBaseSpeed = 0.1 : motorMaxSpeed;

  

}

void adjustMotorsSpeed(float leftMotorSpeed, float rightMotorSpeed, float expectedLeftSpeed_pulses, float expectedRightSpeed_pulses)
{
  float currentLeftSpeed_pulses = ENCODER_Read(LEFT_MOTOR);
  float currentRightSpeed_pulses = ENCODER_Read(RIGHT_MOTOR);

  unsigned long currentTime_sec = millis();
  double timeDifference = (currentTime_sec - previousTime) / 1000.0f;

  float leftMotorError = expectedLeftSpeed_pulses - currentLeftSpeed_pulses;
  float rightMotorError = expectedRightSpeed_pulses - currentRightSpeed_pulses;

  leftMotorIntegral += leftMotorError * timeDifference;
  rightMotorIntegral += rightMotorError * timeDifference;

  float leftMotorDerivative = (leftMotorError - leftMotorPrevErr) / timeDifference;
  float rightMotorDerivative = (rightMotorError - rightMotorPrevErr) / timeDifference;

  float adjustedLeftMotorPulsesCount = (KP * leftMotorError) + (KI * leftMotorIntegral) + (KD * leftMotorDerivative);
  float adjustedRightMotorPulsesCount = (KP * rightMotorError) + (KI * rightMotorIntegral) + (KD * rightMotorDerivative);

  float leftMotorPulsesModifier = adjustedLeftMotorPulsesCount / expectedLeftSpeed_pulses;
  float rightMotorPulsesModifier = adjustedRightMotorPulsesCount / expectedRightSpeed_pulses;

  MOTOR_SetSpeed(LEFT_MOTOR, leftMotorSpeed * leftMotorPulsesModifier);
  MOTOR_SetSpeed(RIGHT_MOTOR, rightMotorSpeed * rightMotorPulsesModifier);

  leftMotorPrevErr = leftMotorError;
  rightMotorPrevErr = rightMotorError;
  previousTime = currentTime_sec;
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