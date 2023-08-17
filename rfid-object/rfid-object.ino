// 라이브러리 해더 
#include "SPI.h"
#include "MFRC522.h"

// SS(Chip Select)과 RST(Reset) 핀 설정
// 나머지 PIN은 SPI 라이브러리를 사용하기에 별도의 설정이 필요없다.
#define SS_PIN 10
#define RST_PIN 9
#define OBJ_1 1
#define OBJ_2 2
#define OBJ_3 3
#define piezo 7

// 라이브러리 생성
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key; 

void playSong() {
  tone(piezo, 523);
  delay(100);
  noTone(piezo);
  delay(100);
  tone(piezo, 523);
  delay(100);
  noTone(piezo);
  delay(100);
}
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
    delay(100);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    if (rfid == "63:65:bf:34") {
      playSong();
    }
    if (rfid == "27:c8:d3:83") {
      playSong();
    }
    if (rfid == "2c:6c:61:39") {
      playSong();
    }
    else {
      noTone(piezo);
    }
    
  }
  return -1;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  noTone(piezo);
}

void loop() {
  isKey();
}


