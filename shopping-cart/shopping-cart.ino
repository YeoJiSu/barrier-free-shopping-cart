#include <SoftwareSerial.h>
#include <AFMotor.h>
#include "SPI.h"
#include "MFRC522.h"

// 최종 핀 => SDA 53 | SCK 52 | MOSI 51 | MISO 50 | RST 49
#define SDA_PIN 53
#define RST_PIN 49
MFRC522 mfrc522(SDA_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

AF_DCMotor MOTOR1(1); // 모터 쉴드 M1 지정
AF_DCMotor MOTOR2(2); // 모터 쉴드 M2 지정
AF_DCMotor MOTOR3(3); // 모터 쉴드 M3 지정
AF_DCMotor MOTOR4(4); // 모터 쉴드 M4 지정

int START = 0;
int OBJ_1 = 1;
int OBJ_2 = 2;
int OBJ_3 = 3;
int CROSS = 4;
int START_POINT = START;
int DESTINATION = OBJ_1;


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

 // 마이크 꺼졌는지 켜졌는지 체크하는 기능
int IRL = 32; // 왼쪽 적외선 센서
int IRR = 33; // 오른쪽 적외선 센서

int count_1 = 0;
int cart_state = 0; // 0 = 정지, 1 = 일시정지, 2 = 출발

void Stop_Release() {
  MOTOR1.run(RELEASE);
  MOTOR2.run(RELEASE);
  MOTOR3.run(RELEASE);
  MOTOR4.run(RELEASE);
  delay(20);
}
void Go_Forward() {
  MOTOR1.run(FORWARD);
  MOTOR2.run(BACKWARD);
  MOTOR3.run(BACKWARD);
  MOTOR4.run(BACKWARD);
  delay(20);
}
void Go_Backward() {
  MOTOR1.run(BACKWARD);
  MOTOR2.run(FORWARD);
  MOTOR3.run(FORWARD);
  MOTOR4.run(FORWARD);
  delay(20);
}
void Go_Left() {
  MOTOR1.run(FORWARD); // MOTOR1.run(FORWARD); 
  MOTOR2.run(FORWARD); // MOTOR2.run(BACKWARD);
  MOTOR3.run(FORWARD); // MOTOR3.run(BACKWARD);
  MOTOR4.run(BACKWARD); // MOTOR4.run(FORWARD);
  delay(20);
}
void Go_Right() {
  MOTOR1.run(BACKWARD); // MOTOR1.run(BACKWARD);
  MOTOR2.run(BACKWARD); // MOTOR2.run(FORWARD); 
  MOTOR3.run(BACKWARD); // MOTOR3.run(FORWARD); 
  MOTOR4.run(FORWARD); // MOTOR4.run(BACKWARD);
  delay(20);
}

void Stop_Destination() {
  // 간단하게 만든 함수
  if (DESTINATION == START) {
    // START -> stop
    if (isKey() == START) {
      Stop_Release();
    }
    else {
      Go_Forward();
    }
  }
  else if (DESTINATION == OBJ_1) {
    // OBJ_1 -> stop
    if (isKey() == OBJ_1) {
      Stop_Release();
      cart_state = 0;
    }  
    else {
      Go_Forward();
    }
  }
  else if (DESTINATION == OBJ_2) {
    // OBJ_2 -> stop
    if (isKey() == OBJ_2) {
      Stop_Release();
    }
    else {
      Go_Forward();
    }
  }
  else if (DESTINATION == OBJ_3) {
    // OBJ_3 -> stop
    if (isKey() == OBJ_3) {
      Stop_Release();
    }  
    else {
      Go_Forward();
    }
  }
  else {
    Go_Forward();
  }
}
void Line_Trace() {
  if (digitalRead(IRL)==LOW && digitalRead(IRR)==LOW){
      // Go_Forward();
      Stop_Destination();
      Serial.println("전진");
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRR)==LOW){
    Go_Right();
    Serial.println("오른쪽으로");
  }
  if (digitalRead(IRL)==LOW && digitalRead(IRR)==HIGH){
    Go_Left();
    Serial.println("왼쪽으로");
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRR)==HIGH){
    Stop_Release();
    Serial.println("정지");
  }
}

void setup() {

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

  MOTOR1.setSpeed(250);
  MOTOR2.setSpeed(250);
  MOTOR3.setSpeed(250);
  MOTOR4.setSpeed(250);

  MOTOR1.run(RELEASE);
  MOTOR2.run(RELEASE);
  MOTOR3.run(RELEASE);
  MOTOR4.run(RELEASE);

  Serial.begin(9600);

  SPI.begin();
  mfrc522.PCD_Init();

}

/*
if (START_POINT == START & DESTINATION == OBJ_1) {
  // START -> right (delay 100 정도 걸고 두개다 lowlow 인식될 때까지 돌기)
  // OBJ_1 -> stop
}
if (START_POINT == START & DESTINATION == OBJ_2) {
  // START -> forward 
  // OBJ_2 -> stop
}
if (START_POINT == START & DESTINATION == OBJ_3) {
  // START -> left
  // OBJ_3 -> stop
}
if (START_POINT == OBJ_1 & DESTINATION == START) {
  // START -> stop
}
if (START_POINT == OBJ_1 & DESTINATION == OBJ_2) {
  // if CROSS -> left
  // if START -> right
  // OBJ_2 -> stop
}
if (START_POINT == OBJ_1 & DESTINATION == OBJ_3) {
  // OBJ_3 -> stop
}
if (START_POINT == OBJ_2 & DESTINATION == START) {
  // if CROSS -> right
  // START-> stop
}
if (START_POINT == OBJ_2 & DESTINATION == OBJ_1) {
  // if START -> left
  // if CROSS -> right
  // OBJ_1 -> stop
}
if (START_POINT == OBJ_2 & DESTINATION == OBJ_3) {
  // if START -> right
  // if CROSS -> left
  // OBJ_3 -> stop
}
if (START_POINT == OBJ_3 & DESTINATION == START) {
  // START -> stop
}
if (START_POINT == OBJ_3 & DESTINATION == OBJ_1) {
  // OBJ_1 -> stop
}
if (START_POINT == OBJ_3 & DESTINATION == OBJ_2) {
  // if START -> left
  // if CROSS -> right
  // OBJ_2 -> stop
}
*/

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
void loop() {
  // TODO 마이크 통해서 목적지 입력하는 코드 추가할 것
  Serial.print("목적지는: ");
  if (DESTINATION == START) {
    Serial.println("START");
  }
  if (DESTINATION == OBJ_1) {
    Serial.println("OBJ_1");
  }
  if (DESTINATION == OBJ_2) {
    Serial.println("OBJ_2");
  }
  if (DESTINATION == OBJ_3) {
    Serial.println("OBJ_3");
  }
  
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

