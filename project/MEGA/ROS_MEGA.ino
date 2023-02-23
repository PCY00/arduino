#include <ros.h>
#include <std_msgs/String.h>
#include <TimeLib.h>

//so2, no2
#define ContinueMode 0
#define PollingMode 1

ros::NodeHandle  nh;

std_msgs::String str;
ros::Publisher chatter("str_data", &str);

char buffer[256];
unsigned long past = 0;
bool flag = false;
char co_print[20];
float co_ppm;
char NO2_data[50];    //Hardware-designed no2 data archiving
char SO2_data[50];    //Hardware-designed so2 data archiving
char NO2_print[7];    //print no2 string
char SO2_print[7];    //print so2 string
char degree_print[7];
float direct[] = {3.84,1.98,2.25,0.41,0.45,0.32,0.90,0.62,1.40,1.19,3.08,2.93,4.62,4.04,4.33,3.43};

void setup(){
  nh.initNode();
  nh.advertise(chatter);
  
  Serial.begin(57600);
  Serial1.begin(9600);        //NO2
  Serial2.begin(9600);        //SO2
  delay(1000);
  
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
  int wind_val = analogRead(A1);
  float wind_outvoltage = wind_val * (5.0 / 1023.0);
  int wind_print = 6 * wind_outvoltage;
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
    Serial2.write('\r');
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
  while (Serial2.available()) { // read from AGSM port, send to Serial port to interupt continuous output send 'c''/r' without line ending, may have to send more than once.
    SO2_data[i] = Serial2.read();
    i++;
  }
  i = 0;
  
  while (Serial1.available()) {
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
    sprintf(buffer,"%s,%s,%s,%2d,%s", SO2_print, NO2_print, co_print, wind_print, degree_print);
    str.data = buffer;
    chatter.publish(&str);

    flag = false;
  }
  nh.spinOnce();
}
