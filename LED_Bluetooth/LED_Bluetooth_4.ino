#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

char btData;

int blueOne = 23;
int blueTwo = 22;
int blueThree = 21;
int blueFour = 19;

char b1 = '1';
char b2 = '2';
char b3 = '3';
char b4 = '4';

void setup() {
  Serial.begin(115200);
  SerialBT.begin("LED four");
  Serial.println("gogo");

  pinMode(blueOne, OUTPUT);
  pinMode(blueTwo, OUTPUT);
  pinMode(blueThree, OUTPUT);
  pinMode(blueFour, OUTPUT);
}

void loop() {
  btData = (char)SerialBT.read();

  if(Serial.available()){
    SerialBT.write(Serial.read()); // 블루투스 문자박스로부터 전송 내용 처리
  }
  
  if(SerialBT.available()) {

    if(btData == '1'){
      digitalWrite(blueOne, HIGH);
      digitalWrite(blueTwo, LOW);
      digitalWrite(blueThree, LOW);
      digitalWrite(blueFour, LOW);
    }
    if(btData == '2'){
      digitalWrite(blueOne, LOW);
      digitalWrite(blueTwo, HIGH);
      digitalWrite(blueThree, LOW);
      digitalWrite(blueFour, LOW);
    }
    if(btData == '3'){
      digitalWrite(blueOne, LOW);
      digitalWrite(blueTwo, LOW);
      digitalWrite(blueThree, HIGH);
      digitalWrite(blueFour, LOW);
    }
    if(btData == '4'){
      digitalWrite(blueOne, LOW);
      digitalWrite(blueTwo, LOW);
      digitalWrite(blueThree, LOW);
      digitalWrite(blueFour, HIGH);
    }
  }
}
