#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>

// Wi-Fi 및 웹 서버 설정
const char* ssid = "olleh_WiFi_BB5F";       // Wi-Fi SSID
const char* password = "0000003353"; // Wi-Fi 비밀번호
WebServer server(80);

// TFT 디스플레이 객체
TFT_eSPI tft = TFT_eSPI();

// BMP 디스플레이 함수
void displayBMP(const char *filename, int16_t x, int16_t y) {
  File bmpFile;
  uint16_t bmpWidth, bmpHeight;
  uint8_t bmpDepth;
  uint32_t bmpImageOffset, rowSize;
  uint8_t sdbuffer[3 * 320];
  uint16_t lineBuffer[320];

  if ((x >= tft.width()) || (y >= tft.height())) return;

  bmpFile = SPIFFS.open(filename, "r");
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

  rowSize = (bmpWidth * 3 + 3) & ~3;

  // 이미지 출력
  for (int16_t row = 0; row < bmpHeight; row++) {
    uint32_t pos = bmpImageOffset + (bmpHeight - 1 - row) * rowSize;
    bmpFile.seek(pos);
    bmpFile.read(sdbuffer, rowSize);

    for (int col = 0; col < bmpWidth; col++) {
      uint8_t b = sdbuffer[col * 3];
      uint8_t g = sdbuffer[col * 3 + 1];
      uint8_t r = sdbuffer[col * 3 + 2];
      lineBuffer[col] = tft.color565(r, g, b);
    }

    tft.pushImage(x, y + row, bmpWidth, 1, lineBuffer);
  }

  bmpFile.close();
  Serial.println("BMP displayed successfully!");
}

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}

// 파일 업로드 처리 함수
void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Upload Start: %s\n", upload.filename.c_str());
    // 파일을 덮어쓰기 전에 기존 파일 삭제
    if (SPIFFS.exists("/uploaded.bmp")) {
      SPIFFS.remove("/uploaded.bmp");  // 기존 파일 삭제
    }
    File file = SPIFFS.open("/uploaded.bmp", FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }
    file.close();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    File file = SPIFFS.open("/uploaded.bmp", FILE_APPEND);
    if (file) {
      file.write(upload.buf, upload.currentSize);
      file.close();
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    Serial.printf("Upload End: %s, %u bytes\n", upload.filename.c_str(), upload.totalSize);
    server.send(200, "text/plain", "File Uploaded");
    displayBMP("/uploaded.bmp", 0, 0);  // 업로드 완료 후 디스플레이
  }
}

void setup() {
  Serial.begin(115200);

  // Wi-Fi Station 모드 시작
  WiFi.begin(ssid, password);  // Wi-Fi 네트워크에 연결
  Serial.println("Connecting to WiFi...");
  
  // Wi-Fi 연결 완료 대기
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // IP 주소 출력

  // SPIFFS 초기화
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Initialization Failed");
    return;
  }

  // TFT 디스플레이 초기화
  tft.init();
  tft.setSwapBytes(true);

  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // 웹 서버 라우트 설정
  server.on("/upload", HTTP_POST, []() {
    server.send(200, "text/plain", "File Uploaded");
  }, handleUpload);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<form method='POST' action='/upload' enctype='multipart/form-data'>"
                                  "<input type='file' name='data'>"
                                  "<input type='submit' value='Upload'>"
                                  "</form>");
  });

  server.begin();
  Serial.println("Server Started");
}

void loop() {
  server.handleClient();
}
