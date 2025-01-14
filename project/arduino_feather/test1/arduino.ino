#include <RH_RF95.h>
#include <SPI.h>

#define RFM95_CS 10  // CS 핀 (Arduino Uno에서 핀 10 사용)
#define RFM95_RST 9  // RST 핀
#define RFM95_INT 2  // INT 핀 (interrupt 핀)

#define RF95_FREQ 920.9  // 주파수 설정 (920.9 MHz)

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int counter = 0;

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  while (!Serial) {
    delay(1);
  }
  delay(100);

  Serial.println("LoRa Sender (RH_RF95)");

  // LoRa 모듈 리셋
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("Set frequency failed");
    while (1);
  }
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);  // 최대 출력 설정
}

void loop() {
  char message[20];
  sprintf(message, "Packet %d", counter);

  Serial.print("Sending: ");
  Serial.println(message);

  rf95.send((uint8_t*)message, strlen(message));
  rf95.waitPacketSent();
  Serial.println("Packet sent!");

  counter++;
  delay(5000);
}
