int LED = 12;
int RIP = 13;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(RIP, INPUT);
  Serial.begin(115200);
}

void loop() {
  int value = digitalRead(RIP);
  if(value == 1){
    digitalWrite(LED, 1);
    Serial.println("LED_ON");
  }else {
    digitalWrite(LED, 0);
    Serial.println("LED_OFF");
  }
  delay(1000);
}
