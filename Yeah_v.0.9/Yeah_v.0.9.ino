//RFID
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 22
#define SS_PIN 21
MFRC522 mfrc522(SS_PIN, RST_PIN);

//UserIdentify
String UID, User = "No data!";
bool giveAcc;
bool GANTI=0;
#define RST_User 27 //Ganti user
#define Curr_eng 14 //cek kondisi mesin (hidup/koid?)
#define Acc_eng 12 //beri akses mesin

//GPS
#include <TinyGPS++.h>
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);
TinyGPSPlus gps;
//Blynk
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32_SSL.h>
char auth[] = "";//change with your token from Blynk
char ssid[] = "";//change with your Wi-Fi SSID 
char pass[] = "";//chaneg with your Wi-Fi password
WidgetMap myMap(V1);
WidgetLED Al_LED(V3);
WidgetLED AC_LED(V4);//ACC LED
float lat,latM=0;
float lon,lonM=0; 
float speed; 
//alarm
#define alarm 26
#define vAlarm V0
bool ALARM = 0;
#define tAlarm 33
//confirm
#define confirm 2

void setup(){
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("RFID Ready"));

  pinMode(RST_User, INPUT);
  pinMode(Curr_eng, INPUT);
  pinMode(Acc_eng, OUTPUT);
  pinMode(alarm, OUTPUT);
  pinMode(confirm, OUTPUT);
  pinMode(tAlarm, INPUT);
  digitalWrite(Acc_eng, LOW);
  giveAcc = 0;  

  //GPS
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(2000);

  //Blynk  
  Blynk.begin(auth, ssid, pass);
  myMap.clear();
  Blynk.setProperty(V3,"color","#FF0000");
  Blynk.setProperty(V4,"color","#00FF00");
}

void getUID(){
  if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;
  UID="";
  Serial.print(F("Card UID: "));
  for(byte i=0; i<mfrc522.uid.size; i++)
  UID+=(mfrc522.uid.uidByte[i]<0x10 ? "0" : "") +
  String(mfrc522.uid.uidByte[i], HEX) +
  (i != (mfrc522.uid.size -1) ? ":" : "");
  UID.toUpperCase();
  Serial.println(UID);
  mfrc522.PICC_HaltA();
  delay(10);
}
void gantiKartu(){
  getUID();
  User = UID;
  Serial.println(F("User card has changed to this UID"));
  GANTI = 0;
}
bool userConfirmed(){
  getUID();
  return (User == UID);
}
void youCan(){
  for(int i=0;i<5;i++){
    digitalWrite(confirm, HIGH);
    delay(100);
    digitalWrite(confirm, LOW);
    delay(900);
  }
}
BLYNK_WRITE(V0){
  ALARM=param.asInt();
  if(ALARM)Serial.println("You Actived the alarm from Blynk");
}
void loop(){
  //ganti kartu
  if(digitalRead(RST_User)||GANTI) gantiKartu();
  //mengidentifiasi pengguna
  if(userConfirmed()){
    digitalWrite(Acc_eng, HIGH);
    myMap.clear();
    giveAcc = 1;
    latM=0;
    lonM=0;
    Serial.println(F("Silahkan nyalakan mesin"));
    AC_LED.on();
    youCan();
    UID = "";
  }
  //mengcek kondisi mesin
  if(!digitalRead(Curr_eng)){
    giveAcc = 0;
    latM=lat;
    lonM=lon;
    myMap.location(1, latM, lonM, "value");
    digitalWrite(Acc_eng, LOW);
    Serial.println(F("Akses ditutup!"));
    AC_LED.off();
  }
  //mengecek kemalingan
  if(!giveAcc){
    if(speed > 12){ ALARM = true; Serial.println("Terdeteksi Pencurian!");}
    if(digitalRead(tAlarm)){ ALARM = true; Serial.println("Alarm actived from Embedded System!");}
  }
  //alarm control 
  if(ALARM){
    Blynk.virtualWrite(V0,HIGH);
    Blynk.syncVirtual(V0);
    digitalWrite(2, HIGH);
    Al_LED.on();
  }
  if(!ALARM){
    Blynk.virtualWrite(V0,LOW);
    Blynk.syncVirtual(V0);
    digitalWrite(2, LOW);
    Al_LED.off();
  }

  digitalWrite(alarm, ALARM);

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
    speed = gps.speed.kmph();
    Serial.println(gps.satellites.value());
    Serial.print(F("Lat: "));
    Serial.println(lat,6);
    Serial.print(F("Lng: "));
    Serial.println(lon,6);
    Serial.print(F("Speed: "));
    Serial.println(speed);
    Serial.print(F("ALT: "));
    Serial.println(gps.altitude.meters());
    myMap.location(0, lat, lon, "value");
  }
  else{
    Serial.println("No Data"); 
  }
  Blynk.run();
}
