#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3


LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(A1);
DallasTemperature tempSensors(&oneWire);
float tempIn; 
float tempOut;
float minTemp;
float maxTemp;

byte celcius[8] = {
  B01000,
  B10100,
  B01000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};


void setupLED()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
}

void setupLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void shineTemp(float temp)
{
  if (temp >= 25.0) 
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  }
  else if (temp >= 24.0) 
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
  }
  else 
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
  }
}


void setup()
{
  setupLED();
  setupLCD();

  tempSensors.begin();
  lcd.createChar(0, celcius);

  float tempOut = tempSensors.getTempCByIndex(0);
  float minTemp = tempOut;
  float maxTemp = tempOut;
}

void loop()
{
  tempSensors.requestTemperatures();
  float tempIn = tempSensors.getTempCByIndex(1);
  float tempOut = tempSensors.getTempCByIndex(0);

// print INSIDE temp
  char buffer[40];
  sprintf(buffer, "IN:%4s", String(tempIn, 1).c_str());
  lcd.setCursor(0, 0);
  lcd.print(buffer);

// print Celcius
  lcd.setCursor(7, 0);
  lcd.write(byte(0));

// print current temp outside
  sprintf(buffer, "OUT:%4s",String(tempOut, 1).c_str());
  lcd.setCursor(8, 0);
  lcd.print(buffer);

// update min and max temps
  minTemp = min(minTemp, tempOut);
  maxTemp = max(maxTemp, tempOut);

// print min temp outside
  sprintf(buffer, "MIN:%2s", String(minTemp, 0).c_str());
  lcd.setCursor(0, 1);
  lcd.print(buffer);
  
// print Celcius
  lcd.setCursor(6, 1);
  lcd.write(byte(0));

// print MAX temp outside
  sprintf(buffer, "  MAX:%2s", String(maxTemp, 0).c_str());
  lcd.setCursor(7, 1);
  lcd.print(buffer);

// print Celcius
  lcd.setCursor(15, 1);
  lcd.write(byte(0));

// display LED
  shineTemp(tempOut);
}