#include <Arduino.h>
#include <Librobus.h>
#include "robotMovement.hpp"
#include "pid.hpp"

#define FRONT_SENSOR1 A7
#define FRONT_SENSOR2 0
#define BOX_DIMENSION 200
#define motorSpeed 0.40

#define LANE 8
#define MAX_ENCODER 8000

// Code pour observer les obstacles en avant (environ 20-25cm)
bool FrontObject() {
  float value = ROBUS_ReadIR(FRONT_SENSOR2);
  return((value > 300) ? true : false);
}

// Code final 
bool active;
int STATE, attempt, lane_cmpt;
float encoders_temp;

void setup(){
  BoardInit();
  active = false;
  STATE = 0, attempt = 0, lane_cmpt = 1;
}

void loop(){
// APPUI SUR UN BOUTON POUR COMMENCER LE ROBOT (INTERACTION 1)

if (ROBUS_IsBumper(1))
  active = (!active) ? true : false;

// SI BOUTON A ÉTÉ APPUYÉ!
if (active){

  // SI OBJET EN AVANT DE TOI
  if (FrontObject()){
    STATE = 1;
    attempt++;
  }

  if (encoders_temp >= MAX_ENCODER){
    STATE = 3;
  }

  // SWITCH CASE D'ACTION
  switch(STATE){

    case(0): 
      // FONCTION POUR AVANCER TOUT DROIT + BROSSE
      break;
    
    case(1):
      MOTOR_SetSpeed(LEFT, 0);
      MOTOR_SetSpeed(RIGHT, 0);
      //Bruit
      delay(2500);

      if (attempt == 2){
        STATE = 2;
        attempt = 0;
      }
      break;

    case(2):
    // FONCTION POUR CONTOURNER
      encoders_temp = ENCODER_Read(LEFT);
      sharpTurn(RightTurn);
      move(motorSpeed, 10);
      sharpTurn(LeftTurn);
      
      // ON RESET LES ENCODERS
      ENCODER_Reset(LEFT);
      move(motorSpeed, 10);

      // ON AJOUTE À encoders_temp
      encoders_temp =  ENCODER_Read(LEFT) + encoders_temp;
      sharpTurn(LeftTurn);
      move(motorSpeed, 10);
      sharpTurn(RightTurn);

      // ON RECOMMENCE À ALLER TOUT DROIT
      STATE = 0;
    default:
      break;
    
    case(3):
    // QUAND IL EST AU BOUT DE SON PARCOURS
    if (lane_cmpt % 2 == 1){
      sharpTurn(RightTurn);
      move(motorSpeed, 10);
      sharpTurn(RightTurn);
    }

    else if (lane_cmpt % 2 == 0){
      // Drop les déchets
      move(motorSpeed, 10);
      sharpTurn(LeftTurn);
    }

    ENCODER_ReadReset(LEFT);
    ENCODER_ReadReset(RIGHT);
    encoders_temp = 0;
    lane_cmpt++;
    
    }
  }
}
