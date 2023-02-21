#define USE_USBCON

#include <ros.h>
#include <std_msgs/String.h>-

#include "PMS.h"              //pms7003
#include <Arduino.h>          // required before wiring_private.h
#include "wiring_private.h"   // pinPeripheral() function
#include <avr/dtostrf.h>    //dtostrf
#include "DHT_T.h"             //DHT22

#define DHTPIN 6              // Digital pin connected to the DHT sensor

ros::NodeHandle  nh;

std_msgs::String str;
ros::Publisher chatter("str_data", &str);

DHT dht(DHTPIN, DHT22);

//==================================================================================
//Add Serial
Uart Serial3 (&sercom3, 13, 11, SERCOM_RX_PAD_1, UART_TX_PAD_0);
void SERCOM3_Handler()
{
  Serial3.IrqHandler();
}

Uart Serial2 (&sercom1, 12, 10, SERCOM_RX_PAD_3, UART_TX_PAD_2);
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}
//==================================================================================
//pms
PMS pms1(Serial3);
PMS::DATA data1;

PMS pms2(Serial2);
PMS::DATA data2;

PMS pms3(Serial1);
PMS::DATA data3;
//==================================================================================
// var
char buffer[256];
unsigned long past = 0;
bool flag = false;
//pms var
int d1, d2, d3;
//dht var
float temp, humi;
//co var
float co_ppm;
char co[20];
 
void setup()  {
  nh.initNode();
  nh.advertise(chatter);
  
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.begin(9600);
//==================================================================================
//pin setting
  // Assign pins 12 & 10 SERCOM functionality
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(10, PIO_SERCOM);

  // Assign pins 13 & 11 SERCOM functionality
  pinPeripheral(13, PIO_SERCOM_ALT);
  pinPeripheral(11, PIO_SERCOM_ALT);
}
 
void loop() {
  unsigned long now = millis();
//==================================================================================
//co ppm value
  int co_val;
  co_val = analogRead(0);
  co_ppm = (float)co_val / 1000;
  
//==================================================================================
// 5sec check
  if(now - past >= 5000){
    past = now;
    flag = true;
  }
//==================================================================================
// pms val read
  if (pms1.read(data1)){
    d1 = data1.PM_AE_UG_2_5;
  }
  if (pms2.read(data2)){
    d2 = data2.PM_AE_UG_2_5;
  }
  if(pms3.read(data3)){
    d3 = data3.PM_AE_UG_2_5;
  }

//==================================================================================
// dht val read
  switch(dht.read())
  {
    case DHT_OK:
      humi = dht.humidity;
      temp = dht.temperature;
      break;
    case DHT_ERR_CHECK:
        Serial.println("DHT CHECK ERROR");break;
    case DHT_ERR_TIMEOUT:
        Serial.println("DHT TIMEOUT EEROR");break;
    default:
        Serial.println("UNKNOWN EEROR");break;
   }

//==================================================================================
// After 5sec print
  if(flag == true){
    dtostrf(co_ppm, 4, 2, co);
    sprintf(buffer,"%4d, %4d, %4d, %2d, %3d, co: %s", d1, d2, d3, temp, humi, co);

    str.data = buffer;
    chatter.publish(&str);

    flag = false;
  }
  nh.spinOnce();
}
