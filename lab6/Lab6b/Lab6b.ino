#include "RGBControler.h"

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

RGBControler ledControler;

void setup() 
{
  Serial.begin(9600);
  Serial.println("START");
  ledControler.init(LED_RED, LED_GREEN, LED_BLUE);
}

void loop() {
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    ledControler.colorLED(input);
  }
}