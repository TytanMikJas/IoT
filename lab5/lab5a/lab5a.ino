#define LED_RED 6
#define LED_BLUE 3
#define ENCODER1 A2
#define ENCODER2 A3

void setup()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(ENCODER1, INPUT_PULLUP);
  pinMode(ENCODER2, INPUT_PULLUP);

  Serial.begin(9600);  
  Serial.println("");
}

int prevEN1 = 1;
int prevEN2 = 1;

void loop()
{
  int en1 = digitalRead(ENCODER1);
  int en2 = digitalRead(ENCODER2);

  if (en1 != prevEN1)
  {
    Serial.print("Encoder 1: ");
    Serial.println(en1);
    prevEN1 = en1;
  }

  if (en2 != prevEN2)
  {
    Serial.print("Encoder 2: ");
    Serial.println(en2);
    prevEN2 = en2;
  }
}