#include "pid.hpp"
#include "robotMovement.hpp"

struct pidParameters
{
  float Kp = 0.0005;
  float Ki = 0.00000008;
  float Kd = 2.0;
  float Alpha = 0.5;
  float SaturationMax = 0.04;
  float SaturationMin = -0.04;
};

pidParameters pidParams;
unsigned long previousTime = millis();

void pidInit(float kp, float ki, float kd, float alpha, float satMax, float satMin)
{
  pidParams.Kp = kp;
  pidParams.Ki = ki;
  pidParams.Kd = kd;
  pidParams.Alpha = alpha;
  pidParams.SaturationMax = satMax;
  pidParams.SaturationMin = satMin;
}

void pid(float leftMotorSpeed, float rightMotorSpeed, int32_t expectedLeftMotorPulses, int32_t expectedRightMotorPulses)
{
  float u;
  float adjustedLeftSpeed = leftMotorSpeed;
  int error = 0;
  float derivatedError = 0;
  float oldError = 0;
  float derivatedOldError = 0;
  float integratedError = 0;
  //Temps entre 2 exécutions de la boucle
  unsigned long currentTime = millis();
  double timeDifference_sec = (currentTime - previousTime);
  //Actions de correction
  float up;
  float ud;
  float ui;

  error = ENCODER_Read(RIGHT_MOTOR) - ENCODER_Read(LEFT_MOTOR);
  up = pidParams.Kp * error;
  derivatedError = (error - oldError) / timeDifference_sec;
  oldError = error;
  //Filtre
  derivatedOldError = (pidParams.Alpha * derivatedOldError) + ((1 - pidParams.Alpha) * derivatedOldError);
  derivatedOldError = derivatedError;
  //Fin du filtre
  ud = pidParams.Kd * derivatedError;
  integratedError = integratedError + (error * timeDifference_sec);
  ui = pidParams.Ki * integratedError;
  u = ui + ud + up;
  //Saturation
  u = constrain(u, pidParams.SaturationMin, pidParams.SaturationMax);
  //write u
  adjustedLeftSpeed += u;

  MOTOR_SetSpeed(LEFT_MOTOR, adjustedLeftSpeed);
  MOTOR_SetSpeed(RIGHT_MOTOR, rightMotorSpeed);

  previousTime = millis() - currentTime;

#ifdef DEBUG
  Serial.print("Encodeur gauche ");
  Serial.print(ENCODER_Read(LEFT_MOTOR));
  Serial.print(" Encodeur droit ");
  Serial.print(ENCODER_Read(RIGHT_MOTOR));
  Serial.print(" Vitesse ");
  Serial.print(adjustedLeftSpeed);
  Serial.print("      u ");
  Serial.print(u);
  Serial.print("    ui ");
  Serial.print(ui);
  Serial.print("    up ");
  Serial.print(up);
  Serial.print("    ud ");
  Serial.println(ud);
#endif

  delay(100);
}