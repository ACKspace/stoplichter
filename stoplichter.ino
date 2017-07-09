/*
 * Write this script to the Arduino
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

void loop()
{
  String statusInput = Serial1.readString();

  if (statusInput == "o" || statusInput == "c" || statusInput == "u")
  {
    statusCode = statusInput;
  }

  off();

  if (statusCode == "u")
  {
    analogWrite(8, 255);
    analogWrite(9, 110);
  }
  else if (statusCode == "o")
  {
    analogWrite(12, 255);
  }
  else if (statusCode == "c")
  {
    analogWrite(5, 255);
  }

  delay(1000);
}
