#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("AI Camera");
  lcd.begin();
  lcd.backlight();
}

void loop() {
  if(Serial.available()){
    SerialBT.write(Serial.read());
  }
  if(SerialBT.available()){
    String strData = SerialBT.readStringUntil('\n'); //뒤에 널문자 올 때까지 읽기
    strData.trim(); //공백 제거
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(strData);
  }
}
