/* 別マイコンとの通信用クラス */

#include "CommWorker.h"

CommWorker::CommWorker(HardwareSerial* xserial){
    serial = xserial;
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
    
    serial->write(send_str, 4);
    
    return true;
}

bool CommWorker::recv()        //足回りから受信するための関数
{
    uint8_t recv_str[10];
  if (serial->available() > 0) //バッファにデータがあれば実行※データは6個であることが前提
  {
    for(int i = 0; i < 7; i++){
      recv_str[i] = serial->read(); //1byteずつバッファから取りだす
    }
    if (recv_str[6] == 0xB4) //終端文字があるかの判別
    {
      uint8_t checksum = 0;
      for(int i = 0; i < 5; i++){
        checksum ^= recv_str[i];
      }

      if (recv_str[5] == checksum) //チェックサムの計算
      {
        buttonState = (recv_str[0] | (recv_str[1] << 8) | (recv_str[2] << 16)); //コントローラのボタン配列の組み立て
        recvOrder = recv_str[3] | (recv_str[4] << 8); //足回りからの命令を格納
      }
      return true;
    }else{
      return false;
    }
  }else{
    while(serial->available() > 0){
      serial->read();
    }
  }
  
  return false;
}