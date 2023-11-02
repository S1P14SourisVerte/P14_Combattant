#include "robotMovement.hpp"
#include "pid.hpp"
#include "robotServo.hpp"
#include "detection.hpp"

int flagZone45 = 0;

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

  float currentLeftPulses = (float)ENCODER_Read(LEFT_MOTOR);
  float currentRightPulses = (float)ENCODER_Read(RIGHT_MOTOR);

  while (currentLeftPulses <= expectedLeftPulses)
  {
    if ((currentLeftPulses / expectedLeftPulses) < accParams.decelerationThreshold)
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

    currentLeftPulses = (float)ENCODER_Read(LEFT_MOTOR);
    currentRightPulses = (float)ENCODER_Read(RIGHT_MOTOR);
    
    pid(motorAccelerationSpeed, motorAccelerationSpeed, expectedLeftPulses, expectedRightPulses);
  }
  stop();
}

void nonStopMove(float motorSpeed, int distance_cm, bool hasAcceleration, bool hasDeceleration)
{
  // resetEncoders();
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;
  float motorAccelerationSpeed = hasAcceleration ? accParams.baseSpeed : motorSpeed;
  
  initializeAccelerationParams(motorSpeed);
  static unsigned int startAccTime = millis();

  MOTOR_SetSpeed(LEFT_MOTOR, motorAccelerationSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorAccelerationSpeed);
  
  float expectedLeftPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;
  float expectedRightPulses = distance_wheelCycles * PULSES_PER_WHEEL_CYCLE;

  float currentLeftPulses = (float)ENCODER_Read(LEFT_MOTOR);
  float currentRightPulses = (float)ENCODER_Read(RIGHT_MOTOR);

  while (currentLeftPulses <= expectedLeftPulses)
  {
    if ((currentLeftPulses / expectedLeftPulses) < accParams.decelerationThreshold)
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

    currentLeftPulses = (float)ENCODER_Read(LEFT_MOTOR);
    currentRightPulses = (float)ENCODER_Read(RIGHT_MOTOR);

    if(flagZone45) {
      if(cupYellow() /*&& i<2*/) {
        sharpTurn(LeftTurn);
        sweepCup();
        sharpTurn(RightTurn);
        resetEncoders();
      } else if (cupGreen()) {
        sharpTurn(RightTurn);
        sweepCup();
        sharpTurn(LeftTurn);
        resetEncoders();
      }
    }
    
    pid(motorAccelerationSpeed, motorAccelerationSpeed, expectedLeftPulses, expectedRightPulses);
  }
}



void move2(float motorSpeed, int distance_cm)
{
  resetEncoders();
  MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed * 0.979);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorSpeed);
  // sigmoid logistic
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;

  while ((float)ENCODER_Read(LEFT_MOTOR) <= PULSES_PER_WHEEL_CYCLE * distance_wheelCycles)
  {
    if(flagZone45) {
      if(cupYellow()) {
        sharpTurn(LeftTurn);
        sweepCup();
        sharpTurn(RightTurn);
        resetEncoders();
      } else if (cupGreen()) {
        sharpTurn(RightTurn);
        sweepCup();
        sharpTurn(LeftTurn);
        resetEncoders();
      }
    }
    // correctDirection(motorSpeed, PULSES_PER_WHEEL_CYCLE * distance_wheelCycles);
  }
  // stop();
}

void correctDirection(float motorSpeed, float distance_pulses)
{
  if (ENCODER_Read(LEFT_MOTOR) == 0 && ENCODER_Read(RIGHT_MOTOR) == 0)
  {
    return;
  }

  if (motorSpeed < 0.1)
  {
    return;
  }

  float expectedPulses = (float)ENCODER_Read(LEFT_MOTOR);
  float currentPulses = (float)ENCODER_Read(RIGHT_MOTOR);

  float pulsesDifference = expectedPulses - currentPulses;
  if (pulsesDifference == 0)
  {
    return;
  }
  float newSpeedLeft = motorSpeed - (0.0005 * pulsesDifference);
  #ifdef DEBUG
    Serial.print("expec: ");
    Serial.print(expectedPulses);
    Serial.print(", cur: ");
    Serial.print(currentPulses);
    Serial.print(", diff: ");
    Serial.print(pulsesDifference);
    Serial.print(", nspeedL: ");
    Serial.println(newSpeedLeft);
  #endif

  MOTOR_SetSpeed(LEFT_MOTOR, newSpeedLeft);
}

void robotMovementSetFlagZone(int value) {
  flagZone45 = value;
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
      exp(-accelerationSpeed * ((currentTime - startTime) * (accelerationSpeed * 0.001)))
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

void smoothTurn(float motorSpeed, char laneColor)
{
  float radius_little;
  float radius_big;
  float distance_little;
  float distance_big;
  int expectedRightPulses;
  int expectedLeftPulses;
 
  float turnMultiplier = laneColor == 'V' ? 1 : 2;
 
  resetEncoders();
 
  radius_little =
    (turnMultiplier * FOOT_TO_CENTIMETER) + ((FOOT_TO_CENTIMETER - DISTANCE_BETWEEN_WHEELS_CM) / 2);
  radius_big =
    turnMultiplier * FOOT_TO_CENTIMETER + ((FOOT_TO_CENTIMETER - DISTANCE_BETWEEN_WHEELS_CM) / 2) + DISTANCE_BETWEEN_WHEELS_CM;
 
  distance_little = (2 * PI * (1.03 * radius_little) / 4.00);
  if ('J') 
    distance_big = (2 * PI * (1.03 * radius_big) / 4.00) - 2.15;
  else if ('V')
    distance_big = (2 * PI * (1.03 * radius_big) / 4.00) - 2;
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
    Serial.println(distance_little);
    Serial.println(distance_big);
  }
 
  stop();
}

void sharpTurn2(turnDirection direction, float motorSpeed, float angle)
{
  MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed * direction);
  MOTOR_SetSpeed(RIGHT_MOTOR, -motorSpeed * direction);
  float angleCorrectionFactor = 0.52;
  if (direction == LeftTurn) {
    angleCorrectionFactor = -0.7;
  }
  angleCorrectionFactor = (angleCorrectionFactor / 90) * angle;
  float distance_cm = ((SELF_TURN_CIRCONFERENCE_CM / 360.0f) * (angle - angleCorrectionFactor));
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;

  while (abs((float)ENCODER_Read(LEFT_MOTOR)) <= PULSES_PER_WHEEL_CYCLE * distance_wheelCycles)
  {
    correctTurnDirection(motorSpeed, direction);
  }
  stop();
}

void sharpTurn(turnDirection direction, float motorSpeed, float angle)
{
  resetEncoders();
  MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed * direction);
  MOTOR_SetSpeed(RIGHT_MOTOR, -motorSpeed * direction);
  float angleCorrectionFactor = 0.52;
  if (direction == LeftTurn) {
    angleCorrectionFactor = -0.7;
  }
  angleCorrectionFactor = (angleCorrectionFactor / 90) * angle;
  float distance_cm = ((SELF_TURN_CIRCONFERENCE_CM / 360.0f) * (angle - angleCorrectionFactor));
  float distance_wheelCycles = (float)distance_cm / WHEEL_CIRCONFERENCE_CM;

  while (abs((float)ENCODER_Read(LEFT_MOTOR)) <= PULSES_PER_WHEEL_CYCLE * distance_wheelCycles)
  {
    correctTurnDirection(motorSpeed, direction);
  }
  stop();
}

void correctTurnDirection(float motorSpeed, turnDirection direction)
{
  if (abs(ENCODER_Read(LEFT_MOTOR)) < abs(ENCODER_Read(RIGHT_MOTOR)))
  {
    MOTOR_SetSpeed(RIGHT_MOTOR, (-motorSpeed * direction) / CORRECTION_MOTOR_SPEED_FACTOR);
  }
  else if (abs(ENCODER_Read(LEFT_MOTOR)) > abs(ENCODER_Read(RIGHT_MOTOR)))
  {
    MOTOR_SetSpeed(LEFT_MOTOR, (motorSpeed * direction) / CORRECTION_MOTOR_SPEED_FACTOR);
  }
}