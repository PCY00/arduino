/*
DFPlayer Mini 占쏙옙占싱브러占쏙옙
*/

#ifndef mp3_h
#define mp3_h

#include <Arduino.h>

// protocal
#define startBit						0x7E
#define VersionBit						0xFF
#define LenBit							0x06
#define FeedbackBit						0x00
#define EndBit							0xEF

// CMD
#define Next							0x01
#define Previous						0x02
#define SpecifyTracking					0x03
#define IncreaseVolume					0x04
#define DecreaseVolume					0x05
#define SpecifyVolume					0x06
#define SpecifyEQ						0x07
#define SpecifyPlaybackMode				0x08
#define SpecifyPlaybackSsourced			0x09
#define EnterIntoStandby				0x0A
#define NormalWorking					0x0B
#define ResetModule						0x0C
#define PlayBack						0x0D
#define Pause							0x0E
#define SpecifyFolderToPlayback			0x0F
#define VolumeAdjustSet					0x10
#define RepeatPlay						0x11

// CMD 2 (return)
#define QueryTheCurrentStatus			0x42
#define QueryTheCurrentVolume			0x43
#define QueryTheCurrentEQ				0x44
#define QueryTheCurrentPlayBackMode		0x45
#define QueryTheCurrentSoftwareVersion	0x46

// EQ
#define Nornal					0x00
#define Pop						0x01
#define Rock					0x02
#define Jazz					0x03
#define Classic					0x04
#define Base					0x05

// 반복 재생 ( 반복, 폴더 반복, 단일 곡 반복, 랜덤 재생 )
#define Repeat					0x01
#define Floder					0x02
#define SingleRepeat			0x03
#define Random					0x04

class mp3 {
private:

public:
	mp3();
	void sendCMD(uint8_t cmdBit, uint8_t DataPara1, uint8_t DataPara2);
	void volumeSet(uint16_t V);
	void selectTrack(uint16_t T);
	void next();
	void previous();
	void pause();
	void playback();
	void increasevolume();
	void decreasevolume();
	void specifyEQ(uint16_t EQ);
	void specifyplaybackmode(uint16_t mode);
	void standby();
};

#endif
