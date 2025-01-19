#include <SPI.h>
#include "TFT_eSPI.h"
#include "FS.h"
#include "SD.h"

TFT_eSPI tft = TFT_eSPI();

void displayBMP(const char *filename, int16_t x, int16_t y) {
  File bmpFile;
  uint16_t bmpWidth, bmpHeight;
  uint8_t bmpDepth;
  uint32_t bmpImageOffset;
  uint32_t rowSize;
  uint8_t sdbuffer[3 * 320];  // 한 줄에서 최대 320픽셀(3바이트/pixel)을 읽음
  uint16_t lineBuffer[320];  // RGB565로 변환된 한 줄의 픽셀 데이터를 저장

  if ((x >= tft.width()) || (y >= tft.height())) return;

  bmpFile = SD.open(filename);
  if (!bmpFile) {
    Serial.print("File not found: ");
    Serial.println(filename);
    return;
  }

  if (read16(bmpFile) != 0x4D42) {  // "BM" 체크
    Serial.println("Not a BMP file");
    bmpFile.close();
    return;
  }

  read32(bmpFile);  // 파일 크기
  read32(bmpFile);  // 예약된 공간
  bmpImageOffset = read32(bmpFile);

  read32(bmpFile);  // DIB 헤더 크기
  bmpWidth = read32(bmpFile);
  bmpHeight = read32(bmpFile);
  if (read16(bmpFile) != 1) {  // 플레인 수 확인
    Serial.println("Invalid BMP");
    bmpFile.close();
    return;
  }

  bmpDepth = read16(bmpFile);  // 비트 깊이 확인
  if (bmpDepth != 24) {  // 24비트 BMP만 지원
    Serial.println("Unsupported BMP format");
    bmpFile.close();
    return;
  }

  read32(bmpFile);  // 압축 방식

  rowSize = (bmpWidth * 3 + 3) & ~3;  // 각 행의 데이터 크기 (4바이트 패딩 포함)

  // 이미지 데이터를 출력
  for (int16_t row = 0; row < bmpHeight; row++) {
    uint32_t pos = bmpImageOffset + (bmpHeight - 1 - row) * rowSize;  // BMP는 아래에서 위로 저장됨
    bmpFile.seek(pos);
    bmpFile.read(sdbuffer, rowSize);

    for (int col = 0; col < bmpWidth; col++) {
      uint8_t b = sdbuffer[col * 3];
      uint8_t g = sdbuffer[col * 3 + 1];
      uint8_t r = sdbuffer[col * 3 + 2];
      lineBuffer[col] = tft.color565(r, g, b);  // RGB888 → RGB565 변환
    }

    tft.pushImage(x, y + row, bmpWidth, 1, lineBuffer);  // 한 줄 출력
  }

  bmpFile.close();
  Serial.println("BMP displayed successfully with pushImage!");
}

uint16_t read16(File &f){
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(File &f){
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}

void setup(){
  Serial.begin(115200);
  if(!SD.begin(10, SPI, 8000000)){
    Serial.println("SD Card Mount Failed");
    return;
  }

  tft.init();
  tft.setSwapBytes(true);

  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
}

void loop(){
  displayBMP("/picture/1.bmp", 0, 0);
  delay(1000);
  displayBMP("/picture/2.bmp", 0, 0);
  delay(1000);
  displayBMP("/picture/3.bmp", 0, 0);
  delay(1000);
}

