#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
SoftwareSerial MP3Module(10, 11);
DFRobotDFPlayerMini MP3Player;

int trigPin = 9;
int echoPin = 8;

void setup () {
  Serial.begin (9600);
  MP3Module.begin(9600);
  if (!MP3Player.begin(MP3Module)) { // MP3 모듈을 초기화합니다. 초기화에 실패하면 오류를 발생시킵니다.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  delay(1);
  MP3Player.volume(20);  // 볼륨을 조절합니다. 0~30까지 설정이 가능합니다.
  MP3Player.play(1);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop () {
  int val_x = analogRead(A0);
  int val_y = analogRead(A1);
  Serial.print("VRx : ");
  Serial.print(val_x);
  Serial.print("    ");
  Serial.print("VRy : ");
  Serial.println(val_y);
  delay(200);
  static unsigned long timer = millis();
  if (val_x == 0) {
    // 출발지점으로 이동합니다.
    Serial.println("출발지점으로");
  }
  if (val_y == 0) {
    Serial.println("물체 1(콜라)로");
    MP3Player.play(2);
  }
  if (val_x > 1000) {
    Serial.println("물체 2(새우깡)로");
    MP3Player.play(3);
  }
  if (val_y > 1000) {
    Serial.println("물체 3(파인애플)로");
    MP3Player.play(6);
  }
  
  
  float duration, distance;
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
 
  // 장애물과의 거리계산
  duration = pulseIn(echoPin, HIGH);
  distance = ((float)(340 * duration) / 10000) / 2;
  // Serial.println(distance);
  if(distance < 10) //장애물이 OOcm 이내일때
  {
    MP3Player.play(5); //(0005.mp3) 파일 플레이
    //delay(5000);
  }
  
}
