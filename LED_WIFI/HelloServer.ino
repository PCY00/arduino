//esp32 내장되어있는 예제에 LEDON, OFF도 넣어 ip주소를 이용하여 esp32안에 내장되어있는 2번 LED "ON","OFF" 실행.
//실행 하고 난 후 인터넷을 활용하여 코드 뜯어봄.

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "olleh_WiFi_BB5F"; //와이파이 ssid
const char* password = "0000003353";  //와이파이 password

WebServer server(80); // HTTP 디폴트 포트인 80번 입력

const int led = 2; //esp32 내장 되어있는 파랑색 led가 2번이여서 2로 설정.

void handleRoot() {
  digitalWrite(led, 1); // 1 == HIGH
  server.send(200, "text/plain", "hello from esp32!"); // 200은 HTTP response code는 "OK"를 의미한다.
  delay(200); //확인용으로 delay 넣어줌
  digitalWrite(led, 0); // 0 == LOW
}

void handleNotFound() { //전송 폼 HTML?같은 느낌인가봄
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";   //ip 접속 시 화면에 URI: 를 보이게함
  message += server.uri(); //ip 접속 시 화면에 URI: 옆에 주소 표시
  message += "\nMethod: "; //ip 접속 시 화면에 Method: 를 보이게함
  message += (server.method() == HTTP_GET) ? "GET" : "POST"; 
  message += "\nArguments: ";
  message += server.args();                                    //파라미터의 개수
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message); //text/plain = 문자열을 리턴해준다고 하는듯 함.
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT); // LED를 출력으로 설정
  digitalWrite(led, 0); // LOW
  Serial.begin(115200);  //터미널 속도 맞춰주기 ( 같지않으면 시리얼 잘 안됨)
  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password); //WIFI 실행
  Serial.println("");

  // 연결 대기
  while (WiFi.status() != WL_CONNECTED) {    //WIFI 연결이 안된 상태일때
    delay(500);
    Serial.print(".");   //시리얼 모니터에 ..... 표시됨
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP주소 

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);  // "/"이면 함수 실행

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/LEDON",[]() {  // "/LEDON"이면 밑에 실행.
    server.send(200, "text/plain","LED ON");
    digitalWrite(led, 1);
    Serial.println("LED ON");
  });
  server.on("/LEDOFF",[]() {  // "/LEDOFF"이면 밑에 실행.
    server.send(200, "text/plain","LED OFF"); 
    digitalWrite(led, 0);
    Serial.println("LED OFF");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);// CPU가 다른 작업으로 전환되도록 허용
}
