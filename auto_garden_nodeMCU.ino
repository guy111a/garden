#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"


// 0x40 humid
// 0x3F LCD

ESP8266WebServer server(80);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

// void handleRoot();

int ledPin = D0;
int relayPin = D8 ;
float airHumid, airTemp ;
int soilHumid = A0 ;
int output_value ;

bool ledStatus = 0 ;
String text ;

IPAddress arduino_ip ( 10.,  0,   0,  99);
IPAddress dns_ip     (  194,   90,   1,   5);
IPAddress gateway_ip ( 10,  0,   0,   138);
IPAddress subnet_mask(255, 255, 255,   0);


void setup()
{

  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT) ;

  digitalWrite(ledPin, HIGH);
  digitalWrite(relayPin, HIGH);

  Serial.begin(115200);

  if (!htu.begin())
  {
    Serial.println("Couldn't find sensor!");
    while (1);
  }


  WiFi.config(arduino_ip, gateway_ip, subnet_mask);
  WiFi.begin("Ezhik", "sushimushi"); //Connect to the WiFi network
  Serial.println("Connecting to WIFI …");
  while (WiFi.status() != WL_CONNECTED) { //Wait for connection

    delay(500);
    Serial.print(".");

  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); //Print the local IP

  //  server.send(200, "text/html", "Water off <p><a href='on'>ON</><p><p><a href='off'>OFF</><p>");
  server.on("/on", turnOn);       //Associate the handler function to the path
  server.on("/off", turnOff);      //Associate the handler function to the path
  server.on("/", handleRoot);

  server.begin(99); //Start the server
  Serial.println("Server listening on port 99");
}

void handleRoot()
{
  text =  "<meta http-equiv='refresh' content='5'/> <body><h1><p><a href='on'>ON</><p><p><a href='off'>OFF</a><p><p>Water status : " ;
  text += ledStatus ;
  text += "<p>air humid is " ;
  text += airHumid ;
  text += "<p>air temp is " ;
  text += airTemp ;
  text += "<p>soil humid : " ;
  text += output_value ;
  text += "<p></h1></body>"  ;
  server.send(200, "text/html", text ) ;
}



void turnOn()
{
  digitalWrite(ledPin, HIGH);
  digitalWrite(relayPin, HIGH);
  text = "<meta http-equiv='refresh' content='5'/> <body><h1><font color='green'>Water is on <font color='black'><p><p><p><a href='off'>OFF</a><p><p>Water status : " ;
  text += ledStatus ;
  text += "<p>air humid : " ;
  text += airHumid ;
  text += "<p>air temp : " ;
  text += airTemp ;
  text += "<p>soil humid : " ;
  text += output_value ;
  text += "<p></h1></body>"  ;
  server.send(200, "text/html", text);
  Serial.println("turning on") ;
  ledStatus = true ;
}

void turnOff()
{
  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, LOW);
  text = "<meta http-equiv='refresh' content='5'/> <body><h1><font color='red'>Water is off  <font color='black'><p><p><p><a href='on'>ON</a><p><p>Water status : " ;
  text += ledStatus ;
  text += "<p>air humid : " ;
  text += airHumid ;
  text += "<p>air temp : " ;
  text += airTemp ;
  text += "<p>soil humid : " ;
  text += output_value ;
  text += "<p></h1></body>"  ;
  server.send(200, "text/html", text);
  Serial.println("turning off") ;
  ledStatus = false ;
}



void loop()
{

  server.handleClient();
  airTemp = htu.readTemperature() ;
  airHumid  = htu.readHumidity() ;
  output_value = analogRead(soilHumid);
  output_value = map(output_value, 550, 0, 0, 100);
  delay(500) ;

}


