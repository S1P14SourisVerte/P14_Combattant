#include "robotMovement.hpp"
#include "pid.hpp"

struct accelerationParameters
{
  float baseSpeed = 0.1;
  float accelerationSpeed = 1;
  float maxSpeed = 1;
  float horizontalOffset = 0;
  float verticalOffset = 0;
};

accelerationParameters accParams;

void initializeAccelerationParams(float maxSpeed, float baseSpeed, float accelerationSpeed, float horizontalOffset, float verticalOffset)
{
  accParams.maxSpeed = maxSpeed;
  accParams.baseSpeed = baseSpeed;
  accParams.accelerationSpeed = accelerationSpeed;
  accParams.horizontalOffset = horizontalOffset;
  accParams.verticalOffset = verticalOffset;
}

void move(float motorSpeed, int distance_cm, bool hasDeceleration)
{
  resetEncoders();
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;
  float motorAccelerationSpeed = accParams.baseSpeed;
  
  initializeAccelerationParams(motorSpeed);
  static unsigned int startAccTime = millis();

  MOTOR_SetSpeed(LEFT_MOTOR, motorAccelerationSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorAccelerationSpeed);
  
  float expectedLeftPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
  float expectedRightPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;

  while ((float)ENCODER_Read(LEFT_MOTOR) <= expectedLeftPulses)
  {
    if (((float)ENCODER_Read(LEFT_MOTOR) / expectedLeftPulses) < 0.9)
    {
      motorAccelerationSpeed = getAcceleration(startAccTime);
    }
    else if (hasDeceleration)
    {
      static unsigned int startDeccTime = millis();
      motorAccelerationSpeed = getAcceleration(startDeccTime, false);
    }
    
    pid(motorAccelerationSpeed, motorAccelerationSpeed, expectedLeftPulses, expectedRightPulses);
  }
  stop();
}

float getAcceleration(unsigned int startTime, bool isAcceleration)
{
  unsigned int currentTime = millis();

  float maxSpeed = accParams.maxSpeed;
  float baseSpeed = accParams.baseSpeed;
  float accelerationSpeed = accParams.accelerationSpeed;

  // Voir https://www.desmos.com/calculator/nu9zyg5lji
  float outputSpeed = 
    maxSpeed / 
    (1 + ((maxSpeed - baseSpeed) / baseSpeed) * 
      exp(-accelerationSpeed * ((currentTime - startTime) * (accelerationSpeed * 0.003)))
    );

  outputSpeed = constrain(outputSpeed, -1, 1);

  if (!isAcceleration)
  {
    outputSpeed = maxSpeed - outputSpeed;
  }

  if (outputSpeed > -0.1 && outputSpeed < 0.1)
  {
    if (outputSpeed < 0)
    {
      outputSpeed = -0.1;
    }
    else
    {
      outputSpeed = 0.1;
    }
  }

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