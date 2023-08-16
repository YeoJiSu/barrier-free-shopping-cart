#include <SoftwareSerial.h>
#include <AFMotor.h>
#include "SPI.h"
#include "MFRC522.h"
#include <DFRobotDFPlayerMini.h>
SoftwareSerial MP3Module(A14, A15); // 34, 35
DFRobotDFPlayerMini MP3Player;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  MP3Module.begin(9600);
  if (!MP3Player.begin(MP3Module)) { // MP3 모듈을 초기화합니다. 초기화에 실패하면 오류를 발생시킵니다.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  MP3Player.volume(20);  // 볼륨을 조절합니다. 0~30까지 설정이 가능합니다.
  MP3Player.play(1);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  int val_x = analogRead(A8);
  int val_y = analogRead(A9);

  if (val_x < 10) {
    // 출발지점으로 이동합니다.
    Serial.println("출발지점으로");
    MP3Player.play(1);
    DESTINATION = START;
  }
  if (val_y < 10) {
    Serial.println("물체 1(콜라)로");
    MP3Player.play(3);
    DESTINATION = OBJ_1;
  }
  if (val_x > 1000) {
    Serial.println("물체 2(새우깡)로");
    MP3Player.play(4);
    DESTINATION = OBJ_2;
  }
  if (val_y > 1000) {
    Serial.println("물체 3(파인애플)로");
    MP3Player.play(5);
    DESTINATION = OBJ_3;
  }
  delay(100);
}
