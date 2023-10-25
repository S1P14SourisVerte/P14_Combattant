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

  MOTOR_SetSpeed(LEFT_MOTOR, motorSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, motorSpeed);
  
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
    
    // adjustMotorsSpeed(motorSpeed, motorSpeed, expectedLeftPulses, expectedRightPulses);
    adjustMotorsSpeed(motorSpeed, motorSpeed, (float)ENCODER_Read(RIGHT_MOTOR), (float)ENCODER_Read(RIGHT_MOTOR));
    // PID(RIGHT_MOTOR, LEFT_MOTOR, motorSpeed);
  }
  stop();
}

// void PID(int weakerMotor, int motorID, float speed) {
//   unsigned long currentTime = millis();
//   double timeChange = (double)(currentTime - previousTime) / 1000.0f;

//   float input = ENCODER_Read(motorID);
//   float lastErr = 0;
//   float output = 0;

//   double error = ENCODER_Read(weakerMotor) - input;
//   leftMotorIntegral += (error * timeChange);
//   double dErr = (error - lastErr) / timeChange;

//   output = KP * error + KI * leftMotorIntegral + KD * dErr;
//   Serial.println(output);

//   output = constrain(output, -1, 1);

//   MOTOR_SetSpeed(motorID, output);
//   MOTOR_SetSpeed(RIGHT_MOTOR, speed);

//   lastErr = error;
//   previousTime = currentTime;
//   delay(10);
// }

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

  unsigned long currentTime = millis();
  // double timeDifference_sec = (currentTime - previousTime);
  double timeDifference_sec = (currentTime - previousTime) / 1000.0f;

  float leftMotorError = (expectedLeftSpeed_pulses - currentLeftSpeed_pulses);
  // float rightMotorError = expectedRightSpeed_pulses - currentRightSpeed_pulses;
  // Serial.print("ELP: ");
  // Serial.print(expectedLeftSpeed_pulses);
  // Serial.print(", ERP: ");
  // Serial.print(expectedRightSpeed_pulses);
  // Serial.print(", LP: ");
  // Serial.print(currentLeftSpeed_pulses);
  // Serial.print(", RP: ");
  // Serial.print(currentRightSpeed_pulses);

  leftMotorIntegral += leftMotorError * timeDifference_sec;
  
  // rightMotorIntegral += rightMotorError * timeDifference_sec;

  float leftMotorDerivative = (leftMotorError - leftMotorPrevErr) / timeDifference_sec;
  // float rightMotorDerivative = (rightMotorError - rightMotorPrevErr) / timeDifference_sec;

  float adjustedLeftMotorPulsesCount = (KP * leftMotorError) + (KI * leftMotorIntegral) + (KD * leftMotorDerivative);
  // float adjustedLeftMotorPulsesCount = (KP * leftMotorError) + (KI * leftMotorIntegral) + (KD * leftMotorDerivative);
  // float adjustedRightMotorPulsesCount = (KP * rightMotorError) + (KI * rightMotorIntegral) + (KD * rightMotorDerivative);

  Serial.print("almpc: ");
  Serial.print(adjustedLeftMotorPulsesCount);

  // Serial.print(", ERR: ");
  // Serial.print(KP * leftMotorError);
  // Serial.print(", INT: ");
  // Serial.print(KI * leftMotorIntegral);
  // Serial.print(", DER: ");
  // Serial.println(KD * leftMotorDerivative);

  float leftMotorPulsesModifier = adjustedLeftMotorPulsesCount * expectedLeftSpeed_pulses;
  // float rightMotorPulsesModifier = adjustedRightMotorPulsesCount / expectedRightSpeed_pulses;

  // Serial.print(", LS: ");
  // Serial.print(leftMotorSpeed);
  // Serial.print(", RS: ");
  // Serial.print(rightMotorSpeed);

  // Serial.print("LNS: ");
  // Serial.print(leftMotorSpeed * leftMotorPulsesModifier);
  // Serial.print(", RNS: ");
  // Serial.println(rightMotorSpeed * rightMotorPulsesModifier);

  // Serial.print("ERR: ");
  // Serial.print(leftMotorError);
  // Serial.print(", INT: ");
  // Serial.print(leftMotorIntegral);
  // Serial.print(", DER: ");
  // Serial.print(leftMotorDerivative);
  // Serial.print(", MOD: ");
  // Serial.println(leftMotorPulsesModifier);

  float adjLeftPulses = currentLeftSpeed_pulses + adjustedLeftMotorPulsesCount;
  float nlms;
  if (currentLeftSpeed_pulses == 0 || adjLeftPulses == 0) {
    nlms = leftMotorSpeed;
  } else {
    nlms = leftMotorSpeed * (adjLeftPulses / currentLeftSpeed_pulses == 0 ? 1 : adjLeftPulses / currentLeftSpeed_pulses);
  }

  Serial.print(", clp: ");
  Serial.print(currentLeftSpeed_pulses);
  Serial.print(", adjLeftPu: ");
  Serial.print(adjLeftPulses);
  Serial.print(", nlms: ");
  Serial.print(nlms);
  Serial.print(", ENCODER_L: ");
  Serial.print(ENCODER_Read(LEFT_MOTOR));
  Serial.print(", ENCODER_R: ");
  Serial.println(ENCODER_Read(RIGHT_MOTOR));

  MOTOR_SetSpeed(LEFT_MOTOR, nlms);
  // MOTOR_SetSpeed(LEFT_MOTOR, leftMotorSpeed * (leftMotorPulsesModifier));
  MOTOR_SetSpeed(RIGHT_MOTOR, rightMotorSpeed);

  leftMotorPrevErr = leftMotorError;
  // rightMotorPrevErr = rightMotorError;
  previousTime = currentTime;
  delay(10);
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