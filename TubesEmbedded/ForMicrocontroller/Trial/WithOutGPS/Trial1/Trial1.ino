#include <TinyGPS++.h>
#define RXD2 16
#define TXD2 17
#define Sein 14
#define Klakson 15
HardwareSerial neogps(1);

TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(2000);
  pinMode(Sein, OUTPUT);
  pinMode(Klakson, Output);
  digitalWrite(Sein, LOW);
  digitalWrite(Klakson, LOW);
}

boolean Pencurian(boolean newData){
    if(newData) if(gps.speed.kmph() > 10.0)
    return true;
    else false;
}

void Alarm(){
    while(true){//replace with NFC card recognized
        digitalWrite(Sein, HIGH);
        digitalWrite(Klakson, HIGH);
        delay(1000);
        digitalWrite(Sein, LOW);
        digitalWrite(Klakson, LOW);
        delay(1000);
    }
}

void loop() {
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
    Serial.println(gps.satellites.value());
    Serial.print(F("Lat: "));
    Serial.println(gps.location.lat(),6);
    Serial.print(F("Lng: "));
    Serial.println(gps.location.lng(),6);
    Serial.print(F("Speed: "));
    Serial.println(gps.speed.kmph());
    Serial.print(F("ALT: "));
    Serial.println(gps.altitude.meters());
  }
  else{
    Serial.println("No Data"); 
  }
  if(Pencurian(newData)){

  }
}
