#ifndef RGBCONTROLER_H_
#define RGBCONTROLER_H_

#include "Arduino.h"

#define RED 1
#define GREEN 2
#define BLUE 3

#define YELLOW 4
#define CYAN 5
#define MAGENTA 6
#define BLACK 7
#define WHITE 8

class RGBControler
{
public:
  void init(int redPin, int greenPin, int bluePin);
  void colorLED(String input);

private:
  const int led_red_values[8] = {255, 0, 0, 255, 0, 255, 0, 255};
  const int led_green_values[8] = {0, 255, 0, 255, 255, 0, 0, 255};
  const int led_blue_values[8] = {0, 0, 255, 0, 255, 255, 0, 255};
  int redPin;
  int greenPin;
  int bluePin;

  int getColorIndex(String color);
};

#endif
