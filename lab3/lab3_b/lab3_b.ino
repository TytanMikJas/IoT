#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define BLINK_CHANGE_PREIOD_RED 900UL
#define BLINK_CHANGE_PREIOD_GREEN 1000UL
#define BLINK_CHANGE_PREIOD_BLUE 1100UL

void initRGB()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, HIGH);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
}

void setup()
{
  initRGB();
}

void blinkLed(const int LED, const unsigned long blinkChangePeriod, int& ledState, unsigned long& lastBlinkTime)
{
  if (millis() - lastBlinkTime >= blinkChangePeriod){
    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }
    digitalWrite(LED, ledState);
    lastBlinkTime += blinkChangePeriod;
  }
}

int ledStateRed = LOW;
int ledStateGreen = LOW;
int ledStateBlue = LOW;

unsigned long lastBlinkTimeRed = 0UL;
unsigned long lastBlinkTimeGreen = 0UL;
unsigned long lastBlinkTimeBlue = 0UL;

void loop()
{
  blinkLed(LED_RED, BLINK_CHANGE_PREIOD_RED, ledStateRed, lastBlinkTimeRed);
  blinkLed(LED_GREEN, BLINK_CHANGE_PREIOD_GREEN, ledStateGreen, lastBlinkTimeGreen);
  blinkLed(LED_BLUE, BLINK_CHANGE_PREIOD_BLUE, ledStateBlue, lastBlinkTimeBlue);  
}