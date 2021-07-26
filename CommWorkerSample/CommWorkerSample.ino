// CommWorkerのテスト用

#include <Arduino.h>
#include <MsTimer2.h>
#include "Controller.h"
#include "CommWorker.h"

#define SERIAL_CON      Serial5

Controller CON;
CommWorker UnderBody(&Serial0);

bool flag_10ms = false;
bool flag_100ms = false;

// setupで有効にされるタイマ割り込み処理が書いてある場所
void timer_warikomi(){    
  // RGB LED を良い感じに光らせるための処理
  static int count = 0;
  static int count_flag = 0;
  count += 2; // ここで光る周期を変えられる(はず)
  count_flag++;

  if(count < 255){
    analogWrite(PIN_LED_RED, count);
    analogWrite(PIN_LED_BLUE, 255 - count);
  }else if(count < 255 * 2){
    analogWrite(PIN_LED_GREEN, count - 255);
    analogWrite(PIN_LED_RED, 255 * 2 - count);
  }else if(count < 255 * 3){
    analogWrite(PIN_LED_BLUE, count - 255 * 2);
    analogWrite(PIN_LED_GREEN, 255 * 3 - count);
  }else{
    count = 0;
  }

  // フラグ立てるための処理
  flag_10ms = true;
  if(count_flag >= 10){
    flag_100ms = true;
    count_flag = 0;
  }
}

void setup()
{
  Serial.begin(115200);
  SERIAL_CON.begin(115200);
  
  pinMode(PIN_LED_USER, OUTPUT);
  analogWrite(PIN_LED_RED, 0); // 消しちゃダメ，ぜったい →　LPMSのために
  analogWrite(PIN_LED_BLUE, 0);
  analogWrite(PIN_LED_GREEN, 0);

  MsTimer2::set(10, timer_warikomi); // 10ms period
  MsTimer2::start();
}

void loop()
{
  // 10msに1回ピン情報を出力する
  if(flag_10ms){
    // 上半身との通信部分
    //UnderBody.send(0x44, 0x00); // 指令の送信
    
    if(UnderBody.recv()){
      Serial.print("button:");
      Serial.print(UnderBody.buttonState);
      Serial.print(", order:");
      Serial.print(UnderBody.recvOrder);
    }
    flag_10ms = false;
  }

  // 100msごとにLCDを更新する
  if(flag_100ms){
    
    flag_100ms = false;
  }
}