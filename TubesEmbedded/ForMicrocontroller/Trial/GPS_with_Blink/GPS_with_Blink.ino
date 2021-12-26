/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  Output any data on Map widget!

  App project setup:
    Map widget on V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32_SSL.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "mCxqHTioIfPdmzwvaLYa4M9sSm8QN1Gz";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Laras";
//char pass[] = "29031969";
char ssid[] = "RED";
char pass[] = "namakamu";


WidgetMap myMap(V1);

#include <TinyGPS++.h>
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);
TinyGPSPlus gps;
float lat;
float lon;

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  // If you want to remove all points:
  //myMap.clear();

  int index = 0;
  lat = 51.5074;
  lon = 0.1278;
  myMap.location(index, lat, lon, "value");
  
  //GPS
//  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(2000);
}

void loop()
{

  //GPS
  boolean newData = false;
  for(unsigned long start = millis();millis()-start<1000;){
    while(neogps.available()){
      if(gps.encode(neogps.read())){
        newData = true;
      }
    }
  }

  if(newData){
    newData = false;
    lat = gps.location.lat();
    lon = gps.location.lng();
    Serial.println(gps.satellites.value());
    Serial.print(F("Lat: "));
    Serial.println(lat,6);
    Serial.print(F("Lng: "));
    Serial.println(lon,6);
    Serial.print(F("Speed: "));
    Serial.println(gps.speed.kmph());
    Serial.print(F("ALT: "));
    Serial.println(gps.altitude.meters());
  }
  else{
    Serial.println("No Data"); 
  }
  int index = 1;
  myMap.location(index, lat, lon, "value");
  Blynk.run();
}
