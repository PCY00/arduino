#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

const uint16_t myBitmap[153600] PROGMEM={
};
void drawBitmapLineByLine(int x, int y, const uint16_t *bitmap, int w, int h) {
  for (int j = 0; j < h; j++) {
    // 각 줄을 하나씩 `pushImage`로 처리
    tft.pushImage(x, y + j, w, 1, bitmap + (j * w));
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ST7796S TFT LCD Test");

  tft.init();
  tft.setSwapBytes(true);

  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  //tft.pushImage(0, 0, 320, 480, myBitmap);
    // 한 줄씩 이미지를 그리기
  drawBitmapLineByLine(0, 0, myBitmap, 320, 480);

}

void loop() {
}
