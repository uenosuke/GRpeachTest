/* 別マイコンとの通信用クラス */

#include "CommWorker.h"

CommWorker::CommWorker(HardwareSerial* xserial){
    serial = xserial;
    serial->begin(115200);
    recvOrder = 0;
}

//足回りに送信するための関数(3byte送信)
uint8_t CommWorker::send(uint8_t sendOrder1, uint8_t sendOrder2)
{
    uint8_t send_str[4];
    send_str[0] = sendOrder1;  //足回りへの命令1
    send_str[1] = sendOrder2;  //足回りへの命令1
    send_str[2] = send_str[0] ^ send_str[1]; //チェックサム
    send_str[3] = 0xB4;        //終端文字
    for (int i = 0; i < 4; i++)
    {
        serial->write(send_str[i]);
    }
    
    return true;
}

bool CommWorker::recv()        //足回りから受信するための関数
{
    uint8_t recv_str[6];
    if (serial->available() > 0) //バッファにデータがあれば実行
    {
        for (int i = 0; i < 6; i++)
        {
            recv_str[i] = serial->read(); //1byteずつバッファから取りだす
        }
        if (recv_str[5] == 0xB4) //終端文字があるかの判別
        {
            if (recv_str[4] == (((recv_str[0] ^ recv_str[1]) ^ recv_str[2]) ^ recv_str[3])) //チェックサムの計算
            {
                buttonState = (recv_str[0] << 8 | recv_str[1]); //コントローラのボタン配列の組み立て
                recvOrder = recv_str[2] | (recv_str[3] << 8); //足回りからの命令を格納
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}