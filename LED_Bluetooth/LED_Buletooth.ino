// APP Serial Bluetooth Terminal 사용.

#include "BluetoothSerial.h" //라이브러리 사용 선언.

BluetoothSerial SerialBT;

int receivedInt; //수신된 값을 이 변수에 int형으로 저장.
char receivedChar;// 수신된 값을 이 변수에 char형으로 저장.

char ON = 'a';
char OFF = 'b';
int Pin = 22;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("예제 보면서 공부중"); //블루투스 이름.
  Serial.println("Turn ON : a");
  Serial.println("Turn OFF : b");
  pinMode(Pin, OUTPUT); // 22번 출력 모드로 설정
}

void loop() {

  receivedChar = (char)SerialBT.read(); 
  
  if(Serial.available()){
    SerialBT.write(Serial.read()); // 블루투스 문자박스로부터 전송 내용 처리
  }
  
  if(SerialBT.available()){
    if(receivedChar == 'a'){
      SerialBT.println("ON");
      Serial.println("ON");
      digitalWrite(Pin, HIGH); // 22번 ON상태
    }
    if(receivedChar == 'b'){
      SerialBT.println("OFF"); 
      Serial.println("OFF");
      digitalWrite(Pin, LOW); //22번 OFF상태
    }
  }
  delay(20); //1000 = 1sec
}
