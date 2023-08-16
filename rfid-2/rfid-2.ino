#include <SoftwareSerial.h>
#include <AFMotor.h>
#include "SPI.h"
#include "MFRC522.h"
#include <DFRobotDFPlayerMini.h>
#include "VoiceRecognitionV3.h"

// RFID 부분
// 최종 핀 => SDA 53 | SCK 52 | MOSI 51 | MISO 50 | RST 49
#define SDA_PIN 53
#define RST_PIN 49
MFRC522 mfrc522(SDA_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int isKey() {
  // 만약 카드가 인식 되었다면
  if (mfrc522.PICC_IsNewCardPresent() & mfrc522.PICC_ReadCardSerial()) {

    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
      {
      Serial.println(F("Your tag is not of type MIFARE Classic."));
      return;
      }
    String rfid = "";
    for (byte i = 0; i < 4; i++) {
      rfid +=
      (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(mfrc522.uid.uidByte[i], HEX) +
      (i!=3 ? ":" : "");
    }
  
    Serial.print("Card key: ");
    Serial.println(rfid);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    if (rfid == "c0:8f:9e:25") {
      return START;
    }
    if (rfid == "e8:ea:cf:0d") {
      return OBJ_1;
    }
    if (rfid == "e8:a0:6f:0d") {
      return OBJ_2;
    }
    if (rfid == "fb:df:c7:22") {
      return OBJ_3;
    }
    else {
      return -1;
    }
    //delay(1000);
  }
  return -1;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  if (isKey() == START) {Serial.println("START");}
  if (isKey() == OBJ_1) {Serial.println("OBJ_1");}
  if (isKey() == OBJ_2) {Serial.println("OBJ_2");}
  if (isKey() == OBJ_3) {Serial.println("OBJ_3");}
  // put your main code here, to run repeatedly:

}
