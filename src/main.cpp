#include <Arduino.h>
#include <math.h>

//>> Define <<//
#define PIN_TOF A0
#define DATA_TIMEOUT 1000 // https://learn.digilentinc.com/Documents/407#:~:text=The%20main%20reason%20to%20use,passed%2C%20and%20then%20they%20reset.
////////////////

//>> Macros <<//
#define PRINT(x) Serial.print(x);
#define PRINTLN(x) Serial.println(x);
#define PRINT_COMMA Serial.print(',')
#define PRINT_BLACKSLASH Serial.print('\n')
#define WAIT(x) delay(x);
////////////////

unsigned int distanceTOF_mm = 0;
unsigned int correctionFactor = 17158;
float exponent_Factor = -0.797;

unsigned int avalogValue = 0;
unsigned long cmptData = 0;

void terminalDisplay(int cmpt, int value1, int value2);

void setup() {
  
  Serial.begin(9600);
  pinMode(PIN_TOF, INPUT);




  
}

void loop() {
  
  static int last_time = millis();
  int current_time = millis();

  if(current_time - last_time >= DATA_TIMEOUT){
    avalogValue = analogRead(PIN_TOF);
    distanceTOF_mm = (float) correctionFactor * pow((float) avalogValue, exponent_Factor);

    terminalDisplay(cmptData, avalogValue, distanceTOF_mm);
    cmptData ++;

    last_time = current_time;
  }
  else{
    cmptData = 0;
    last_time = current_time;
  }
}

void terminalDisplay(int cmpt, int value1, int value2){

      PRINT((String) cmpt);
      PRINT_COMMA;
      PRINT((String) value1);
      PRINT_COMMA;
      PRINT((String) value2);
      PRINT_BLACKSLASH;


}








