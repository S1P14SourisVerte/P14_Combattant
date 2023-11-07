#include <Arduino.h>
#include <Librobus.h>

// PUISQUE L'ON NE CONNAÎT PAS LA GROSSEUR DE LA DIMENSION DU TERRAIN DANS LEQUEL NOUS SERONS, on utilise un chiffre random
#define BOX_DIMENSION 200

// PAS D'UTILISATION DE BOUCLE WHILE, car on veut que le robot puisse faire d'autre chose en avançant

void zigzag(){

// il faut que la fonction move() retourne la distance parcourue et qu'il ne reset pas les encodeurs 
ENCODER_Reset(LEFT);
ENCODER_Reset(RIGHT);

  if (move() < (BOX_DIMENSION - 10)){
    move();
  }

  else{
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(0,0);

  }

}

void setup(){
  BoardInit();
}

void loop(){
  int b;
}