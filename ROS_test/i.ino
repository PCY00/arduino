#include <SoftwareSerial.h>

//so2, no2
#define ContinueMode 0
#define PollingMode 1

SoftwareSerial SO2_serial(10,11);   //RX TX

char buffer[30];
unsigned long past = 0;
bool flag = false;
char co_print[7];
float co_ppm;
char NO2_data[50];    //Hardware-designed no2 data archiving
char SO2_data[50];    //Hardware-designed so2 data archiving
char NO2_print[7];    //print no2 string
char SO2_print[7];    //print so2 string
char degree_print[7];
float direct[] = {3.84,1.98,2.25,0.41,0.45,0.32,0.90,0.62,1.40,1.19,3.08,2.93,4.62,4.04,4.33,3.43};

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);       //NO2 0 1
  SO2_serial.begin(9600);
  delay(1000);

  #if ContinueMode
    Serial1.write('c');
    Serial1.write('\r');
    SO2_serial.write('c');
    SO2_serial.write('\r');
  #endif
  
}

void loop(){
//============================================================================
// 5sec check
  unsigned long now = millis();
  if(now - past >= 5000) {
    past = now;
    flag = true;
  }
//============================================================================
// wind speed 
  int wind_speed_val = analogRead(A1);
  float wind_speed_outvoltage = wind_speed_val * (5.0 / 1023.0);
  int wind_speed_print = 6 * wind_speed_outvoltage;
// wind direct
  int wind_direction_val = analogRead(A2);
  float wind_direction_outvoltage = wind_direction_val * 5 / 1023.0;

//============================================================================
// co
  int co_val = analogRead(0);
  co_ppm = (float)co_val / 1000;
  
//============================================================================
//SO2, NO2
  #if PollingMode
    SO2_serial.write('\r');
    Serial1.write('\r');
    delay(1000);
  #else
    delay(100);
  #endif

  int i = 0;
  String str_so2_data = "";
  String str_no2_data = "";

//==================================================================================
// wind degree get
  float degree;
  for(int j =0; j < 16; j++){
    if(direct[j]-0.02 < wind_direction_outvoltage && direct[j]+0.02 > wind_direction_outvoltage){
       degree = j * 22.5;
       break;
    }
  }
//==================================================================================
//SO2, NO2 data get
  while (SO2_serial.available() > 0) { // read from AGSM port, send to Serial port to interupt continuous output send 'c''/r' without line ending, may have to send more than once.
    SO2_data[i] = SO2_serial.read();
    i++;
  }
  i = 0;
  while (Serial1.available() > 0) {
    NO2_data[i] = Serial1.read();
    i++;
  }
  
//==================================================================================
// After 5sec print
  if(flag == true){
    for(int j = 13; j < 16; j++){
       str_so2_data += SO2_data[j];
       str_no2_data += NO2_data[j];
    }
    
    float so2_ppm, no2_ppm;
    //The part that changes String to Int and converts it to float to express it in ppm.
    so2_ppm = (float)str_so2_data.toInt() / 1000;
    no2_ppm = (float)str_no2_data.toInt() / 1000;
    
    
    //Function used by printf to use prime numbers
    //Decimal, total digits, number of representations after decimal point, string
    dtostrf(so2_ppm, 5, 3, SO2_print);
    dtostrf(no2_ppm, 5, 3, NO2_print);
    dtostrf(co_ppm, 4, 2, co_print);
    dtostrf(degree, 5, 2, degree_print);
    sprintf(buffer,"%s,%s,%s,%2d,%s", SO2_print, NO2_print, co_print, wind_speed_print, degree_print);
    Serial.println(buffer);

    flag = false;
  }
}
