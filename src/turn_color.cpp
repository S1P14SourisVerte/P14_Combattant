#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Arduino.h>
#include <Librobus.h>

#define Wheel_Circumference 22.86
#define Foot_To_Centimeter 30.48
#define Distance_Between_Wheels 19.0
#define motor_SetSpeed 0.10

// POUR QUE LA FONCTION SUIVEUR DE LIGNE FONCTIONNE => Définition de la couleur de départ

char Start_Color = 'V';

// Fonction pour la COULEUR

char Floor_Color() {
  Adafruit_TCS34725 colorSensor;
  colorSensor.begin();
  colorSensor.setGain(TCS34725_GAIN_60X);

  uint16_t red, green, blue, c;
  //Pour les données de couleur
  colorSensor.getRawData(&red, &green, &blue, &c);		// Valeur RGB
  delay(300);

// VERT
  if (((red > 22) && (red < 36)) && ((green > 38) && (green < 50)) && ((blue > 28) && (blue < 46))){
  return 'V';
  }
// JAUNE
  else if (((red > 108) && (red < 120)) && ((green > 96) && (green < 108)) && ((blue > 48) && (blue < 59))){
  return 'J';
  }
// ROUGE
  else if (((red > 46) && (red < 59)) && ((green > 29) && (green < 42)) && ((blue > 27) && (blue < 37))){
  return 'R';
  }
// BLANC
  else if (((red > 118) && (red < 136)) && ((green > 118) && (green < 136)) && ((blue > 98) && (blue < 120))){
  return 'W';
  }
// BLEU
 else if (((red > 18) && (red < 32)) && ((green > 30) && (green < 46)) && ((blue > 42) && (blue < 60))){
  return 'B';
  }

  colorSensor.clearInterrupt();
  return 'E';
}

// Fonction pour TOURNER

void Turn_DF() {
  float radius_little;
  float radius_big;
  float distance_little;
  float distance_big;
  int pulse_droite;
  int pulse_gauche;

//Si la couleur détecté est verte

  if (Floor_Color() == 'V') {

    radius_little = Foot_To_Centimeter + ((Foot_To_Centimeter/2) - (Distance_Between_Wheels/2));
    radius_big = Foot_To_Centimeter + ((Foot_To_Centimeter/2) + (Distance_Between_Wheels/2));

    distance_little = ((2 * PI * radius_little)/4);
    distance_big = ((2 * PI * radius_big)/4);
 
    pulse_droite = ((distance_little / Wheel_Circumference) * 3200);
    pulse_gauche = ((distance_big / Wheel_Circumference) * 3200);
  }
  
  
  //Si la couleur détecté est JAUNE

  else if (Floor_Color() == 'J') {

    radius_little = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) - (Distance_Between_Wheels/2));
    radius_big = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) + (Distance_Between_Wheels/2));

    distance_little = ((2 * PI * radius_little)/4);
    distance_big = ((2 * PI * radius_big)/4);

    pulse_droite = ((distance_little / Wheel_Circumference) * 3200);
    pulse_gauche = ((distance_big / Wheel_Circumference) * 3200);

  }

  int pulse_reel_gauche;
  int pulse_reel_droite;

  pulse_reel_gauche = ENCODER_Read(0);
  pulse_reel_droite = ENCODER_Read(1);
  
  while ((pulse_reel_gauche < pulse_gauche) && (pulse_reel_droite < pulse_droite))
  {
    MOTOR_SetSpeed(0, (0.20));
    MOTOR_SetSpeed(1, (1.07* 0.20 * (distance_little/distance_big)));

  pulse_reel_gauche = ENCODER_Read(0);
  pulse_reel_droite = ENCODER_Read(1);
  }

  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);

  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

// Fonction pour tourner les deux roues
 
void Turn_2_Wheel(float speed, float angle, int direction) {
 
  float ratio_p_a;
  float ratio_a_c;
  float circonferenceTrajectory = 23.88;
  float circonferenceWheel = 9.4247;
  float pulses;
  float LeftPulses;
  float RightPulses;
  float pulsesDifference;
  float newSpeedLeft;
  float newSpeedRight;
 
  ratio_a_c = (angle / 360) * circonferenceTrajectory;
  ratio_p_a = (ratio_a_c / circonferenceWheel);
  pulses = (ratio_p_a * 3200);
 
  // Encoder variable
 
  LeftPulses = (float)abs(ENCODER_Read(0));
  RightPulses = (float)abs(ENCODER_Read(1));
 
  if (direction == 0) {
    while (LeftPulses < pulses)
    {
 
      MOTOR_SetSpeed(0, 0.98*speed);
      MOTOR_SetSpeed(1, -speed);
      LeftPulses = (float)abs(ENCODER_Read(0));
      RightPulses = (float)abs(ENCODER_Read(1));
 
    }
 
    LeftPulses = (float)abs(ENCODER_Read(0));
    RightPulses = (float)abs(ENCODER_Read(1));
 
    while (LeftPulses >= RightPulses)
 
    {
      RightPulses = (float)abs(ENCODER_Read(1));
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, -0.01);
      RightPulses = (float)abs(ENCODER_Read(1));
    }
 
    MOTOR_SetSpeed(1, 0);
 
  } else
 
    while ((pulses >= LeftPulses) && (pulses >= RightPulses)) {
 
      LeftPulses = (float)ENCODER_Read(0);
      RightPulses = (float)ENCODER_Read(1);
      pulsesDifference = LeftPulses - RightPulses;
      newSpeedLeft = speed - (0.0005 * pulsesDifference);
      newSpeedRight= speed + (0.0001 * pulsesDifference);
 
      MOTOR_SetSpeed(0, -1*newSpeedLeft);
      MOTOR_SetSpeed(1, newSpeedRight);
 
      LeftPulses = (float)ENCODER_Read(0);
      RightPulses = (float)ENCODER_Read(1);
 
    }
 
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
 
}

// Fonction pour le suiveur de ligne

void Detect_Line() {
  
  // Si couleur qu'on était est verte
  if (Start_Color == 'V')
    {
    
    // Capteur Gauche => A1
    // Capteur Centre => A2
    // Capteur Droit => A3

    while (analogRead(A1) > 5)
    {
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }

    delay(100);

    MOTOR_SetSpeed(LEFT, 0.5* motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, 0.5* motor_SetSpeed);
  
    while (analogRead(A2) > 5)
    {
      MOTOR_SetSpeed(LEFT, 0.5* motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, 0.5* motor_SetSpeed);
    }

    delay(50);
  
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    delay(100);
  
    Turn_2_Wheel(0.10, 45, 0);

    delay(100);
  
    MOTOR_SetSpeed(LEFT, motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
   }
  
   // Si couleur qu'on était est jaune
   else if (Floor_Color() == 'J') 
   {

    MOTOR_SetSpeed(LEFT, motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
  
    delay(1000);

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    Turn_2_Wheel(motor_SetSpeed, 90, 0);

    while (analogRead(A3) > 5)
    {
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }

    delay(100);

    MOTOR_SetSpeed(LEFT, 0.5* motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, 0.5* motor_SetSpeed);
  
    while (analogRead(A2) > 5)
    {
      MOTOR_SetSpeed(LEFT, 0.5* motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, 0.5* motor_SetSpeed);
    }

    delay(50);
  
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    delay(100);
  
    Turn_2_Wheel(0.10, 45, 0);

    delay(100);
  
    MOTOR_SetSpeed(LEFT, motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
   }
}

void setup() {
// POUR LE TURN
BoardInit();
Serial.begin(9600);

  // POUR LA COULEUR
Wire.begin();

// POUR LE SUIVEUR DE LIGNE
pinMode(A1, INPUT);
pinMode(A2, INPUT);
pinMode(A3, INPUT);
}

void loop() {

  Detect_Line();
  
}