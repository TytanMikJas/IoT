#include <LiquidCrystal_I2C.h>
#include <util/atomic.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define ENCODER1 A2
#define ENCODER2 A3

#define RED_BUTTON 2

#define DEBOUNCING_PERIOD 100
#define DEBOUNCE_PERIOD 10UL

LiquidCrystal_I2C lcd(0x27, 16, 2);

// printing result of intensity change on given LED
void printResults(int val)
{
  char buffer[40];
  sprintf(buffer, "Encoder: %3d", val);
  lcd.setCursor(2, 0);
  lcd.print(buffer);
}

// Calling printResult() to display data + change the intensity on given led
void changeIntensity(int led, int intensity)
{
  printResults(intensity);
  analogWrite(led, intensity);
}

// Displaying menu based on chosen LED
void menu(int value)
{
  if (value + 1 == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println(">LED RED");
    lcd.setCursor(1, 1);
    lcd.println("LED GREEN");
  }
  else if (value + 1 == 2)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.println("LED RED");
    lcd.setCursor(0, 1);
    lcd.println(">LED GREEN");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.println("LED GREEN");
    lcd.setCursor(0, 1);
    lcd.println(">LED BLUE");
  }
}

bool isRedButtonPressed() 
{
  static int debounced_button_state = HIGH;
  static int previous_reading = HIGH;
  static unsigned long last_change_time = 0UL;
  bool isPressed = false;

  int current_reading = digitalRead(RED_BUTTON);

  if (previous_reading != current_reading) 
  {
    last_change_time = millis();
  }

  if (millis() - last_change_time > DEBOUNCE_PERIOD) {
    if (current_reading != debounced_button_state) {
      if (debounced_button_state == HIGH && current_reading == LOW) {
        isPressed = true;
      }
      debounced_button_state = current_reading;
    }
  }

  previous_reading = current_reading;

  return isPressed;
}

// Choosing LED option
int ledOption(int option)
{
  if (option == 0)
  {
    return LED_RED;
  }
  else if (option == 1)
  {
    return LED_GREEN;    
  }
  else
  {
    return LED_BLUE;
  }
}

// Settup
void setup()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, 0);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, 0);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, 0);

  pinMode(ENCODER1, INPUT_PULLUP);
  pinMode(ENCODER2, INPUT_PULLUP);

  pinMode(RED_BUTTON, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  menu(0);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10);
}

// to know if we should display menu or values
volatile bool menuMode = true;
volatile int encoder1 = HIGH;
volatile int encoder2 = HIGH;
volatile unsigned long encoderTimestamp = 0UL;

// setting up the encoder
ISR(PCINT1_vect) {
  encoder1 = digitalRead(ENCODER1);
  encoder2 = digitalRead(ENCODER2);
  encoderTimestamp = millis();
}

int chosen_option;
int encoderValue = 0;
int lastEn1 = LOW;
bool menuDisplayed = false;
unsigned long lastChangeTimestamp = 0UL;

void loop()
{
  int en1;
  int en2;
  unsigned long timestamp;

  // ATOMIC - the reding can not be interupted
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    en1 = encoder1;
    en2 = encoder2;
    timestamp = encoderTimestamp;
  }

  // if we are in the mode that displays menu
  if (menuMode) 
  {
    // we check wheather the encoder CLK was changed
    if (en1 == LOW && timestamp > lastChangeTimestamp + DEBOUNCING_PERIOD) {
      if (en2 == HIGH) {
        if (encoderValue < 2)
          encoderValue += 1;
      } else {
        if (encoderValue > 0)
          encoderValue -= 1;
      }
      lastChangeTimestamp = timestamp;
      menuDisplayed = false;
    }
    // we display menu with our chosen LED read from encoder
    if (!menuDisplayed)
    {
      menu(encoderValue);
      menuDisplayed = true;
    }
    lastEn1 = en1;
  } 

  // for chosing the option
  if (isRedButtonPressed()) 
  {
    if (menuMode)
    {
      menuMode = false;
      chosen_option = encoderValue;
    }
    else
    {
      menuMode = true;
    }
    lcd.clear();
    encoderValue = 0;
  }

  // if we are not in menu then we print the intensity and we can change it by encoder
  if (!menuMode)
  {
    if (en1 == LOW && timestamp > lastChangeTimestamp + DEBOUNCING_PERIOD) 
    {
      if (en2 == HIGH) 
      {
        if (encoderValue < 255)
          encoderValue += 15;
      } 
      else 
      {
        if (encoderValue > 0)
          encoderValue -= 15;
      }
      lastChangeTimestamp = timestamp;
    }
    changeIntensity(ledOption(chosen_option), encoderValue);
    lastEn1 = en1;
    menuDisplayed = false;
  }


}
