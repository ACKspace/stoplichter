/*
 * Write this to the ESP
 */
/*
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "";
const char* password = "";

const char* host = "ackspace.nl";
const int httpsPort = 443;
String url = "/spaceAPI/";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop()
{
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;

  // Check connection, if fail return error and start loop again
  if (client.connect(host, httpsPort)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: stoplichter\r\n" +
                 "Connection: close\r\n\r\n");

    String statusCode = client.readString();
    int startPos = statusCode.indexOf('{');
    statusCode.remove(0, startPos); // Some header stuff is at the beginning, remove that shite

    int endPos = statusCode.indexOf(",\"message\"");
    statusCode.remove(endPos); // Make sure not too much of the JSON is handled, keep memory usage low

    int statusPos = statusCode.indexOf("\"open\":");
    statusCode.remove(0, statusPos + 7); // Only keep the "open" return value

    if (statusCode == "true")
    {
      Serial.print("o");
    }
    else if (statusCode == "false")
    {
      Serial.print("c");
    }
  }

  delay(10000);
}
*/
