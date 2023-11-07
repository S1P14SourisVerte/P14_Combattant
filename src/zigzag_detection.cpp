#include <Arduino.h>
#include <Librobus.h>

#define FRONT_SENSOR1 A1
#define FRONT_SENSOR2 0


// Code pour observer les obstacles en avant (test 1)
bool FrontObject1() {
  float distance = analogRead(FRONT_SENSOR1);
  return((distance > 400) ? true : false);
}

// Code pour observer les obstacles en avant (test 2)
bool FrontObject2() {
  float value = ROBUS_ReadIR(FRONT_SENSOR2);
  return((value > 400) ? true : false);
}


// Code pour faire la partie en zigzag
void Zigzag_Movement(){
  int i;

}
void setup(){
  BoardInit();
}

void loop(){
  int b;
}