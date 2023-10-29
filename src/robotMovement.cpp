#include "robotMovement.hpp"
#include "pid.hpp"

struct accelerationParameters
{
  float baseSpeed = 0.1;
  float accelerationSpeed = 1;
  float maxSpeed = 1;
};

accelerationParameters accParams;

void initializeAccelerationParams(float maxSpeed, float baseSpeed = 0.1, float accelerationSpeed = 1)
{
  accParams.maxSpeed = maxSpeed;
  accParams.baseSpeed = baseSpeed;
  accParams.accelerationSpeed = accelerationSpeed;
}

void move(float motorSpeed, int distance_cm)
{
  resetEncoders();
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;
  float motorAccelerationSpeed = accParams.baseSpeed;
  
  initializeAccelerationParams(motorSpeed);
  static unsigned int startTime = millis();

  MOTOR_SetSpeed(LEFT_MOTOR, motorAccelerationSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorAccelerationSpeed);
  
  float expectedLeftPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
  float expectedRightPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;

  while ((float)ENCODER_Read(LEFT_MOTOR) <= expectedLeftPulses)
  {
    motorAccelerationSpeed = getAcceleration(startTime);
    pid(motorAccelerationSpeed, motorAccelerationSpeed, expectedLeftPulses, expectedRightPulses);
  }
  stop();
}

float getAcceleration(unsigned int startTime)
{
  unsigned int currentTime = millis();

  // Voir https://www.desmos.com/calculator/nu9zyg5lji
  float outputSpeed = 
    accParams.maxSpeed / 
    (1 + ((accParams.maxSpeed - accParams.baseSpeed) / accParams.baseSpeed) * 
      exp(-accParams.accelerationSpeed * ((currentTime - startTime) * (accParams.accelerationSpeed * 0.003)))
    );

  outputSpeed = constrain(outputSpeed, -1, 1);
  return outputSpeed;
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