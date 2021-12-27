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
char auth[] = "mCxqHTioIfPdmzwvaLYa4M9sSm8QN1Gz";
char ssid[] = "Laras";
char pass[] = "29031969";
//char ssid[] = "RED";
//char pass[] = "namakamu";
WidgetMap myMap(V1);
float lat,latM=0;
float lon,lonM=0; 
float speed; 
//alarm
#define alarm 26
#define vAlarm V0
bool ALARM = 0;
#define tAlarm 25
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
  digitalWrite(confirm, HIGH);
  delay(150);
  digitalWrite(confirm, LOW);
  delay(100);
  digitalWrite(confirm, HIGH);
  delay(150);
  for(int i=0;i<5;i++){
    digitalWrite(confirm, HIGH);
    delay(100);
    digitalWrite(confirm, LOW);
    delay(900);
  }
}
BLYNK_WRITE(V0){
  ALARM=param.asInt();
}
BLYNK_WRITE(V2){
  GANTI = param.asInt();
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
  }
  //mengecek kemalingan
  if(!giveAcc){
    if(lat>latM+0.002 || lat<latM-0.002) ALARM = true;
    if(lon>lonM+0.002 || lon<lonM-0.002) ALARM = true;
    if(speed > 12) ALARM = true;
    if(digitalRead(tAlarm)) ALARM = true;
  }
  //alarm control 
  if(digitalRead(ALARM)){
    Blynk.virtualWrite(V0,HIGH);
    Blynk.syncVirtual(V0);
    digitalWrite(2, HIGH);
  }
  if(digitalRead(!ALARM)){
    Blynk.virtualWrite(V0,LOW);
    Blynk.syncVirtual(V0);
    digitalWrite(2, LOW);
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
