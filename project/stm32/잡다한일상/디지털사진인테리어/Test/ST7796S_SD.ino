#include <SPI.h>
#include "TFT_eSPI.h"
#include "FS.h"
#include "SD.h"

TFT_eSPI tft = TFT_eSPI();

void displayBMP(const char *filename, int16_t x, int16_t y){
  File bmpFile;
  uint16_t bmpWidth, bmpHeight; // 이미지 가로, 세로
  uint8_t bmpDepth;             // 색상 깊이
  uint32_t bmpImageOffset;      //이미지 데이터의 시작 위치
  uint32_t rowSize;
  uint8_t sdbuffer[3*20];       //sd에서 읽을 버퍼
  uint8_t buffidx = sizeof(sdbuffer);

  if((x >= tft.width()) || (y >= tft.height())){ return; }

  bmpFile = SD.open(filename);
  if(!bmpFile){
    Serial.print("File not found: ");
    Serial.println(filename);
    return;
  }

  // bmp 파일 구조
  //오프셋(Byte)	크기(Byte)	설명
  //  0	            2	      파일 식별자 ("BM")
  //  2	            4     	파일 크기
  //  6           	4	      예약된 공간
  //  10	          4	      이미지 데이터 시작 위치
  //  14	          ...    	DIB 헤더
  if(read16(bmpFile) != 0x4D42){
    Serial.println("Not a BMP file");
    bmpFile.close();
    return;
  }

  read32(bmpFile);  //file size
  read32(bmpFile);  //예약된 공간
  bmpImageOffset = read32(bmpFile);

  read32(bmpFile);
  bmpWidth = read32(bmpFile);
  bmpHeight = read32(bmpFile);
  if(read16(bmpFile) != 1){
    Serial.println("Invalid BMP");
    bmpFile.close();
    return;
  }

  bmpDepth = read16(bmpFile);
  if(bmpDepth != 24){
    Serial.println("Unsupported BMP format");
    bmpFile.close();
    return;
  }

  read32(bmpFile);
  // 이미지의 데이터 시작점으로 이동
  bmpFile.seek(bmpImageOffset);

  //BMP는 bmpWidth픽셀이고, RGB 즉 3byte로 저장, 따라서 bmpWidth * 3, 
  //+3을 더한뒤 ~3을 &연산해줌 (그래야 4의 배수가 되기 때문 -> 4바이트 단위 정렬 규칙)
  rowSize = (bmpWidth * 3 + 3) & ~3;

  for(int16_t row = 0; row < bmpHeight; row++){
    uint32_t pos = bmpImageOffset + (bmpHeight - 1 - row) * rowSize;
    bmpFile.seek(pos);

    for(int col = 0; col < bmpWidth; col++){
      bmpFile.read(sdbuffer, 3);
      uint16_t color = tft.color565(sdbuffer[2], sdbuffer[1], sdbuffer[0]);
      tft.drawPixel(x + col, y + row, color);
    }
  }

  bmpFile.close();
  Serial.println("BMP displayed successfully!");
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
  if(!SD.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
}

void loop(){
  displayBMP("/picture/1.bmp", 0, 0);
  delay(5000);
  displayBMP("/picture/2.bmp", 0, 0);
  delay(5000);
  displayBMP("/picture/3.bmp", 0, 0);
  delay(5000);
}

