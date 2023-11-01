#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Arduino.h>
#include <Librobus.h>
#include <robotMovement.hpp>
#include <detection.hpp>
#include <robotServo.hpp>
 
#define Wheel_Circumference 22.86
#define Foot_To_Centimeter 30.48
#define Distance_Between_Wheels 19.0
#define motor_SetSpeed 0.30
#define REF 50
 
bool Cup_Drop = false;
// POUR QUE LA FONCTION SUIVEUR DE LIGNE FONCTIONNE => Définition de la couleur de départ
 
// Fonction pour la COULEUR
 
char Floor_Color() {
  Adafruit_TCS34725 colorSensor;
  colorSensor.begin();
  colorSensor.setGain(TCS34725_GAIN_60X);
 
  uint16_t red, green, blue, c;
  //Pour les données de couleur
  colorSensor.getRawData(&red, &green, &blue, &c);    // Valeur RGB
  delay(300);
 
// VERT
  if (((red > 26) && (red < 40)) && ((green > 42) && (green < 58)) && ((blue > 34) && (blue < 50))){
  return 'V';
  }
// JAUNE
  else if (((red > 108) && (red < 120)) && ((green > 98) && (green < 110)) && ((blue > 54) && (blue < 66))){
  return 'J';
  }
// ROUGE
  else if (((red > 46) && (red < 59)) && ((green > 34) && (green < 48)) && ((blue > 32) && (blue < 47))){
  return 'R';
  }
// BLANC
  else if (((red > 118) && (red < 136)) && ((green > 118) && (green < 136)) && ((blue > 98) && (blue < 120))){
  return 'W';
  }
// BLEU
 else if (((red > 24) && (red < 39)) && ((green > 34) && (green < 49)) && ((blue > 42) && (blue < 60))){
  return 'B';
  }
 
  colorSensor.clearInterrupt();
  return 'E';
}
 
 
void Detect_Line2(char Start_Color)
{
  MOTOR_SetSpeed(LEFT,0);
  MOTOR_SetSpeed(RIGHT,0);
 
  // AVANCE ET TOURNE JUSQU'AU MILIEU DE LA LIGNE
  move(0.40, 59, true, true);
  sharpTurn(RightTurn, 0.25);
 
  if (Start_Color == 'V')
  {
    move (0.30, 36);
  }
 
  else if (Start_Color == 'J')
  {
    move(0.30, 70);
  }
 
    Serial.print("Fin de ligne droite");
 
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
 
  while ((ENCODER_Read(LEFT) < 8900)){
 
    if ((distanceTOF_mm() > 150) && (distanceTOF_mm() < 200) && (Cup_Drop == false))
    {
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, 0);
      move(0.25,5);
      dropCup();
      Cup_Drop = true;
    }
    else {
       if (analogRead(A1) < REF)
    {
      Serial.print("Correction à droite");
      MOTOR_SetSpeed(LEFT, 0.18);
      MOTOR_SetSpeed(RIGHT, 0.30);
      delay(75);
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
 
    else if (analogRead(A2) < REF)
    {
      Serial.print("Pas de correction");
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
 
    else if (analogRead(A3) < REF)
    {
      Serial.print("Correction à gauche");
      MOTOR_SetSpeed(LEFT, 0.30);
      MOTOR_SetSpeed(RIGHT, 0.18);
      delay(75);
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
 
    else
    {
      Serial.print("TOUT DROIT");
      MOTOR_SetSpeed(LEFT, 0.30);
      MOTOR_SetSpeed(RIGHT, 0.30);
    }
    }
  }
 
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
 
  sharpTurn(RightTurn, 0.25, 45);
 
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
 
// MEME PRINCIPE POUR LA DEUXIÈME LIGNE OÙ IL PEUT AVOIR LA BALLE
 
while ((ENCODER_Read(LEFT) < 11525)){
 
    if ((distanceTOF_mm() > 150) && (distanceTOF_mm() < 200) && (Cup_Drop == false))
    {
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, 0);
      move(0.25,5);
      dropCup();
      Cup_Drop = true;
    }
    else if (Cup_Drop == true) {
      move(0.25, 86);
    }
    else{
       if (analogRead(A1) < REF)
    {
      Serial.print("Correction à droite");
      MOTOR_SetSpeed(LEFT, 0.25);
      MOTOR_SetSpeed(RIGHT, 0.30);
      delay(75);
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
 
    else if (analogRead(A2) < REF)
    {
      Serial.print("Pas de correction");
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
 
    else if (analogRead(A3) < REF)
    {
      Serial.print("Correction à gauche");
      MOTOR_SetSpeed(LEFT, 0.30);
      MOTOR_SetSpeed(RIGHT, 0.25);
      delay(75);
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
 
    else
    {
      MOTOR_SetSpeed(LEFT, 0.25);
      MOTOR_SetSpeed(RIGHT, 0.25);
    }
    }
  }
 
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
 
  sharpTurn(RightTurn, 0.25, 45);
 
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
}

void initialiser_color() {
  // POUR LA COULEUR
Wire.begin();
 
// POUR LE SERVO MOTEUR
servoInit();
 
// POUR LE SUIVEUR DE LIGNE
pinMode(A1, INPUT);
pinMode(A2, INPUT);
pinMode(A3, INPUT);
}