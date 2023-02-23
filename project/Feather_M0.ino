#include "PMS.h"                //pms7003
#include <Arduino.h>            // required before wiring_private.h
#include "wiring_private.h"     // pinPeripheral() function
#include <avr/dtostrf.h>        //dtostrf
#include "DHT_T.h"              //DHT22
#include "DFRobot_OzoneSensor.h"

#define DHTPIN 6              // Digital pin connected to the DHT sensor

DHT dht(DHTPIN, DHT22);

#define COLLECT_NUMBER   20              // collect number, the collection range is 1-100
#define Ozone_IICAddress OZONE_ADDRESS_3

DFRobot_OzoneSensor Ozone;

//==================================================================================
//Add Serial
Uart Serial3 (&sercom2, 5, 4, SERCOM_RX_PAD_3, UART_TX_PAD_0);
void SERCOM2_Handler(){
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
char temp_print[7];
char humi_print[7];
char Ozone_print[7];  //print Ozone string

void setup() {
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
  pinPeripheral(5, PIO_SERCOM);
  pinPeripheral(4, PIO_SERCOM_ALT);

  Ozone.begin(Ozone_IICAddress);
  
  Ozone.setModes(MEASURE_MODE_PASSIVE);
}

void loop() {
  unsigned long now = millis();

  int ozoneConcentration = Ozone.readOzoneData(COLLECT_NUMBER);
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
    float Ozone_ppm = (float)ozoneConcentration / 1000;
    
    dtostrf(temp, 5, 2, temp_print);
    dtostrf(humi, 5, 2, humi_print);
    dtostrf(Ozone_ppm, 5, 3, Ozone_print);
    sprintf(buffer,"data1:%4d, data2:%4d, data3:%4d, temp: %s, humi: %s, Ozone: %s", d1, d2, d3, temp_print, humi_print, Ozone_print);
    Serial.println(buffer);

    flag = false;
  }

  Serial.flush();
}
