#include <WiFi.h>

const char* ssid = "olleh_WiFi_BB5F"; //같은 공유기에 연결되어 있어야 함.
const char* password = "0000003353";  //비번도 마찬가지.

WiFiServer server(80);                //보통 서버는 80포트를 이용함

String haeder;                       //?
String currentLine;                  //?

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){    //와이파이가 연결될 때까지 WiFi.status()는 와이파의 상태를 말하고, WL_CONNECTED는 와이파이가 네트워크에 연결될 때 할당됨
                                           //와이파이가 네트워크에 연결되지 않았으므로 while돌려.
    delay(500);                            //0.5초 간격으로
    Serial.print(".");                     // . 을 시리얼 화면에 출력
  }
  Serial.println("");                      //연결되면 한칸 점프'
  Serial.println("WiFi connected.");       
  Serial.println("IP adress: ");          
  Serial.println(WiFi.localIP());         //와이파이의 IP를 프린트
  server.begin();                         //서버 시작
}

void loop() {
  WiFiClient client = server.available(); //서버에 연결되어 있고 읽을 수 있는 데이터가 있는 클라이언트를 가져옴.

  if(client){                             //클라이언트 호출이 true이면 실행
    Serial.println("New Client.");        
    while(client.available()){            //클라이언트에 데이터가 없을 때 까지 실행
      char c = client.read();             //클라이언트를 바이트단위로 읽어드림.
      Serial.write(c);                    //c에 담겨진 데이터 읽음.
      if(c == '\n'){                      //만약 '\n'이면 밑에 실행
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection:close");
        client.println();

        client.println("<!DOCTYPE html><html>");
        client.println("<head><meta name=\"viewport\" conrtent=\"width=device-width, inital-scale=1\">");
        client.println("<link rel=\"icon\" href=\"data:,\"></head>");

        client.println("<body><h1>ESP32 Web Server</h1>");
        client.println("<img src=\"http://image.dongascience.com/Photo/2017/12/15130427942754.png\">");
        client.println("</body></html>");

        client.println();
        client.stop();                            //서버연결 해제
        Serial.println("Client disconnected.");
        Serial.println("");
      }
    }
  }
}
