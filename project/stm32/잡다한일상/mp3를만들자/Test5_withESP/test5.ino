#include "mp3.h"

mp3 m1(Serial1);

//uint8_t re[10];  // 10바이트 배열 선언
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 20, 21); // ESP32-C3 기본 UART1 핀 사용

  delay(3000);  // MP3 모듈 초기화 시간 확보

  Serial.println("MP3 Module Ready!");

  // 명령 테스트
  m1.volumeSet(10);
  delay(100);
  m1.querythecurrentvolume();
  delay(1000);
  m1.selectTrack(1);
  delay(1000);
  m1.pause();
  delay(100);
  m1.querythecurrentstatus();

  delay(100);
  m1.playback();
  delay(100);
  m1.querythecurrentstatus();

}


void loop() {

}
