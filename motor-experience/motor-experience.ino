#include <SoftwareSerial.h>
#include <AFMotor.h>
// 모터 부분
AF_DCMotor MOTOR1(1); // 모터 쉴드 M1 지정
AF_DCMotor MOTOR2(2); // 모터 쉴드 M2 지정
AF_DCMotor MOTOR3(3); // 모터 쉴드 M3 지정
AF_DCMotor MOTOR4(4); // 모터 쉴드 M4 지정

void setup() {
  // put your setup code here, to run once:
  MOTOR1.setSpeed(255);
  MOTOR2.setSpeed(255);
  MOTOR3.setSpeed(255);
  MOTOR4.setSpeed(255);
  
  MOTOR1.run(RELEASE);
  MOTOR2.run(RELEASE);
  MOTOR3.run(RELEASE);
  MOTOR4.run(RELEASE);
  Serial.begin(9600);
}
void Go_Forward() {
  MOTOR1.run(BACKWARD);
  MOTOR2.run(BACKWARD);
  MOTOR3.run(FORWARD);
  MOTOR4.run(BACKWARD);
  delay(20);
}

void loop() {
  // put your main code here, to run repeatedly:
  Go_Forward();
  //MOTOR1.run(FORWARD);


}
