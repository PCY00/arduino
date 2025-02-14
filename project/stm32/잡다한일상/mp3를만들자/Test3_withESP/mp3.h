/*
DFPlayer Mini 占쏙옙占싱브러占쏙옙
*/

#ifndef mp3_h
#define mp3_h

#include <Arduino.h>

// protocal
#define startBit				0x7E
#define VersionBit				0xFF
#define LenBit					0x06
#define FeedbackBit				0x00
#define EndBit					0xEF

// CMD
#define Next					0x01
#define Previous				0x02
#define SpecifyTracking			0x03
#define IncreaseVolume			0x04
#define DecreaseVolume			0x05
#define SpecifyVolume			0x06
#define SpecifyEQ				0x07
#define SpecifyPlaybackMode		0x08
#define SpecifyPlaybackSsourced 0x09
#define EnterIntoStandby		0x0A
#define NormalWorking			0x0B
#define ResetModule				0x0C
#define PlayBack				0x0D
#define Pause					0x0E
#define SpecifyFolderToPlayback 0x0F
#define VolumeAdjustSet			0x10
#define RepeatPlay				0x11

class mp3 {
private:

public:
	mp3();
	void sendCMD(uint8_t cmdBit, uint8_t DataPara1, uint8_t DataPara2);
	void volumeSet(uint16_t V);
	void selectTrack(uint16_t T);
};

#endif
