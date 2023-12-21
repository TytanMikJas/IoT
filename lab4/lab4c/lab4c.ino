#include <LiquidCrystal_I2C.h>

#define POTENTIOMETER A0

LiquidCrystal_I2C lcd(0x27, 16, 2);

int value;
int valueLen;

float voltage;
int voltageLen;

int min;
int minLen;

int max;
int maxLen;


void initDisplay()
{
  lcd.init();
  lcd.backlight();
}

void initWrite()
{
  lcd.setCursor(0, 0); 
  lcd.print(String("ADC:"));

  lcd.setCursor(8, 0); 
  lcd.print(String("V:"));

  lcd.setCursor(0, 1); 
  lcd.print(String("MIN:"));

  lcd.setCursor(8, 1); 
  lcd.print(String("MAX:"));

  value = analogRead(A0);
  valueLen = String(value).length();
  lcd.setCursor(4, 0);
  lcd.print(value);

  voltage = value * (5.0 / 1023.0);
  voltageLen = String(voltage).length();
  lcd.setCursor(10, 0);
  lcd.print(voltage);

  min = analogRead(A0);
  minLen = String(min).length();
  lcd.setCursor(4, 1);
  lcd.print(min);

  max = analogRead(A0);
  maxLen = String(max).length();
  lcd.setCursor(12, 1);
  lcd.print(max);
}

void setup()
{
  initDisplay();
  initWrite();
}


void loop()
{
  // reading sensor
  value = analogRead(A0);

  //clearing if sensor length changed + printing sensor value
  if (String(value).length() != valueLen)
  {
    lcd.setCursor(4, 0);
    lcd.print("    "); 
    valueLen = String(value).length();
  }
  lcd.setCursor(4, 0);
  lcd.print(value);

  // voltage len is const so we don't clear + printing voltage value
  voltage = value * (5.0 / 1023.0);
  lcd.setCursor(10, 0);
  lcd.print(voltage);

  // if min value changed then print + if length changed then clear
  if (value < min)
  {
    min = value;
    if (String(min).length() != minLen)
    {
      lcd.setCursor(4, 1);
      lcd.print("    ");
    }
    lcd.setCursor(4, 1);
    lcd.print(min);
  } 

  // if max value changed then print + if length changed then clear przetwornik anaogowo cyfrowy
  if (value > max)
  {
    max = value;
    if (String(max).length() != minLen)
    {
      lcd.setCursor(12, 1);
      lcd.print("    "); 
    }
    lcd.setCursor(12, 1);
    lcd.print(max);
  } 

  delay(500);
}