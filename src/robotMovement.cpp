#include "robotMovement.hpp"
#include "pid.hpp"

struct accelerationParameters
{
  float baseSpeed = 0.1;
  float accelerationSpeed = 1;
  float maxSpeed = 1;
  float decelerationThreshold = 1;
  float horizontalOffset = 0;
  float verticalOffset = 0;
};

accelerationParameters accParams;

void initializeAccelerationParams(float maxSpeed, 
                                  float baseSpeed, 
                                  float accelerationSpeed,
                                  float horizontalOffset, 
                                  float verticalOffset)
{
  accParams.maxSpeed = maxSpeed;
  accParams.baseSpeed = baseSpeed;
  accParams.accelerationSpeed = accelerationSpeed;
  accParams.horizontalOffset = horizontalOffset;
  accParams.verticalOffset = verticalOffset;

  if (maxSpeed <= 0.3) {
    accParams.decelerationThreshold = 0.975;
  } else if (maxSpeed <= 0.4) {
    accParams.decelerationThreshold = 0.95;
  } else if (maxSpeed <= 0.5) {
    accParams.decelerationThreshold = 0.925;
  } else if (maxSpeed <= 0.65) {
    accParams.decelerationThreshold = 0.875;
  } else if (maxSpeed <= 0.75) {
    accParams.decelerationThreshold = 0.825;
  } else if (maxSpeed <= 0.85) {
    accParams.decelerationThreshold = 0.775;
  } else {
    accParams.decelerationThreshold = 0.725;
  }
}

void move(float motorSpeed, int distance_cm, bool hasAcceleration, bool hasDeceleration)
{
  resetEncoders();
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;
  float motorAccelerationSpeed = hasAcceleration ? accParams.baseSpeed : motorSpeed;
  
  initializeAccelerationParams(motorSpeed);
  static unsigned int startAccTime = millis();

  MOTOR_SetSpeed(LEFT_MOTOR, motorAccelerationSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorAccelerationSpeed);
  
  float expectedLeftPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
  float expectedRightPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;

  while ((float)ENCODER_Read(LEFT_MOTOR) <= expectedLeftPulses)
  {
    if (((float)ENCODER_Read(LEFT_MOTOR) / expectedLeftPulses) < accParams.decelerationThreshold)
    {
      if (hasAcceleration)
      {
        motorAccelerationSpeed = getAcceleration(startAccTime);
      }
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


void turnSmooth(float motorSpeed)
{
  float radius_little;
  float radius_big;
  float distance_little;
  float distance_big;
  int expectedRightPulses;
  int expectedLeftPulses;

  char detectedColor = 'J';
  int turnMultiplier = detectedColor == 'V' ? 1 : 2;
  // char detectedColor = Floor_Color();

  resetEncoders();

  radius_little = 
    (turnMultiplier * FOOT_TO_CENTIMETER) + ((FOOT_TO_CENTIMETER - DISTANCE_BETWEEN_WHEELS_CM) / 2);
  radius_big = 
    turnMultiplier * FOOT_TO_CENTIMETER + ((FOOT_TO_CENTIMETER - DISTANCE_BETWEEN_WHEELS_CM) / 2) + DISTANCE_BETWEEN_WHEELS_CM;

  distance_little = (2 * PI * radius_little) / 4;
  distance_big = (2 * PI * radius_big) / 4;

  expectedRightPulses = ((distance_little / WHEEL_CIRCONFERENCE_CM) * PULSES_PER_WHEEL_CYCLE);
  expectedLeftPulses = ((distance_big / WHEEL_CIRCONFERENCE_CM) * PULSES_PER_WHEEL_CYCLE);

  int pulse_reel_gauche;
  int pulse_reel_droite;

  pulse_reel_gauche = ENCODER_Read(LEFT_MOTOR);
  pulse_reel_droite = ENCODER_Read(RIGHT_MOTOR);
  
  float leftMotorSpeed = motorSpeed;
  float rightMotorSpeed = motorSpeed * (distance_little / distance_big);

  MOTOR_SetSpeed(LEFT_MOTOR, leftMotorSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, rightMotorSpeed);

  while ((float)ENCODER_Read(RIGHT_MOTOR) < expectedRightPulses)
  {
    pulse_reel_gauche = ENCODER_Read(LEFT_MOTOR);
    pulse_reel_droite = ENCODER_Read(RIGHT_MOTOR);

    pid(leftMotorSpeed, rightMotorSpeed, expectedLeftPulses, expectedRightPulses);

    Serial.print("TPL: ");
    Serial.print(expectedLeftPulses);
    Serial.print(", TPR: ");
    Serial.println(expectedRightPulses);
    Serial.print(", CPL: ");
    Serial.print(pulse_reel_gauche);
    Serial.print(", CPR: ");
    Serial.println(pulse_reel_droite);
  }

  stop();
}