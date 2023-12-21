#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4


void initRGB()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
}

void initButtons()
{
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

bool is_on = false;
int cur_led = LED_RED;

void setup()
{
  initRGB();
  initButtons();
}

void loop()
{
  if (digitalRead(RED_BUTTON) == LOW)
  {
    if (!is_on)
    {
      digitalWrite(cur_led, HIGH);
    }
    else{
      digitalWrite(cur_led, LOW);
    }
    is_on = !is_on;
  }

  if (digitalRead(GREEN_BUTTON) == LOW)
  {
    if (is_on == true)
    {
      if (cur_led == LED_RED)
      {
        digitalWrite(LED_GREEN, HIGH);
        cur_led = LED_GREEN;
        digitalWrite(LED_RED, LOW);
      }
      else if (cur_led == LED_GREEN)
      {
        digitalWrite(LED_BLUE, HIGH);
        cur_led = LED_BLUE;
        digitalWrite(LED_GREEN, LOW);
      }
      else
      {
        digitalWrite(LED_RED, HIGH);
        cur_led = LED_RED;
        digitalWrite(LED_BLUE, LOW);
      }
    }
    else
    {
      if (cur_led == LED_RED)
      {
        cur_led = LED_GREEN;
      }
      else if (cur_led == LED_GREEN)
      {
        cur_led = LED_BLUE;
      }
      else
      {
        cur_led = LED_RED;
      }
    }
  }
}