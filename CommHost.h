/* 別マイコンとの通信用クラス */
#ifndef COMMHOST_h
#define COMMHOST_h

#include "Arduino.h"
#include "RZ_A1H.h"

class CommHost{
    public:
        CommHost(HardwareSerial*);
        uint8_t send(uint16_t buttonState, uint8_t sendOrder1, uint8_t sendOrder2); //送信するための関数(3byte送信)
        bool recv();                 //受信するための関数
        unsigned int recvOrder;
    private:
        HardwareSerial* serial; // 使うシリアル
};

#endif