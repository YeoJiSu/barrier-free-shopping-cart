// 최종 핀 => SDA 53 | SCK 52 | MOSI 51 | MISO 50 | RST 49
#include "SPI.h"
#include "MFRC522.h"

#define SDA_PIN 53
#define RST_PIN 49

MFRC522 mfrc522(SDA_PIN, RST_PIN);

MFRC522::MIFARE_Key key;


void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() 
{
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

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

}