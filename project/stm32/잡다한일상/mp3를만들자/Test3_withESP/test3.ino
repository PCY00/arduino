#include "mp3.h"

mp3 m1;

void setup(){
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 20, 21);
  delay(3000);
  m1.volumeSet(10);
  delay(100);
  m1.selectTrack(1);
}

void loop(){

}

