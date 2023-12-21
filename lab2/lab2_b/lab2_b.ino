#define LED_GREEN 5

#define RED_BUTTON 2
#define GREEN_BUTTON 4


void initRGB()
{
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);
}

void initButtons()
{
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void setup()
{
  initRGB();
  initButtons();
}

int brightness = 255;

void loop() {

  if (digitalRead(GREEN_BUTTON) == LOW && brightness < 255)
  {
    brightness++;
  }

  if (digitalRead(RED_BUTTON) == LOW && brightness > 0)
  {
    brightness--;
  }

  analogWrite(LED_GREEN, brightness);
  delay(5);
}
