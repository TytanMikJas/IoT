#include "RGBControler.h"

void RGBControler::init(int redPin, int greenPin, int bluePin)
{
  this->redPin = redPin;
  this->greenPin = greenPin;
  this->bluePin = bluePin;

  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, LOW);

  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, LOW);

  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, LOW);

  Serial.begin(9600);
}

void RGBControler::colorLED(String input)
{
  if (input.substring(0, 4).equals("rgb("))
  {
    short red = -1;
    short green = -1;
    short blue = -1;
    int endIndex = 4;
    String substring = "";

    input = input.substring(endIndex);
    endIndex = input.indexOf(',');
    substring = input.substring(0, endIndex);
    substring.trim();
    red = substring.toInt();

    input = input.substring(endIndex + 1);
    endIndex = input.indexOf(',');
    substring = input.substring(0, endIndex);
    substring.trim();
    green = substring.toInt();

    input = input.substring(endIndex + 1);
    endIndex = input.indexOf(')');
    substring = input.substring(0, endIndex);
    substring.trim();
    blue = substring.toInt();

    if (red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255)
    {
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);
    }
    else
    {
      Serial.println("Wrong input format, should be like: rgb(r,g,b) - r,g,b being integers between 0 and 255.");
    }
  }
  else
  {
    int colorIndex = getColorIndex(input);
    if (colorIndex != -1)
    {
      analogWrite(redPin, led_red_values[colorIndex]);
      analogWrite(greenPin, led_green_values[colorIndex]);
      analogWrite(bluePin, led_blue_values[colorIndex]);
    }
    else
    {
      Serial.println("Invalid input. Write rgb(r,g,b) or a valid color name.");
    }
  }
}

int RGBControler::getColorIndex(String color)
{
  if (color.equals("RED"))
    return RED - 1;
  else if (color.equals("GREEN"))
    return GREEN - 1;
  else if (color.equals("BLUE"))
    return BLUE - 1;
  else if (color.equals("YELLOW"))
    return YELLOW - 1;
  else if (color.equals("CYAN"))
    return CYAN - 1;
  else if (color.equals("MAGENTA"))
    return MAGENTA - 1;
  else if (color.equals("BLACK"))
    return BLACK - 1;
  else if (color.equals("WHITE"))
    return WHITE - 1;
  else
    return -1;
}
