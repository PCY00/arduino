#define LED_BUILTIN 8

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("OFF");
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("ON");
  delay(1000);
}
