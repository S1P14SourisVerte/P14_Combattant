#include "pid.hpp"
#include "robotMovement.hpp"

struct pidParameters
{
  float Kp = KP;
  float Ki = KI;
  float Kd = KD;
  float Alpha = ALPHA;
  float SaturationMax = SATURATION_MAX;
  float SaturationMin = SATURATION_MIN;
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
  float error = 0;
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

  // Modification du calcul de lerreur pour permettre eventuellement le support du PID en tournant
  float errorFactor = (float)expectedRightMotorPulses / (float)expectedLeftMotorPulses;
  float leftPulses = ENCODER_Read(LEFT_MOTOR);
  float rightPulses = ENCODER_Read(RIGHT_MOTOR);

  if (leftPulses == 0 || rightPulses == 0)
    return;

  float currentErrorFactor = (float)rightPulses / (float)leftPulses;
  error = leftPulses * (currentErrorFactor - errorFactor);


  // Serial.print(" ELP ");
  // Serial.print(expectedRightMotorPulses);
  // Serial.print(" ERP ");
  // Serial.print(expectedLeftMotorPulses);

  // Serial.print(" err ");
  // Serial.print(error);
  // Serial.print(" errFac ");
  // Serial.print(errorFactor);
  // Serial.print(" currErrFac ");
  // Serial.print(currentErrorFactor);

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

// #ifdef DEBUG
//   Serial.print(" e_g ");
//   Serial.print(ENCODER_Read(LEFT_MOTOR));
//   Serial.print(" e_d ");
//   Serial.print(ENCODER_Read(RIGHT_MOTOR));
//   Serial.print(" v ");
//   Serial.print(adjustedLeftSpeed);
//   Serial.print(" u ");
//   Serial.print(u);
//   Serial.print(" ui ");
//   Serial.print(ui);
//   Serial.print(" up ");
//   Serial.print(up);
//   Serial.print(" ud ");
//   Serial.println(ud);
// #endif
  
  // Le delais non bloquant fait passer le delta pulses de 10 a environ 100-120
  // donc jutilise la fonction delay()
  delay(5);
}