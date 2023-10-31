#include <Arduino.h>
#include <LibRobus.h>
#include <turn_color.hpp>
#include <robotMovement.hpp>

#define PIN_WHISTLE A0
int detect_whistle(void);
int active;
int current_zone = 0;

void setup() {
    Serial.begin(9600);
    pinMode(PIN_WHISTLE, INPUT);
    active = 0;
    initialiser_color();
}

void loop() {
  if(detect_whistle() == 1)
    active = 1;
  char start_color = Floor_Color();
  smoothTurn(0.3, start_color);
  move(0.3, 61, false, false);
  smoothTurn(0.3, start_color);
  
}

int detect_whistle(void) {
    int valueIn = analogRead(PIN_WHISTLE);
    return((valueIn >= 400) ? 1 : 0);
}