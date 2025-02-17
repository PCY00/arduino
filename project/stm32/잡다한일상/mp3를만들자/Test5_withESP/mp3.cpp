#include "mp3.h"
#include <Arduino.h>

mp3::mp3(Stream& serial) : _serial(&serial) {}

void mp3::sendCMD(uint8_t cmdBit, uint8_t DataPara1, uint8_t DataPara2) {
    if (!_serial) return;

    uint16_t DataBit = VersionBit + LenBit + cmdBit + FeedbackBit + DataPara1 + DataPara2;
    uint16_t checksum = 0xFFFF - DataBit + 1;

    uint8_t writeCMD[10] = { startBit, VersionBit, LenBit, cmdBit, FeedbackBit, DataPara1, DataPara2, highByte(checksum), lowByte(checksum), EndBit };

    for (int i = 0; i < 10; i++) {
        _serial->write(writeCMD[i]);
    }
}

String mp3::getCMD() {
    uint8_t re[10];

    unsigned long startTime = millis();
    while (_serial->available() < 10) {
        if (millis() - startTime > 500) {  // 500ms
            Serial.println("Error: Timeout while waiting for response.");
            return "error";
        }
    }

    for (int i = 0; i < 10; i++) {
        re[i] = _serial->read();
    }

    switch (re[3]) {
    case QueryTheCurrentStatus:
        return parseStatus(re[5], re[6]);
    case QueryTheCurrentVolume:
        return "Volume_" + String(re[6]);
    case QueryTheCurrentEQ:
        return "EQ_" + String(re[6]);
    case QueryTheCurrentPlayBackMode:
        return "PlaybackMode_" + String(re[6]);
    case QueryTheCurrentSoftwareVersion:
        return "SoftwareVersion_" + String(re[6]);
    default:
        return "error";
    }
}

String mp3::parseStatus(uint8_t highData, uint8_t lowData) {
    struct DataMapping {
        uint8_t highData;
        uint8_t lowData;
        const char* reData;
    };

    static const DataMapping datamap[] = {
        {0x01, 0x00, "flash_Stopped"},
        {0x01, 0x01, "flash_Playing"},
        {0x01, 0x02, "flash_Paused"},
        {0x02, 0x00, "SDmode_Stopped"},
        {0x02, 0x01, "SDmode_Playing"},
        {0x02, 0x02, "SDmode_Paused"},
        {0x10, 0x00, "Module_In_Sleep"},
    };

    for (const auto& entry : datamap) {
        if (entry.highData == highData && entry.lowData == lowData) {
            return entry.reData;
        }
    }

    return "I_Dont_Know";

}

void mp3::volumeSet(uint16_t V) {
    sendCMD(SpecifyVolume, highByte(V), lowByte(V));
}

void mp3::selectTrack(uint16_t T) {
    sendCMD(SpecifyTracking, highByte(T), lowByte(T));
}

void mp3::next() {
    sendCMD(Next);
}

void mp3::previous() {
    sendCMD(Previous);
}

void mp3::pause() {
    sendCMD(Pause);
}

void mp3::playback() {
    sendCMD(PlayBack);
}

void mp3::increasevolume() {
    sendCMD(IncreaseVolume);
}

void mp3::decreasevolume() {
    sendCMD(DecreaseVolume);
}

void mp3::specifyEQ(uint16_t EQ) {
    sendCMD(SpecifyEQ, highByte(EQ), lowByte(EQ));
}

void mp3::specifyplaybackmode(uint16_t mode) {
    sendCMD(SpecifyPlaybackMode, highByte(mode), lowByte(mode));
}

void mp3::standby() {
    sendCMD(EnterIntoStandby);
}

void mp3::querythecurrentstatus() {
    sendCMD(QueryTheCurrentStatus);
    String str = getCMD();
    
    Serial.println(str);
}

void mp3::querythecurrentvolume() {
    sendCMD(QueryTheCurrentVolume);
    String str = getCMD();
    Serial.println(str);
}
