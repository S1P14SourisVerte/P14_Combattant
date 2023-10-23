#include <Arduino.h>
#include <Librobus.h>

#define PI 3.14159265359
#define Wheel_Circumference 22.86
#define Foot_To_Centimeter 30.48
#define Distance_Between_Wheels 19.0

// PROTOTYPE DE FONCTION
int Floor_Color();
int Turn_DF();
int Follow_Line();

// FONCTION COULEUR

int Floor_Color() {
  return(0);
}

// FONCTION TOURNER
int Turn_DF() {
  float radius_little;
  float radius_big;
  float distance_little;
  float distance_big;
  int pulse_droite;
  int pulse_gauche;

// SI LA COULEUR DETECTÉ EST VERT
  if (Floor_Color() == 0) {
    // TROUVER LE RAYON À FAIRE
    radius_little = Foot_To_Centimeter + ((Foot_To_Centimeter/2) - (Distance_Between_Wheels/2));
    radius_big = Foot_To_Centimeter + ((Foot_To_Centimeter/2) + (Distance_Between_Wheels/2));

    // TROUVER LE NOMBRE DE CM À FAIRE
    distance_little = ((2 * PI * radius_little)/4);
    distance_big = ((2 * PI * radius_big)/4);

    // TROUVER LE NOMBRE DE PULSES À FAIRE 
    pulse_droite = ((distance_little / Wheel_Circumference) * 3200);
    pulse_gauche = ((distance_big / Wheel_Circumference) * 3200);
  }
  
  
  // SI LA COULEUR DÉTECTÉ EST JAUNE
  else if (Floor_Color() == 1) {
    // TROUVER LE RAYON À FAIRE
    radius_little = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) - (Distance_Between_Wheels/2));
    radius_big = 2* Foot_To_Centimeter + ((Foot_To_Centimeter/2) + (Distance_Between_Wheels/2));

    // TROUVER LE NOMBRE DE CM À FAIRE
    distance_little = ((2 * PI * radius_little)/4);
    distance_big = ((2 * PI * radius_big)/4);

    // TROUVER LE NOMBRE DE PULSES À FAIRE 
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

  // ON RENTRE DANS LA BOUCLE WHILE POUR REGARDER LE NB DE PULSES

void setup() {
  BoardInit();
  Serial.begin(9600);
}

void loop() {
  Turn_DF();
}






// Étape 1: Regarder couleur en dessous de soi => INITIALISER COULEUR
// Étape 2: Avancer tout droit (fonction SAM + ANTOINE) jusqu'à temps de voir une ligne noire
// Étape 3: Mettre fonction pour tourner : Dire au robot de faire un certain nb de pulse pour moteur gauche et moteur droit (calcul au préalable)
// Étape 4: Tourne jusqu'à temps d'atteindre le nb de pulses
// Étape 5: Avancer tout droit jusqu'à temps de voir une couleur => Quand voit une couleur tourne comme à l'étape 3 et 4


// POUR LE TOURNANT => Nb de pulses différent en fonction de la couleur => SI VERT => nb de pulse 1 / SI JAUNE => nb de pulse 2


// FONCTION TOURNER (couleur) => Boucle if à l'intérieur 

// CIRCONFÉRENCE TRAJECTOIRE VERT quart de cercle (petit) = (2 * pi * (1 pied ENVIRON)) / 4 
// NOMBRE DE TOURS DE ROUE: Circonference_Trajectoire / (9.4247 pouces)
// NOMBRE DE PULSES: Nombre_de_tours * 3200 

// CIRCONFÉRENCE TRAJECTOIRE VERT quart de cercle (grand) = (2 * pi * (2 pied ENVIRON)) / 4 
// NOMBRE DE TOURS DE ROUE: Circonference_Trajectoire / (9.4247 pouces)
// NOMBRE DE PULSES: Nombre_de_tours * 3200 

// EN THÉORIE, la roue de gauche DEVRAIT FAIRE 2x le nombre de pulses de la roue droite




// CIRCONFÉRENCE TRAJECTOIRE JAUNE quart de cercle (petit) = (2 * pi * (2 pied ENVIRON)) / 4 
// NOMBRE DE TOURS DE ROUE: Circonference_Trajectoire / (9.4247 pouces)
// NOMBRE DE PULSES: Nombre_de_tours * 3200 

// CIRCONFÉRENCE TRAJECTOIRE JAUNE quart de cercle (grand) = (2 * pi * (3 pied ENVIRON)) / 4 
// NOMBRE DE TOURS DE ROUE: Circonference_Trajectoire / (9.4247 pouces)
// NOMBRE DE PULSES: Nombre_de_tours * 3200 

// EN THÉORIE, la roue de gauche DEVRAIT FAIRE 1.5x le nombre de pulses de la roue droite




//
