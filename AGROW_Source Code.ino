#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "oneplus";
char wifiPassword[] = "umer12345";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "ab2efad0-4b6e-11ea-b73d-1be39589c6b2";
char password[] = "fd0295ebad1ccef24f659654f60cbbd8c7b23238";
char clientID[] = "cd0082f0-4b6e-11ea-8221-599f77add412";

unsigned long lastMillis = 0;
#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float h,t,f,hic,hif;

void setup() {
Serial.begin(9600);
Cayenne.begin(username, password, clientID, ssid, wifiPassword);
pinMode(D0,OUTPUT);
pinMode(D1,OUTPUT);
pinMode(D2,OUTPUT);
pinMode(D3,OUTPUT);
pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
Cayenne.loop();
if (millis() - lastMillis > 10000) 
{
lastMillis = millis();
Cayenne.virtualWrite(0, h);
//Cayenne.virtualWrite(1, t);
Cayenne.celsiusWrite(1, t);
Cayenne.virtualWrite(2, hic);
}
//h = dht.readHumidity();
h = analogRead(A0);
//t = dht.readTemperature();
t=random(15,35);
f = dht.readTemperature(true);
//if (isnan(h) || isnan(t) || isnan(f)) {
//Serial.println("Failed to read from DHT sensor!");
//return;
//}

//hif = dht.computeHeatIndex(f, h);
hic = random(200,250);
//hic = dht.computeHeatIndex(t, h, false);
hic = random(150,200);

Serial.print("Humidity: ");
Serial.print(h);

Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);

Serial.print(" *C ");
Serial.print(f);
Serial.print(" *F\t");
Serial.print("Heat index: ");
Serial.print(hic);

Serial.print(" *C ");
Serial.print(hif);
Serial.println(" *F");

// Serial.println(" Output at 0");
//Serial.print(D0);
}

CAYENNE_IN_DEFAULT()
{
CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s" , request.channel, getValue.getId(), getValue.asString());
//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");

int iq=getValue.asInt();
Serial.println(" Output at 0");
Serial.print(D0);
Serial.println("Control Achieved");
}

CAYENNE_IN(3)
{
int currentValue=getValue.asInt();
if(currentValue==1){
digitalWrite(D0,HIGH);
}

else{
digitalWrite(D0,LOW);
}       }
CAYENNE_IN(4)
{
int currentValue=getValue.asInt();
// Serial.println("4 working");
if (currentValue == 0) {
Serial.println("Valve turned OFF");
}
else if (currentValue == 1) { // 60 <= temperature < 70
Serial.println("Valve turned ON");
}
else {
Serial.println("I don't know what's going on.");
}

Serial.print(currentValue);
if(currentValue==1){
digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
// but actually the LED is on; this is because
// it is active low on the ESP-01)
//delay(5000);                      // Wait for a second
//digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
//delay(5000);
}


Else
{
digitalWrite(D1,LOW);
}       }

CAYENNE_IN(5)
{
int currentValue=getValue.asInt();
if(currentValue==1){
digitalWrite(D2,HIGH);
}
else{
digitalWrite(D2,LOW);
}       }
CAYENNE_IN(6)
{
int currentValue=getValue.asInt();
if(currentValue==1){
digitalWrite(D3,HIGH);
}
else{
digitalWrite(D3,LOW);
}

}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
