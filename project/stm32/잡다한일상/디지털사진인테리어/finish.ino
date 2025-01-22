#include <WiFi.h>
#include <WebServer.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "SD.h"

// Wi-Fi 및 웹 서버 설정
const char* ssid = "olleh_WiFi_BB5F";       // Wi-Fi SSID
const char* password = "0000003353"; // Wi-Fi 비밀번호
WebServer server(80);

// TFT 디스플레이 객체
TFT_eSPI tft = TFT_eSPI();

// SD 카드 및 디스플레이 CS 핀
#define SD_CS 10
#define TFT_CS 7

String savePath = "";

// BMP 디스플레이 함수
void displayBMP(const char *filename, int16_t x, int16_t y) {
  File bmpFile;
  uint16_t bmpWidth, bmpHeight;
  uint8_t bmpDepth;
  uint32_t bmpImageOffset, rowSize;
  uint8_t sdbuffer[3 * 320]; // BMP 읽기 버퍼
  uint16_t lineBuffer[320];  // 디스플레이 출력 버퍼

  if ((x >= tft.width()) || (y >= tft.height())) return;

  // SD 카드 활성화
  digitalWrite(SD_CS, LOW);
  digitalWrite(TFT_CS, HIGH); // 디스플레이 비활성화

  bmpFile = SD.open(filename, "r");
  if (!bmpFile) {
    Serial.print("File not found: ");
    Serial.println(filename);
    digitalWrite(SD_CS, HIGH);
    return;
  }

  if (read16(bmpFile) != 0x4D42) {  // "BM" 체크
    Serial.println("Not a BMP file");
    bmpFile.close();
    digitalWrite(SD_CS, HIGH);
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
    digitalWrite(SD_CS, HIGH);
    return;
  }

  bmpDepth = read16(bmpFile);  // 비트 깊이 확인
  if (bmpDepth != 24) {  // 24비트 BMP만 지원
    Serial.println("Unsupported BMP format");
    bmpFile.close();
    digitalWrite(SD_CS, HIGH);
    return;
  }

  read32(bmpFile);  // 압축 방식
  rowSize = (bmpWidth * 3 + 3) & ~3;

  // 디스플레이 활성화
  digitalWrite(SD_CS, HIGH);
  digitalWrite(TFT_CS, LOW);

  for (int16_t row = 0; row < bmpHeight; row++) {
    uint32_t pos = bmpImageOffset + (bmpHeight - 1 - row) * rowSize;

    // SD 카드 활성화
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(SD_CS, LOW);

    bmpFile.seek(pos);
    bmpFile.read(sdbuffer, rowSize);

    // 디스플레이 활성화
    digitalWrite(SD_CS, HIGH);
    digitalWrite(TFT_CS, LOW);

    for (int col = 0; col < bmpWidth; col++) {
      uint8_t b = sdbuffer[col * 3];
      uint8_t g = sdbuffer[col * 3 + 1];
      uint8_t r = sdbuffer[col * 3 + 2];
      lineBuffer[col] = tft.color565(r, g, b);
    }

    tft.pushImage(x, y + row, bmpWidth, 1, lineBuffer);
  }

  bmpFile.close();
  digitalWrite(TFT_CS, HIGH);  // 디스플레이 비활성화
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

// SD 카드 파일 및 폴더 목록 생성 함수
String listSDFiles(String path = "/") {
  String fileList = "<form method='GET' action='/display'>";
  File root = SD.open(path);
  File file = root.openNextFile();

  while (file) {
    String fileName = file.name();
    if (file.isDirectory()) {
      // 폴더 클릭 시 해당 폴더로 이동
      if(path == "/"){
        fileList += "<a href='/?path=" + path + fileName + "'><b>[DIR]</b> " + fileName + "</a><br>";
      }else{
        fileList += "<a href='/?path=" + path + "/" + fileName + "'><b>[DIR]</b> " + fileName + "</a><br>";
      }
    } else {
      // 파일 클릭 시 해당 파일을 표시
      fileList += "<input type='radio' name='file' value='" + fileName + "'> " + fileName + "<br>";
    }
    file = root.openNextFile();
  }

  fileList += "<input type='submit' value='Display'>";
  fileList += "</form>";
  savePath = path;
  Serial.print("move : ");
  Serial.println(savePath);
  return fileList;
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>SD Card File List</h1>";

  // Get the current path
  String path = "/";
  if (server.hasArg("path")) {
    path = server.arg("path");
  }

  html += "<h3>Current path: " + path + "</h3>";
  html += listSDFiles(path);  // Display files and folders in the current directory
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleDisplay() {
  if (server.hasArg("file")) {
    String fileName = server.arg("file");
    String path = savePath;
    if (server.hasArg("path")) {
      path = server.arg("path");
    }

    String fullPath = path + "/" + fileName;  // Construct the full path
    
    File selectedFile = SD.open(fullPath.c_str());
    if (!selectedFile) {
      server.send(400, "text/plain", "File or folder not found on SD card");
      return;
    }

    if (selectedFile.isDirectory()) {
      // If it's a directory, redirect to the directory's contents
      String newPath = path + "/" + fileName;
      server.sendHeader("Location", "/?path=" + newPath);  // Redirect to the directory
      server.send(303);  // HTTP Status: See Other
      selectedFile.close();
      return;
    } else {
      // If it's a file, display it as a BMP image
      displayBMP(fullPath.c_str(), 0, 0);
      server.send(200, "text/plain", "Displaying: " + fileName);
    }
  } else {
    server.send(400, "text/plain", "No file selected");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(SD_CS, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);   // SD 카드 기본 비활성화
  digitalWrite(TFT_CS, HIGH);  // 디스플레이 기본 비활성화

  if (!SD.begin(SD_CS, SPI, 8000000)) {
    Serial.println("SD Card Mount Failed");
    return;
  } else {
    Serial.println("SD Card Mount Successful");
  }

  tft.init();
  tft.setSwapBytes(true);
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/display", HTTP_GET, handleDisplay);

  server.begin();
  Serial.println("Server Started");
}

void loop() {
  server.handleClient();
}
