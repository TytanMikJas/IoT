#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3


void initRGB()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
}


void setup()
{
  initRGB();
  Serial.begin(9600);
  while (!Serial){}
  Serial.println(String("START"));
}

void loop()
{
  String command = Serial.readStringUntil('\n');
    if(command.equals("")){
      return;
  }

  Serial.println(command);
  command.trim();
  command.toLowerCase();


  if(command.substring(0, 4).equals("rgb(") && command.endsWith(")"))
  {
    short red = -1;
    short green = -1;
    short blue = -1;
    int endIndex = 4;
    String substring = "";

    command = command.substring(endIndex);
    endIndex = command.indexOf(',');
    substring = command.substring(0, endIndex);
    substring.trim();
    red = substring.toInt();

    command = command.substring(endIndex + 1);
    endIndex = command.indexOf(',');
    substring = command.substring(0, endIndex);
    substring.trim();
    green = substring.toInt();

    command = command.substring(endIndex + 1);
    endIndex = command.indexOf(')');
    substring = command.substring(0, endIndex);
    substring.trim();
    blue = substring.toInt();

    if(red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255)
    {
      analogWrite(LED_RED, red);
      analogWrite(LED_GREEN, green);
      analogWrite(LED_BLUE, blue);
    }
    else{
      Serial.println("write rgb(r,g,b)");
    }
  }
  else{
    Serial.println("write rgb(r,g,b)");
  }
}