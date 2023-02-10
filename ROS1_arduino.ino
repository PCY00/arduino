#define USE_USBCON

#include <ros.h>
#include <std_msgs/String.h>
#include <PMS.h>
#include <TimeLib.h>
#include <Arduino.h>
#include "wiring_private.h"
#include "DHT.h"

#define DHTPIN 6
#define DHTTYPE DHT11


ros::NodeHandle  nh;

std_msgs::String str;
ros::Publisher chatter("str_data", &str);

//=======================================================================================
// make serial Rx, Tx
Uart Serial3 (&sercom3, 12, 13, SERCOM_RX_PAD_3, UART_TX_PAD_2);
void SERCOM3_Handler()
{
  Serial3.IrqHandler();
}

Uart Serial2 (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}
//=======================================================================================
//PMS
PMS pms1(Serial3);
PMS::DATA data1;

PMS pms2(Serial2);
PMS::DATA data2;

PMS pms3(Serial1);
PMS::DATA data3;
//=======================================================================================
//dht
DHT dht(DHTPIN, DHTTYPE);

char buffer[200];
unsigned long past = 0;
bool flag = false;
int d1, d2, d3;             //pms data1,2,3



void setup()
{
  nh.initNode();
  nh.advertise(chatter);
  
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.begin(9600);
  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);

  // Assign pins 12 & 13 SERCOM functionality
  pinPeripheral(12, PIO_SERCOM_ALT);
  pinPeripheral(13, PIO_SERCOM_ALT);

  //dht start
  dht.begin();
}

void loop()
{

int time_now = now();
  
  //=======================================================================================
//dht value
  int temp = (int)dht.readTemperature();
  int humi = (int)dht.readHumidity();
//=======================================================================================
// timer 5sec
  unsigned long now = millis();
  
  if(now - past >= 5000){
    past = now;
    flag = true;
  }
//=======================================================================================
//pms read  
  if (pms1.read(data1)){
    d1 = data1.PM_AE_UG_2_5;
  }
  if (pms2.read(data2)){
    d2 = data2.PM_AE_UG_2_5;
  }
  if(pms3.read(data3)){
    d3 = data3.PM_AE_UG_2_5;
  }
//=======================================================================================
//After 5sec print
  if(flag == true){
    sprintf(buffer,"%d, %4d,%4d,%4d,%2d,%3d", time_now, d1, d2, d3, temp, humi);
    
    str.data = buffer;
    chatter.publish(&str);
    flag = false;
  }

//=======================================================================================
  nh.spinOnce();
  delay(1000);
}
