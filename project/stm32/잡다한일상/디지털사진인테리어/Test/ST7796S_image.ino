#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

// 320x480px
const uint16_t myBitmap[] PROGMEM = {
//비트맵 이미지 데이터
};

// 비트맵을 그리는 함수
void drawBitmap(int x, int y, const uint16_t *bitmap, int w, int h) {
  int byteIndex = 0;
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      tft.drawPixel(x + i, y + j, pgm_read_word_near(bitmap + byteIndex));
      byteIndex++;
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // 비트맵 이미지를 화면에 그립니다.
  drawBitmap(0, 0, myBitmap, 320, 480); // (x, y, 비트맵 배열, 가로 크기, 세로 크기)
}

void loop() {
}
출처: https://kksp12y.tistory.com/95 [PCY_00:티스토리]
