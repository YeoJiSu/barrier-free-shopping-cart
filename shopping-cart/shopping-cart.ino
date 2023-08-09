int piezo = 13;
int trig = 12;
int echo = 11;
int led_R = 10;
int led_Y = 9;
int led_B = 8;

void setup() {
  pinMode(trig, OUTPUT); // 초음파 센서 trigger pin의 pinmode 설정 (초음파 송신)
  pinMode(echo, INPUT); // 초음파 센서 ech pin의 pinmode 설정 (초음파 수신)
  pinMode(led_R, OUTPUT); // LED RED
  pinMode(led_Y, OUTPUT); // LED YELLOW
  pinMode(led_B, OUTPUT); // LED BLUE
  Serial.begin(9600);
}


void loop() {
  float len, distance;
  digitalWrite(trig, LOW); // 초기화
  delay(2);
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig, LOW);

  len = pulseIn(echo, HIGH);
  distance = ((float)(340*len)/10000)/2;
  Serial.println(distance);

  if (distance<=10) {
    digitalWrite(led_Y, HIGH);
    tone(piezo, 494);
    
    
  }
  if (distance>10) {
    digitalWrite(led_Y, LOW);
    noTone(piezo);
  }
  delay(500);
  

}
