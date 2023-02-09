#include "PMS.h"              //pms7003
#include <Arduino.h>          // required before wiring_private.h
#include "wiring_private.h"   // pinPeripheral() function
#include "DHT.h"              //DHT11

#define DHTPIN 6              // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11         // DHT 11
//#define DHTTYPE DHT22       // DHT 22  (AM2302), AM2321

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

char buffer[40];
unsigned long past = 0;
bool flag = false;
int d1, d2, d3;             //pms data1,2,3

void setup() {
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

void loop() {
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
    sprintf(buffer,"data1:%4d, data2:%4d, data3:%4d, temp: %2d, humi: %3d", d1, d2, d3, temp, humi);
    Serial.println(buffer);

    flag = false;
  }

//=======================================================================================
// buffer clear
  Serial.flush();
}
