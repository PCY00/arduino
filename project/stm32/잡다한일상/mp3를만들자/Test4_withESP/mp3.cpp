#include "mp3.h"
#include <Arduino.h>

mp3::mp3() {

}

// send cmd
void mp3::sendCMD(uint8_t cmdBit, uint8_t DataPara1 = 0x00, uint8_t DataPara2 = 0x00) {
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

// 특정 트랙 재생
void mp3::selectTrack(uint16_t T) {
	sendCMD(SpecifyTracking, highByte(T), lowByte(T));
}

// 다음 곡 재생
void mp3::next() {
	sendCMD(Next);
}

// 이전 곡 재생
void mp3::previous() {
	sendCMD(Previous);
}

// 재생 일시 정시
void mp3::pause() {
	sendCMD(Pause);
}

// 재생 시작
void mp3::playback() {
	sendCMD(PlayBack);
}

// 볼륨 증가
void mp3::increasevolume() {
	sendCMD(IncreaseVolume);
}

// 볼륨 감소
void mp3::decreasevolume() {
	sendCMD(DecreaseVolume);
}

// 이퀄라이저 설정
void mp3::specifyEQ(uint16_t EQ) {
	sendCMD(SpecifyEQ, highByte(EQ), lowByte(EQ));
}

// 재생 모드 설정
void mp3::specifyplaybackmode(uint16_t mode) {
	sendCMD(SpecifyPlaybackMode, highByte(mode), lowByte(mode));
}

// 대기모드 ( 저전력 )
void mp3::standby() {
	sendCMD(EnterIntoStandby);
}

