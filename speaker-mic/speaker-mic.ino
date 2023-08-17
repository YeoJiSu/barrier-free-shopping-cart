#include <SoftwareSerial.h>
#include <AFMotor.h>
#include "SPI.h"
#include "MFRC522.h"
#include <DFRobotDFPlayerMini.h>
#include "VoiceRecognitionV3.h"
// 스피커 부분
SoftwareSerial MP3Module(A14, A15); // 34, 35
DFRobotDFPlayerMini MP3Player;

// 마이크 부분
VR myVR(10,11);    // 2:RX 3:TX,
uint8_t records[7];
uint8_t buf[64];

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
  // MP3Player.play(1);
  delay(1000);

}

void loop() {

  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0)
  {
    // 0, 1 콜라
    // 2,3 새우깡
    // 4,5 파인애플
    // 6 시작점
    
    Serial.print(buf[1]);
    Serial.println("");
    

    if (buf[1] == 1){
      Serial.println("빨강");
      MP3Player.play(3);
    } 
    if (buf[1] == 2){
      Serial.println("빨강");
      MP3Player.play(3);
    } 
    if (buf[1] == 3){
      Serial.println("빨강");
      MP3Player.play(3);
    } 
    if (buf[1] == 4){
      Serial.println("빨강");
      MP3Player.play(3);
    } 

  }
}
