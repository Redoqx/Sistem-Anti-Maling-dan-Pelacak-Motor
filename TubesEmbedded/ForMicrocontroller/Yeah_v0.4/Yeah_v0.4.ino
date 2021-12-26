//RFID
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 22
#define SS_PIN 21
#define RST_User 34
MFRC522 mfrc522(SS_PIN, RST_PIN);
//UserIdentify
String UID, User = "No data!";
bool giveAcc;
#define RST_User 27 //Ganti user
#define Curr_eng 14 //cek kondisi mesin (hidup/koid?)
#define Acc_eng 12 //beri akses mesin
//GPS
#include <TinyGPS++.h>
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);
TinyGPSPlus gps;

void setup(){
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("RFID Ready"));

  pinMode(RST_User, INPUT);
  pinMode(Curr_eng, INPUT);
  pinMode(Acc_eng, OUTPUT);
  digitalWrite(Acc_eng, LOW);
  giveAcc = 0;  

  //GPS
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(2000);
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
}
bool userConfirmed(){
  getUID();
  return (User == UID);
}

void loop(){
  if(digitalRead(RST_User)) gantiKartu();
  if(userConfirmed()){
    digitalWrite(Acc_eng, HIGH);
    Serial.println(F("Silahkan nyalakan mesin"));
    UID = "";
    delay(5000);
  }
  if(!digitalRead(Curr_eng)){
    digitalWrite(Acc_eng, LOW);
    Serial.println(F("Akses ditutup!"));
  }

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
}
