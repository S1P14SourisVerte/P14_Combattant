#include <Arduino.h>
#include <LibRobus.h>

#define PIN_WHISTLE A0
int detect_whistle(void);
int active;
int current_zone = 0;

void setup() {
    Serial.begin(9600);
    pinMode(PIN_WHISTLE, INPUT);
    active = 0;
}

void loop() {
  if(detect_whistle() == 1)
    active = 1;
  
}

int detect_whistle(void) {
    int valueIn = analogRead(PIN_WHISTLE);
    return((valueIn >= 400) ? 1 : 0);
}