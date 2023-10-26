#include "robotMovement.hpp"
#include "pid.hpp"

void turnSmooth()
{
  resetEncoders();

  #define PI 3.14159265359
#define Wheel_Circumference 22.86
#define Foot_To_Centimeter 30.48
#define Distance_Between_Wheels 19.0

  float radius_little;
  float radius_big;
  float distance_little;
  float distance_big;
  int pulse_droite;
  int pulse_gauche;

    radius_little = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) - (Distance_Between_Wheels/2));
    radius_big = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) + (Distance_Between_Wheels/2));

    // TROUVER LE NOMBRE DE CM À FAIRE
    distance_little = ((2 * PI * radius_little)/4);
    distance_big = ((2 * PI * radius_big)/4);

    // TROUVER LE NOMBRE DE PULSES À FAIRE 
    pulse_droite = ((distance_little / Wheel_Circumference) * 3200);
    pulse_gauche = ((distance_big / Wheel_Circumference) * 3200);

  float leftSpeed = 0.25;
  float rightSpeed = 0.125;
  float expectedLeftPulses = 3200;
  float expectedRightPulses = 1600;

  // MOTOR_SetSpeed(LEFT_MOTOR, leftSpeed);
  // MOTOR_SetSpeed(RIGHT_MOTOR, rightSpeed);
  MOTOR_SetSpeed(LEFT_MOTOR, leftSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, (1.07 * leftSpeed * (distance_little/distance_big)));

  Serial.print("egauche ");
  Serial.print(ENCODER_Read(pulse_gauche));
  Serial.print(" edroit ");
  Serial.print(ENCODER_Read(pulse_droite));

  while (ENCODER_Read(RIGHT_MOTOR) < pulse_droite)
  {
    Serial.print(" Encodeur gauche ");
    Serial.print(ENCODER_Read(LEFT_MOTOR));
    Serial.print(" Encodeur droit ");
    Serial.print(ENCODER_Read(RIGHT_MOTOR));
    pid(leftSpeed, rightSpeed, pulse_gauche, pulse_droite);
    // pid(leftSpeed, rightSpeed, expectedLeftPulses, expectedRightPulses);
  }
  
  stop();
}

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