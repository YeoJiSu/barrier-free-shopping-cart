#include <SoftwareSerial.h>
#include <AFMotor.h>
#include "SPI.h"
#include "MFRC522.h"
#include <DFRobotDFPlayerMini.h>
#include "VoiceRecognitionV3.h"

int IRL = 32; // 왼쪽 적외선 센서
int IRM = 33; // 오른쪽 적외선 센서
int IRR = 34;
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

void Line_Trace() {
  // 0 0 0 
  if (digitalRead(IRL)==LOW && digitalRead(IRM)==LOW && digitalRead(IRR)==LOW)  { // 0 0 0
    delay(20);
    Go_Right();
    Serial.println("0 0 0");
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRM)==LOW && digitalRead(IRR)==HIGH)  { // 1 0 1
    delay(20);
    Go_Right();
    Serial.println("1 0 1");
  }
  if (digitalRead(IRL)==LOW && digitalRead(IRM)==LOW && digitalRead(IRR)==HIGH)  { // 0 0 1
    delay(20);
    Go_Right();
    Serial.println("0 0 1");
  }
  if (digitalRead(IRL)==LOW && digitalRead(IRM)==HIGH && digitalRead(IRR)==HIGH)  { // 0 1 1
    delay(20);
    Go_Right();
    Serial.println("0 1 1");
    //delay(1000);
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRM)==LOW && digitalRead(IRR)==LOW)  { // 1 0 0
    delay(20);
    Go_Left();
    Serial.println("1 0 0");
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRM)==HIGH && digitalRead(IRR)==LOW)  { // 1 1 0
    delay(20);
    Go_Left();
    Serial.println("1 1 0");
  }
  if (digitalRead(IRL)==HIGH && digitalRead(IRM)==HIGH && digitalRead(IRR)==HIGH)  { // 1 1 1
    delay(20);
    Go_Forward();
    Serial.println("1 1 1");
  }
  if (digitalRead(IRL)==LOW && digitalRead(IRM)==HIGH && digitalRead(IRR)==LOW)  { // 0 1 0
    delay(20);
    Go_Forward();
    Serial.println("0 1 0");
  }

}

void setup() {
  Serial.begin(9600);
  // 적외선 센서
  pinMode(IRR, INPUT);
  pinMode(IRM, INPUT);
  pinMode(IRL, INPUT);

  MOTOR1.setSpeed(100);
  MOTOR2.setSpeed(100);
  MOTOR3.setSpeed(100);
  MOTOR4.setSpeed(100);

  MOTOR1.run(RELEASE);
  MOTOR2.run(RELEASE);
  MOTOR3.run(RELEASE);
  MOTOR4.run(RELEASE);

  SPI.begin();
  //mfrc522.PCD_Init();

}

void loop() {
  // Line_Trace();
  // Go_Forward();
  Go_Right();
  // Go_Left();

}
