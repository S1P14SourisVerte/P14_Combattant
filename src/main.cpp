#include <Arduino.h>
#include <LibRobus.h>
#include <turn_color.hpp>
#include <robotMovement.hpp>

#define PIN_WHISTLE A0
int detect_whistle(void);
bool active;
int current_zone = 0;

void setup() {
    Serial.begin(9600);
    pinMode(PIN_WHISTLE, INPUT);
    active = false;
    initialiser_color();
}

void loop() {
  //Départ
  if(detect_whistle() == 1){
    active = true;
  }
  if (active){
    char start_color = Floor_Color();
    //CASE 1
    smoothTurn(0.3, start_color);
    //CASE 2
    move(0.3, 61, false, false);
    //CASE 3
    smoothTurn(0.3, start_color);
    //CASE 4-5
  }
}

int detect_whistle(void) {
    int valueIn = analogRead(PIN_WHISTLE);
    return((valueIn >= 400) ? 1 : 0);
}