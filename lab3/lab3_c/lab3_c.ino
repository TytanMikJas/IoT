#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

unsigned long seconds = 0;
unsigned long millis_on_start = 0;
bool is_on = false;

void initButtons()
{
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void initDisplay()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(0);
}

bool second_passed()
{
  bool second_passed = false;

  if (millis() - (seconds*1000 + millis_on_start) >= 1000){
    seconds++;
    second_passed = true;
  }
  return second_passed;
}

void print_time()
{
    lcd.setCursor(0, 0);    
    lcd.print(seconds);
}

bool isButtonPressed(int buttonPin, int &debounced_button_state, int &previous_reading, unsigned long &last_change_time)
{
  bool isPressed = false;
  int current_reading = digitalRead(buttonPin);

  if (previous_reading != current_reading)
  {
    last_change_time = millis();
  }

  if (millis() - last_change_time > DEBOUNCE_PERIOD)
  {
    if (current_reading != debounced_button_state)
    {
      if (debounced_button_state == HIGH && current_reading == LOW)
      {
        isPressed = true;
      }
      debounced_button_state = current_reading;
    }
  }

  previous_reading = current_reading;

  return isPressed;
}

int green_button_state = HIGH;
int green_previous_reading = HIGH;
unsigned long green_last_change_time = 0UL;

int red_button_state = HIGH;
int red_previous_reading = HIGH;
unsigned long red_last_change_time = 0UL;

void setup() {
  initButtons();
  initDisplay();
}

void loop() {

  if(!is_on){
    if(isButtonPressed(GREEN_BUTTON, green_button_state, green_previous_reading, green_last_change_time))
    {
      is_on = true;
      millis_on_start = millis() - seconds*1000;
    }
  }
  else
  {
    if(second_passed())
    {
      print_time();
    }
    if(isButtonPressed(GREEN_BUTTON, green_button_state, green_previous_reading, green_last_change_time))
    {
      is_on = false;
    }
  }

  if (isButtonPressed(RED_BUTTON, red_button_state, red_previous_reading, red_last_change_time))
  {
    is_on = false;
    lcd.clear();
    seconds = 0;
    lcd.print(0);
  }
}
