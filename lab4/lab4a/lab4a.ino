#define RED_BUTTON 2
#define GREEN_BUTTON 4
#define DEBOUNCE_PERIOD 10UL

void initButtons()
{
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void setup()
{
  initButtons();
  Serial.begin(9600);
}


void loop()
{
  Serial.println(String(digitalRead(RED_BUTTON)) + " " + String(digitalRead(GREEN_BUTTON)));
}