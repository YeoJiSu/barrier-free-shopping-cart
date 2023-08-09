int piezo = 13;
int trig = 12;
int echo = 11;
int led_R = 10;
int led_Y = 9;
int led_B = 8;
int but_3 = 7; // 정지 기능
int but_2 = 6; // 출발 기능
int but_1 = 5; // 마이크로 말하는 기능
int led_G = 4; // 마이크 꺼졌는지 켜졌는지 체크하는 기능

int count_1 = 0;
int cart_state = 0; // 0 = 정지, 1 = 일시정지, 2 = 출발

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
  
  Serial.begin(9600);
}

void loop() {
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
    tone(piezo, 494);
  }
  if (distance>10 & cart_state == 1) { // 일시 정지 상태라면
    cart_state = 2; // 카트 다시 출발
    noTone(piezo);
  }
  
  if (cart_state == 0){ // 정지 상태라면, 
    Serial.println("카트 정지 상태.");
    digitalWrite(led_R, HIGH);
    digitalWrite(led_Y, LOW);
    digitalWrite(led_B, LOW);
  }
  if (cart_state == 1) { // 일시 정지 상태라면,
    Serial.println("카트 일시정지 상태.");
    digitalWrite(led_R, LOW);
    digitalWrite(led_Y, HIGH);
    digitalWrite(led_B, LOW); 
  }
  if (cart_state == 2) { // 출발 상태라면, 
    Serial.println("카트 출발 상태.");
    digitalWrite(led_R, LOW);
    digitalWrite(led_Y, LOW);
    digitalWrite(led_B, HIGH);     
  }
  
  delay(1000);
}
