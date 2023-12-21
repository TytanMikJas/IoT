#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

int intensity_red = 255;
int intensity_green = 1;
int intensity_blue = 1;
int current = LED_RED;

void setup()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, 255);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, 0);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, 0);
}


void loop() {
  if (current == LED_RED) {
    if (intensity_red == 0) current = LED_GREEN;
    intensity_red--;
    intensity_green++;
  } else if (current == LED_GREEN) {
    if (intensity_green == 0) current = LED_BLUE;
    intensity_green--;
    intensity_blue++;
  } else {
    if (intensity_blue == 0) current = LED_RED;
    intensity_blue--;
    intensity_red++;
  }

  delay(15);
  analogWrite(LED_RED, intensity_red);
  analogWrite(LED_GREEN, intensity_green);
  analogWrite(LED_BLUE, intensity_blue);
}
