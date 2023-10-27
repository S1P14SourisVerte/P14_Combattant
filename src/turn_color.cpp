#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Arduino.h>
#include <Librobus.h>

#define PI 3.14159265359
#define Wheel_Circumference 22.86
#define Foot_To_Centimeter 30.48
#define Distance_Between_Wheels 19.0

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
}

// Fonction pour TOURNER

int Turn_DF() {
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




// Fonction pour détecter avec le capteur IR
void SensorIR(){


}



void setup() {
  // POUR LE TURN
  BoardInit();
  Serial.begin(9600);

  // POUR LA COULEUR
  Wire.begin();
}

void loop() {
  Serial.println(Floor_Color());
  
}