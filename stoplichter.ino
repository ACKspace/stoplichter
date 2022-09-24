//#define DEBUG

// LED sleeve 21x7 circular display offset (4 RGB pixels)
#define OFFSET 12

// Also used as AP name, password "ackspace"
char hostname[] = "Stoplichter";

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Artnet.h> // https://github.com/hideakitai/ArtNet/tree/0.1.10

ESP8266WebServer    server(80);
ArtnetWiFiReceiver artnet;

struct settings {
  char ssid[30];
  char password[30];
} user_wifi = {};

// ESP12E pins in RGB clockwise order (from the top) starting near the serial port
byte pins[] =   {5,4,0,2,15,13,12,14,16};

void setup() {
#ifdef DEBUG
  Serial.begin( 115200 );
  Serial.println( F("initializing" ) );
#endif

  for ( byte n = 0; n < sizeof( pins ) / sizeof( byte ); ++n )
    pinMode( pins[n], OUTPUT );

  EEPROM.begin(sizeof(struct settings) );
  EEPROM.get( 0, user_wifi );
   
  WiFi.mode(WIFI_STA);
  wifi_station_set_hostname( hostname );
  WiFi.begin(user_wifi.ssid, user_wifi.password);
  
  byte tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    if (tries++ > 30) {
      WiFi.mode(WIFI_AP);
      WiFi.softAP( hostname, F("ackspace"));
      break;
    }
  }

  server.on("/",  handlePortal);
  server.begin();

  artnet.begin();

  artnet.subscribe(0/*universe*/, callback);
    
#ifdef DEBUG
  Serial.printf("Initialized.\nIP %s\n", WiFi.localIP().toString().c_str());
#endif
  
}

byte incomingPacket[600];

void loop() {

  server.handleClient();
  artnet.parse(); // check if artnet packet has come and execute callback
}

void callback(uint8_t* data, uint16_t size)
{
#ifdef DEBUG  
  Serial.print(", size = ");
  Serial.println(size);
#endif

  if ( size < OFFSET + sizeof( pins ) )
    return;

  if ( size > sizeof( pins ) )
    size = sizeof( pins );

  // Set the actual LEDs
  for ( byte n = 0; n < size; ++n )
    analogWrite( pins[n], data[n+OFFSET] );
}

void handlePortal() {

  if (server.method() == HTTP_POST) {

    strncpy(user_wifi.ssid,     server.arg("ssid").c_str(),     sizeof(user_wifi.ssid) );
    strncpy(user_wifi.password, server.arg("password").c_str(), sizeof(user_wifi.password) );
    user_wifi.ssid[server.arg("ssid").length()] = user_wifi.password[server.arg("password").length()] = '\0';
    EEPROM.put(0, user_wifi);
    EEPROM.commit();

    server.send(200,   "text/html",  "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Wifi Setup</title><style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}</style> </head> <body><main class='form-signin'> <h1>Wifi Setup</h1> <br/> <p>Your settings have been saved successfully!<br />Please restart the device.</p></main></body></html>" );
  } else {
    server.send(200,   "text/html", "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Wifi Setup</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><main class='form-signin'> <form action='/' method='post'> <h1 class=''>Wifi Setup</h1><br/><div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid'> </div><div class='form-floating'><br/><label>Password</label><input type='password' class='form-control' name='password'></div><br/><br/><button type='submit'>Save</button><p style='text-align: right'><a href='https://ack.space' style='color: #32C5FF'>ack.space</a></p></form></main> </body></html>" );
  }
}
