/* 別マイコンとの通信用クラス */

#include "CommHost.h"

CommHost::CommHost(HardwareSerial* xserial){
    serial = xserial;
    serial->begin(115200);
    master_order = 0;
    for(int i = 0; i < BUTTONS; i++) buttonState[i] = false;
}

//足回りに送信するための関数(3byte送信)
uint8_t CommHost::serialSend(uint8_t order)
{
    send_str[0] = order;       //足回りへの命令
    send_str[1] = send_str[0]; //チェックサム
    send_str[2] = 0xB4;        //終端文字
    for (int i = 0; i < 3; i++)
    {
        serial->write(send_str[i]);
    }
    
    return true;
}

bool CommHost::serialRecv()        //足回りから受信するための関数
{
    bool recv_flag = false;            //受信成功を示すためのフラグ
    if (serial->available() > 0) //バッファにデータがあれば実行
    {
        for (int i = 0; i < 5; i++)
        {
            recv_str[i] = serial->read(); //1byteずつバッファから取りだす
        }
        if (recv_str[4] == 0xB4) //終端文字があるかの判別
        {
            if (recv_str[3] == recv_str[0] ^ send_str[1] ^ send_str[2]) //チェックサムの計算
            {
                int controller_data = (int)(recv_str[0] << 8 | recv_str[1]); //コントローラのボタン配列の組み立て
                master_order = (int)recv_str[2];                         //足回りからの命令を格納
                for (int i_R2 = 0; i_R2 < 16; i_R2++)
                {
                    controller[i_R2] = controller_data >> i_R2 & 0X0001; //ボタン情報を配列化
                }
            }
        }
        else
        {
            recv_flag = false;
        }
    }
    return recv_flag;
}