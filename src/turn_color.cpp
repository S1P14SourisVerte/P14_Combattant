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

// Fonction pour le suiveur de ligne

void Detect_Line(char Start_Color) {

    // Capteur Gauche => A1
    // Capteur Centre => A2
    // Capteur Droit => A3

  // Si couleur qu'on était est verte

  if (Start_Color == 'V')
    {

    while (analogRead(A1) > REF)
    {
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
  
    while (analogRead(A3) > REF)
    {
      MOTOR_SetSpeed(LEFT, 0.10);
      MOTOR_SetSpeed(RIGHT, 0.10);
    }

    // UTILE POUR QU'IL SE METTE EXACTEMENT AVEC LE CAPTEUR AU MILIEU
  
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
  
    delay(100);

    sharpTurn(RightTurn, 0.25, 40);

    delay(100);
  
    MOTOR_SetSpeed(LEFT, motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
   }
  
   // Si couleur qu'on était est jaune
   else if (Floor_Color() == 'J') 
   {

    MOTOR_SetSpeed(LEFT, motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
  
    delay(500);

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    sharpTurn(RightTurn, 0.25, 90);

    while (analogRead(A3) > REF)
    {
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }
  
    while (analogRead(A1) > REF)
    {
      MOTOR_SetSpeed(LEFT, 0.10);
      MOTOR_SetSpeed(RIGHT, 0.10);
    }

    MOTOR_SetSpeed(LEFT, 0.10);
    MOTOR_SetSpeed(RIGHT, 0.10);

    delay(1000);
    // UTILE POUR QU'IL SE METTE EXACTEMENT AVEC LE CAPTEUR AU MILIEU
  
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    sharpTurn(RightTurn, 0.25, 40);

    delay(100);
  
    MOTOR_SetSpeed(LEFT, motor_SetSpeed);
    MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
   }

  // 4 POSSIBILITÉS
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  while (Floor_Color() != 'J' || Floor_Color() != 'V')
  {

    if (analogRead(A1) < REF)
    {
      Serial.print("Correction à droite");
      MOTOR_SetSpeed(LEFT, 0.12);
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
      MOTOR_SetSpeed(RIGHT, 0.12);
      delay(75);
      MOTOR_SetSpeed(LEFT, motor_SetSpeed);
      MOTOR_SetSpeed(RIGHT, motor_SetSpeed);
    }

    else if (((analogRead(A1) > REF) && (analogRead(A2) > REF) && (analogRead(A3) > REF)) && (ENCODER_Read(LEFT) > 8150))
    {
      Serial.print("Tous sont dans le blanc et sortis");
      sharpTurn(RightTurn, 0.25, 50);
      delay(50);

      while (analogRead(A3) > REF){
        MOTOR_SetSpeed(LEFT, 0.15);
        MOTOR_SetSpeed(RIGHT, 0.15);
      }
  
      while (analogRead(A1) > REF){
        MOTOR_SetSpeed(LEFT, 0.10);
        MOTOR_SetSpeed(RIGHT, 0.10);
      }

      delay(50);
  
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, 0);

      Serial.print("Fin du tournant");
    }
  }
}

void Detect_Line2(char Start_Color)
{
  // AVANCE ET TOURNE JUSQU'AU MILIEU DE LA LIGNE
  move(1.00, 60, false, false);
  sharpTurn(RightTurn, 0.25);

  if (Start_Color = 'V')
  move (0.25, 30);

  else if (Start_Color = 'J')
    move(0.25, 60);

  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  while ((ENCODER_Read(LEFT) < 8150)){

    if ((distanceTOF_mm() > 100) && (distanceTOF_mm() < 120) && (Cup_Drop == false))
    {
      move(0.25,5);
      dropCup();
      Cup_Drop = true;
    }
    else {
       if (analogRead(A1) < REF)
    {
      Serial.print("Correction à droite");
      MOTOR_SetSpeed(LEFT, 0.12);
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
      MOTOR_SetSpeed(RIGHT, 0.12);
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

// MEME PRINCIPE POUR LA DEUXIÈME LIGNE OÙ IL PEUT AVOIR LA BALLE

while ((ENCODER_Read(LEFT) < 11525)){

    if ((distanceTOF_mm() > 100) && (distanceTOF_mm() < 120) && (Cup_Drop == false))
    {
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
      MOTOR_SetSpeed(LEFT, 0.12);
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
      MOTOR_SetSpeed(RIGHT, 0.12);
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
void setup() {
// POUR LE TURN
BoardInit();
Serial.begin(9600);

// POUR LA COULEUR
Wire.begin();

// POUR LE SERVO MOTEUR
servoInit();

// POUR LE SUIVEUR DE LIGNE
pinMode(A1, INPUT);
pinMode(A2, INPUT);
pinMode(A3, INPUT);
}

void loop() {
  
  Serial.println("COMMENCE");
  MOTOR_SetSpeed(LEFT, 0.0);
  MOTOR_SetSpeed(RIGHT, 0.0);
  delay(5000);
  MOTOR_SetSpeed(LEFT, 0.5);
  MOTOR_SetSpeed(RIGHT, 0.5);
  
}