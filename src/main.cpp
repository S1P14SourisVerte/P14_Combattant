#include <Arduino.h>

#include <LibRobus.h>

unsigned long previousTime = 0;

void setup() 
{
  BoardInit();
}

void loop() 
{
  if(ROBUS_IsBumper(3)){

    float u;
    float speed = 0.5;
    int error = 0;
    float derivated_error = 0;
    float old_error = 0;
    float derivated_old_error = 0;
    float integrated_error = 0;
    //proportionnel
    float Kp = 0.0005;
    //dérivée
    float Kd = 2.0;
    //intégral
    float Ki = 0.00000002;
    //Temps entre 2 exécutions de la boucle
    unsigned long currentTime = millis();
    double timeDifference_sec = (currentTime - previousTime);
    //Variable pour filtre
    float Alpha = 0.5;
    //paramètres saturation
    float max = 0.04;
    float min = -0.04;
    //Actions de correction
    float up;
    float ud;
    float ui;

    error = ENCODER_Read(1) - ENCODER_Read(0);
    up = Kp * error;
    derivated_error = (error - old_error) / timeDifference_sec;
    old_error = error;
    //Filtre
    derivated_old_error = (Alpha*derivated_old_error) + ((1-Alpha)*derivated_old_error);
    derivated_old_error = derivated_error;
    //Fin du filtre
    ud = Kd*derivated_error;
    integrated_error = integrated_error + (error*timeDifference_sec);
    ui = Ki*integrated_error;
    u = ui + ud + up;
    //Saturation
    u = constrain(u, min, max);
    //write u
    speed = speed + u;

    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1, 0.5);

    previousTime = millis() - currentTime;

    Serial.print("Encodeur gauche ");
    Serial.print(ENCODER_Read(0));
    Serial.print(" Encodeur droit ");
    Serial.print(ENCODER_Read(1));
    Serial.print(" Vitesse ");
    Serial.print(speed);
    Serial.print("      u ");
    Serial.print(u);
    Serial.print("    ui ");
    Serial.print(ui);
    Serial.print("    up ");
    Serial.print(up);
    Serial.print("    ud ");
    Serial.println(ud);

    delay(100);
  }
}