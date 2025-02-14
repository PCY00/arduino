#include "mp3.h"
#include <Arduino.h>

mp3::mp3() {

}

// send cmd
void mp3::sendCMD(uint8_t cmdBit, uint8_t DataPara1, uint8_t DataPara2) {
	// checksum = 0xFFFF - (6bit) + 1
	uint16_t DataBit = VersionBit + LenBit + cmdBit + FeedbackBit + DataPara1 + DataPara2;
	uint16_t checksum = 0xFFFF - DataBit + 1;

	uint8_t writeCMD[10] = { startBit, VersionBit, LenBit, cmdBit, FeedbackBit, DataPara1, DataPara2, highByte(checksum), lowByte(checksum), EndBit };
	for (int i = 0; i < 10; i++) {
		Serial1.write(writeCMD[i]);
	}
}

// input 0 ~ 30 Value
void mp3::volumeSet(uint16_t V) {
	sendCMD(SpecifyVolume, highByte(V), lowByte(V));
}

void mp3::selectTrack(uint16_t T) {
	sendCMD(SpecifyTracking, highByte(T), lowByte(T));
}
