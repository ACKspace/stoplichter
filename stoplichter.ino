/*
   Write this script to the Arduino
*/

String statusCode = "u";

void setup()
{
  Serial1.begin(115200);

  // top light
  pinMode(5, OUTPUT); // red
  pinMode(6, OUTPUT); // green
  pinMode(7, OUTPUT); // blue

  // middle light
  pinMode(8, OUTPUT); // red
  pinMode(9, OUTPUT); // green
  pinMode(10, OUTPUT); // blue

  // bottom light
  pinMode(11, OUTPUT); // red
  pinMode(12, OUTPUT); // green
  pinMode(13, OUTPUT); // blue
}

void off()
{
  for (uint8_t n = 5; n < 13; ++n)
  {
    digitalWrite(n, LOW);
  }
}

void rainbow(int lightPos) // 1=top; 2=middle; 3=bottom
{
  for (int n = lightPos * 3 + 2; n < lightPos * 3 + 5; ++n) // Starting position for every light is lightPos*3+2
  {
    analogWrite(n, random(0, 255));
  }
}

void loop()
{
  String statusInput = Serial1.readString();

  if (statusInput == "o" || statusInput == "c" || statusInput == "u")
  {
    statusCode = statusInput;
  }

  off();

  int lightSelector;
  bool startRainbow = false;
  if (random(300) == 42)
  {
    startRainbow = true;
  }

  if (statusCode == "u")
  {
    lightSelector = 2;
    analogWrite(8, 255);
    analogWrite(9, 110);
  }
  else if (statusCode == "o")
  {
    lightSelector = 3;
    analogWrite(12, 255);
  }
  else if (statusCode == "c")
  {
    lightSelector = 1;
    analogWrite(5, 255);
  }

  if (startRainbow)
  {
    rainbow(lightSelector);
  }

  delay(1000);
}
