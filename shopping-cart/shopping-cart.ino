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

int START = 0;
int OBJ_1 = 1;
int OBJ_2 = 2;
int OBJ_3 = 3;
int CROSS = 4;
int START_POINT = START;
int DESTINATION = START;

int trig = 22;
int echo = 23;

int but_1 = 24; // 마이크로 말하는 기능
int but_2 = 25; // 출발 기능
int but_3 = 26; // 정지 기능

int piezo = 27;
int led_G = 28;
int led_R = 29;
int led_Y = 30;
int led_B = 31;

int IRL = 32; // 왼쪽 적외선 센서 (초)
int IRM = 33; // 중간 적외선 센서 (갈)
int IRR = 34; // 오른쪽 적외선 센서 (노)

int count_1 = 0;
int cart_state = 0; // 0 = 정지, 1 = 일시정지, 2 = 출발

// 모터 부분
AF_DCMotor MOTOR1(1); // 모터 쉴드 M1 지정
AF_DCMotor MOTOR2(2); // 모터 쉴드 M2 지정
AF_DCMotor MOTOR3(3); // 모터 쉴드 M3 지정
AF_DCMotor MOTOR4(4); // 모터 쉴드 M4 지정

void Stop_Release() {
  MOTOR1.run(RELEASE);
  MOTOR2.run(RELEASE);
  MOTOR3.run(RELEASE);
  MOTOR4.run(RELEASE);
  delay(20);
}
void Go_Forward() {
  MOTOR1.run(BACKWARD);
  MOTOR2.run(BACKWARD);
  MOTOR3.run(FORWARD);
  MOTOR4.run(BACKWARD);
  delay(20);
}
void Go_Backward() {
  MOTOR1.run(FORWARD);
  MOTOR2.run(FORWARD);
  MOTOR3.run(BACKWARD);
  MOTOR4.run(FORWARD);
  delay(20);
}
void Go_Left() {
  MOTOR1.run(BACKWARD); // MOTOR1.run(FORWARD); 
  MOTOR2.run(FORWARD); // MOTOR2.run(BACKWARD);
  MOTOR3.run(BACKWARD); // MOTOR3.run(BACKWARD);
  MOTOR4.run(BACKWARD); // MOTOR4.run(FORWARD);
  delay(20);
}
void Go_Right() {
  MOTOR1.run(FORWARD); // MOTOR1.run(BACKWARD);
  MOTOR2.run(BACKWARD); // MOTOR2.run(FORWARD); 
  MOTOR3.run(FORWARD); // MOTOR3.run(FORWARD); 
  MOTOR4.run(FORWARD); // MOTOR4.run(BACKWARD);
  delay(20);
}

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

void Stop_Destination() {
  // 간단하게 만든 함수
  if (DESTINATION == START) {
    // START -> stop
    if (isKey() == START) {
      cart_state = 0;
      Stop_Release();
      //delay(1000);
    }
  }
  else if (DESTINATION == OBJ_1) {
    // OBJ_1 -> stop
    if (isKey() == OBJ_1) {
      cart_state = 0;
      Stop_Release();
      //delay(1000);
    } 
  }
  else if (DESTINATION == OBJ_2) {
    // OBJ_2 -> stop
    if (isKey() == OBJ_2) {
      cart_state = 0;
      Stop_Release();
      //delay(1000);
    }
  }
  else if (DESTINATION == OBJ_3) {
    // OBJ_3 -> stop
    if (isKey() == OBJ_3) {
      cart_state = 0;
      Stop_Release();
      //delay(1000);
    }  
  }
}

void Line_Trace() {
  // 0 0 0 
  if (digitalRead(IRL)==LOW && digitalRead(IRR)==LOW) {
    
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRR)==HIGH){
    Go_Forward();
    Serial.println("전진");
  }
  if (digitalRead(IRL)==LOW && digitalRead(IRR)==HIGH){
    Go_Right();
    Serial.println("오른쪽으로");
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRR)==LOW){
    Go_Left();
    Serial.println("왼쪽으로");
  }
  if (digitalRead(IRL)==LOW && digitalRead(IRR)==LOW){
    //Stop_Release();
    Go_Left();
    Serial.println("정지");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myVR.begin(9600);
  MP3Module.begin(9600);
  if (!MP3Player.begin(MP3Module)) { // MP3 모듈을 초기화합니다. 초기화에 실패하면 오류를 발생시킵니다.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  MP3Player.volume(20);  // 볼륨을 조절합니다. 0~30까지 설정이 가능합니다.
  MP3Player.play(2);
  delay(1000);

  pinMode(trig, OUTPUT); // 초음파 센서 trigger pin의 pinmode 설정 (초음파 송신)
  pinMode(echo, INPUT); // 초음파 센서 ech pin의 pinmode 설정 (초음파 수신)

  pinMode(led_R, OUTPUT); // LED RED
  pinMode(led_Y, OUTPUT); // LED YELLOW
  pinMode(led_B, OUTPUT); // LED BLUE
  pinMode(led_G, OUTPUT);
  // GND에 저항을 연결해서 풀다운 저항 푸시 버튼 스위치이다.
  pinMode(but_3, INPUT); 
  pinMode(but_2, INPUT);
  pinMode(but_1, INPUT);
  // 적외선 센서
  pinMode(IRR, INPUT);
  pinMode(IRL, INPUT);

  MOTOR1.setSpeed(200);
  MOTOR2.setSpeed(200);
  MOTOR3.setSpeed(200);
  MOTOR4.setSpeed(200);

  MOTOR1.run(RELEASE);
  MOTOR2.run(RELEASE);
  MOTOR3.run(RELEASE);
  MOTOR4.run(RELEASE);

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  
  // put your main code here, to run repeatedly:
  Stop_Destination();
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
    if (buf[1] == 6) {
      Serial.println("출발지점으로");
      MP3Player.play(1);
    }
    if ((buf[1] == 0) | (buf[1] == 1)) {
      Serial.println("물체 1(콜라)로");
      MP3Player.play(3);
    }
    if ((buf[1] == 2) | (buf[1] == 3)) {
      Serial.println("물체 2(새우깡)로");
      MP3Player.play(4);
    }
    if ((buf[1] == 4) | (buf[1] == 5)) {
      Serial.println("물체 3(파인애플)로");
      MP3Player.play(5);
    }
  }
  
  int val_x = analogRead(A8);
  int val_y = analogRead(A9);
  Serial.print(val_x);
  Serial.print(", ");
  Serial.println(val_y);
  
  if (val_x < 10) {
    // 출발지점으로 이동합니다.
    Serial.println("출발지점으로");
    DESTINATION = START;
    MP3Player.play(1);
  }
  if (val_y < 10) {
    Serial.println("물체 1(콜라)로");
    MP3Player.play(3);
    DESTINATION = OBJ_1;
  }
  if (val_x > 1000){
    Serial.println("물체 2(새우깡)로");
    MP3Player.play(4);
    DESTINATION = OBJ_2;
  }
  if (val_y > 1000){
    Serial.println("물체 3(파인애플)로");
    MP3Player.play(5);
    DESTINATION = OBJ_3;
  }
  
  
  Serial.print("목적지는: ");
  Serial.println(DESTINATION);
  
  
  if (count_1 >= 10000) {count_1 = 0;}
  
  if (digitalRead(but_1)==HIGH) {
    count_1 += 1;
    delay(20);
    if (count_1 % 2 == 1) {
      // 마이크 작동 기능
      digitalWrite(led_G, HIGH);
      Serial.println("마이크 작동합니다.");
    }
    if (count_1 % 2 == 0) {
      // 마이크 작동X 
      digitalWrite(led_G, LOW);
      Serial.println("마이크 껐습니다.");
    }
  }
  
  if (digitalRead(but_2)==HIGH) { // 카트 출발 시키기
    //delay(20);
    cart_state = 2;
  }
  if (digitalRead(but_3)==HIGH) { // 카트 정지 시키기
    //delay(20);
    cart_state = 0;                                             
  }

  float len, distance;
  digitalWrite(trig, LOW); // 초기화
  delay(2);
  digitalWrite(trig, HIGH );
  delay(10);
  digitalWrite(trig, LOW);

  len = pulseIn(echo, HIGH);
  distance = ((float)(340*len)/10000)/2;
  Serial.println(distance);

  if (cart_state == 2 & distance<=10) { // 출발 상태인데 앞에 물체가 있으면
    cart_state = 1; // 카트 일시정지
    Stop_Release();
    tone(piezo, 494);
    MP3Player.play(7);
  }
  if (distance>10 & cart_state == 1) { // 일시 정지 상태라면
    cart_state = 2; // 카트 다시 출발
    noTone(piezo);
    Line_Trace();
  }
  
  if (cart_state == 0){ // 정지 상태라면, 
    Serial.println("카트 정지 상태.");
    digitalWrite(led_R, HIGH);
    digitalWrite(led_Y, LOW);
    digitalWrite(led_B, LOW);
    Stop_Release();  
  }
  if (cart_state == 1) { // 일시 정지 상태라면,
    Serial.println("카트 일시정지 상태.");
    digitalWrite(led_R, LOW);
    digitalWrite(led_Y, HIGH);
    digitalWrite(led_B, LOW); 
    Stop_Release();  
  }
  if (cart_state == 2) { // 출발 상태라면, 
    Serial.println("카트 출발 상태.");
    digitalWrite(led_R, LOW);
    digitalWrite(led_Y, LOW);
    digitalWrite(led_B, HIGH);   
    Line_Trace();  
  }
  
  delay(100);

}
