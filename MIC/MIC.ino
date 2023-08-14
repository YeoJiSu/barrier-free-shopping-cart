#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
VR myVR(2,3);    // 2:RX 3:TX,
uint8_t records[7];
uint8_t buf[64];

void setup()
{
  myVR.begin(9600);
  Serial.begin(115200);
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0)
  {
    Serial.print(buf[1]);
    Serial.println("");
  }
}
