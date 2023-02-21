#define USE_USBCON

#include <ros.h>
#include <std_msgs/String.h>
#include <TimeLib.h>

#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <avr/dtostrf.h>    //dtostrf
#include "DFRobot_OzoneSensor.h"

//so2, no2
#define ContinueMode 0
#define PollingMode 1
//ozone
#define COLLECT_NUMBER   20              // collect number, the collection range is 1-100
#define Ozone_IICAddress OZONE_ADDRESS_3

ros::NodeHandle  nh;

std_msgs::String str;
ros::Publisher chatter("str_data", &str);

//==================================================================================
//Add Serial
Uart Serial3 (&sercom2, 0, 1, SERCOM_RX_PAD_3, UART_TX_PAD_2);
void SERCOM2_Handler(){
    Serial3.IrqHandler();
}

Uart Serial2 (&sercom1, 12, 10, SERCOM_RX_PAD_3, UART_TX_PAD_2);
void SERCOM1_Handler(){
  Serial2.IrqHandler();
}
//==================================================================================
// var
char buffer[256];
unsigned long past = 0;
bool flag = false;
char NO2_data[50];    //Hardware-designed no2 data archiving
char SO2_data[50];    //Hardware-designed so2 data archiving
char NO2_print[7];    //print no2 string
char SO2_print[7];    //print so2 string
char Ozone_print[7];  //print Ozone string

DFRobot_OzoneSensor Ozone;
 
void setup()  {
  nh.initNode();
  nh.advertise(chatter);
  
  Serial.begin(9600);
  Serial2.begin(9600);//AGSM SO2 sensor module connection
  Serial3.begin(9600);//AGSM NO2 sensor module connection
  delay(1000);
//==================================================================================
//pin setting
  // Assign pins 0 & 1 SERCOM functionality
  pinPeripheral(0, PIO_SERCOM_ALT);
  pinPeripheral(1, PIO_SERCOM_ALT);

  // Assign pins 12 & 10 SERCOM functionality
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(10, PIO_SERCOM);
//==================================================================================
// module setting
  #if ContinueMode
    Serial2.write('c');
    Serial2.write('\r');
    Serial3.write('c');
    Serial3.write('\r');
  #endif

  while(!Ozone.begin(Ozone_IICAddress)) {
    chatter.publish("Ozone error");
    delay(1000);
  }
  
  Ozone.setModes(MEASURE_MODE_PASSIVE);
}
 
void loop() {
  unsigned long now = millis();
  //A0 is likely to be used for something inside, so it is easy to change the value
  int Wind_val = analogRead(A1);
  float Wind_outvoltage = Wind_val * (5.0 / 1023.0);
  int wind_speed = 6*outvoltage;
//==================================================================================
// 5sec check
  if(now - past >= 5000){
    past = now;
    flag = true;
  }
//==================================================================================
//SO2, NO2
  #if PollingMode
    Serial2.write('\r');
    Serial3.write('\r');
    delay(1000);
  #else
    delay(100);
  #endif

  int i = 0;
  String str_so2_data = "";
  String str_no2_data = "";
  int ozoneConcentration = Ozone.readOzoneData(COLLECT_NUMBER);
  //==================================================================================
//SO2, NO2 data get
  while (Serial2.available()) { // read from AGSM port, send to Serial port to interupt continuous output send 'c''/r' without line ending, may have to send more than once.
    SO2_data[i] = Serial2.read();
    i++;
  }
  i = 0;
  while (Serial3.available()) {
    NO2_data[i] = Serial3.read();
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
    
    float Ozone_ppm = (float)ozoneConcentration / 1000;
    
    //Function used by printf to use prime numbers
    //Decimal, total digits, number of representations after decimal point, string
    dtostrf(so2_ppm, 5, 3, SO2_print);
    dtostrf(no2_ppm, 5, 3, NO2_print);
    dtostrf(Ozone_ppm, 5, 3, Ozone_print);
    
    sprintf(buffer,"%s, %s, %s, %2d", SO2_print, NO2_print, Ozone_print, wind_speed);
    str.data = buffer;
    chatter.publish(&str);

    flag = false;
  }
  nh.spinOnce();
}
