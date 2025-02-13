#include <SoftwareSerial.h>

SoftwareSerial softSerial(10,11);

// protocal
#define startBit 0x7E
#define VersionBit 0xFF
#define LenBit 0x06
#define FeedbackBit 0x00
#define EndBit 0xEF

// CMD
#define Next 0x01
#define Previous 0x02
#define SpecifyTracking 0x03
#define IncreaseVolume 0x04
#define DecreaseVolume 0x05
#define SpecifyVolume  0x06
#define SpecifyEQ 0x07
#define SpecifyPlaybackMode 0x08
#define SpecifyPlaybackSsourced 0x09
#define EnterIntoStandby 0x0A
#define NormalWorking 0x0B
#define ResetModule 0x0C
#define PlayBack 0x0D
#define Pause 0x0E
#define SpecifyFolderToPlayback 0x0F
#define VolumeAdjustSet 0x10
#define RepeatPlay 0x11

uint16_t checkSum(uint8_t CmdBit, uint8_t DataPara1, uint8_t DataPara2){
  // checksum = 0xFFFF - (6bit) + 1
  uint16_t DataBit = VersionBit + LenBit + CmdBit + FeedbackBit + DataPara1 + DataPara2;
  uint16_t C = 0xFFFF - DataBit + 1;
  return C;
}

void volumeSet(int V){
  uint16_t checksum = checkSum(SpecifyVolume, 0x00, V);
  uint8_t writeCMD[10] = {startBit, VersionBit, LenBit, SpecifyVolume, FeedbackBit, 0x00, V, highByte(checksum), lowByte(checksum), EndBit};
  for(int i = 0; i < 10; i++){
    softSerial.write(writeCMD[i]);
    Serial.print("0x");
    Serial.print(writeCMD[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void selectTrack(int T){
  uint16_t checksum = checkSum(SpecifyTracking, 0x00, T);
  uint8_t writeCMD[10] = {startBit, VersionBit, LenBit, SpecifyTracking, FeedbackBit, 0x00, T, highByte(checksum), lowByte(checksum), EndBit};
  for(int i = 0; i < 10; i++){
    softSerial.write(writeCMD[i]);
    Serial.print("0x");
    Serial.print(writeCMD[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup(){
  Serial.begin(115200);
  softSerial.begin(9600);
  delay(3000);
  volumeSet(10);
  delay(100);
  selectTrack(1);
}

void loop(){

}

