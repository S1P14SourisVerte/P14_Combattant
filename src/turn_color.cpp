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
  Serial.println("test");
  Adafruit_TCS34725 colorSensor;
  colorSensor.begin();
  colorSensor.setGain(TCS34725_GAIN_60X);

  uint16_t red, green, blue, c;
  Serial.println("test1");
  //Pour les données de couleur
  colorSensor.getRawData(&red, &green, &blue, &c);		// Valeur RGB
  delay(300);

  Serial.println("test2");
// VERT
  if (((red > 25) && (red < 31)) && ((green > 43) && (green < 47)) && ((blue > 33) && (blue < 39))){
  return 'V';
  }
// JAUNE
  else if (((red > 112) && (red < 122)) && ((green > 105) && (green < 109)) && ((blue > 53) && (blue < 59))){
  Serial.println("test3");
  return 'J';
  }
// ROUGE
  else if (((red > 55) && (red < 59)) && ((green > 33) && (green < 38)) && ((blue > 31) && (blue < 35))){
  return 'R';
  }
// BLANC
  else if (((red > 126) && (red < 132)) && ((green > 130) && (green < 136)) && ((blue > 109) && (blue < 115))){
  return 'B';
  }
// NOIR
  else if (((red > 126) && (red < 132)) && ((green > 130) && (green < 136)) && ((blue > 109) && (blue < 115))){
  return 'N';
  }

  colorSensor.clearInterrupt();
}


// Fonction pour TOURNER

// int Turn_DF() {
//   float radius_little;
//   float radius_big;
//   float distance_little;
//   float distance_big;
//   int pulse_droite;
//   int pulse_gauche;

// Si la couleur détecté est verte

//   if (Floor_Color() == 0) {

//     radius_little = Foot_To_Centimeter + ((Foot_To_Centimeter/2) - (Distance_Between_Wheels/2));
//     radius_big = Foot_To_Centimeter + ((Foot_To_Centimeter/2) + (Distance_Between_Wheels/2));

//     distance_little = ((2 * PI * radius_little)/4);
//     distance_big = ((2 * PI * radius_big)/4);
 
//     pulse_droite = ((distance_little / Wheel_Circumference) * 3200);
//     pulse_gauche = ((distance_big / Wheel_Circumference) * 3200);
//   }
  
  
//   Si la couleur détecté est JAUNE
//   else if (Floor_Color() == 1) {

//     radius_little = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) - (Distance_Between_Wheels/2));
//     radius_big = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) + (Distance_Between_Wheels/2));

//     distance_little = ((2 * PI * radius_little)/4);
//     distance_big = ((2 * PI * radius_big)/4);

//     pulse_droite = ((distance_little / Wheel_Circumference) * 3200);
//     pulse_gauche = ((distance_big / Wheel_Circumference) * 3200);

//   }

//   int pulse_reel_gauche;
//   int pulse_reel_droite;

//   pulse_reel_gauche = ENCODER_Read(0);
//   pulse_reel_droite = ENCODER_Read(1);
  
//   while ((pulse_reel_gauche < pulse_gauche) && (pulse_reel_droite < pulse_droite))
//   {
//     MOTOR_SetSpeed(0, (0.20));
//     MOTOR_SetSpeed(1, (1.07* 0.20 * (distance_little/distance_big)));

//   pulse_reel_gauche = ENCODER_Read(0);
//   pulse_reel_droite = ENCODER_Read(1);
//   }

//   MOTOR_SetSpeed(0,0);
//   MOTOR_SetSpeed(1,0);

//   ENCODER_Reset(0);
//   ENCODER_Reset(1);
// }




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