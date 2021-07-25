/* 別マイコンとの通信用クラス */

#include "CommHost.h"

CommHost::CommHost(HardwareSerial* xserial){
    serial = xserial;
    serial->begin(115200);
    recvOrder = 0;
}

//足回りに送信するための関数(3byte送信)
uint8_t CommHost::send(uint16_t buttonState, uint8_t sendOrder1, uint8_t sendOrder2)
{
    uint8_t send_str[6] = {};
    
    send_str[0] = (uint8_t)(buttonState >> 8 & 0xFF);
    send_str[1] = (uint8_t)buttonState;
    send_str[2] = sendOrder1;
    send_str[3] = sendOrder2;

    send_str[4] = ( (send_str[0] ^ send_str[1]) ^ send_str[2]) ^ send_str[3];
    send_str[5] = 0xB4;
    for (int i = 0; i < 6; i++)
    {
        serial->write(send_str[i]);
    }
    
    return true;
}

bool CommHost::recv()        //足回りから受信するための関数※10msに1回実行される想定
{
    uint8_t recv_str[4] = {};
    if (serial->available() > 0) //バッファにデータがあれば実行
    {
        for (int i = 0; i < 4; i++)
        {
            recv_str[i] = serial->read(); //1byteずつバッファから取りだす
        }
        if (recv_str[3] == 0xB4) //終端文字があるかの判別
        {
            if (recv_str[0] == recv_str[1] ^ recv_str[2]) //チェックサムの計算
            {
                recvOrder = recv_str[1] | recv_str[2] << 8; //上半身からの命令を格納
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}