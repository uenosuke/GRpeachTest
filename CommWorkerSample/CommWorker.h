/* 別マイコンとの通信用クラス */
#ifndef COMMWORKER_h
#define COMMWORKER_h

#include "Arduino.h"
#include "RZ_A1H.h"

// 以下は足回り側にあるコントローラの定義
#define CON_ADACHI    ( 0 )
#define CON_ELECOM    ( 1 )
#define CON_DS4       ( 2 )

#define CON_TYPE  ( CON_ELECOM ) // ここを足回りと合わせる

#if CON_TYPE == CON_ADACHI
    #define MASK_BUTTON_UP    0x01
    #define MASK_BUTTON_RIGHT 0x02
    #define MASK_BUTTON_DOWN  0x04
    #define MASK_BUTTON_LEFT  0x08

    #define MASK_BUTTON_R1    0x10
    #define MASK_BUTTON_R2    0x20
    #define MASK_BUTTON_L1    0x40
    #define MASK_BUTTON_L2    0x80

    // Uiコンにはこれらは無いが，上下左右と共通化してプログラムは動くようにさせる
    #define MASK_BUTTON_X  0x01
    #define MASK_BUTTON_Y  0x02
    #define MASK_BUTTON_A  0x04
    #define MASK_BUTTON_B  0x08
    // Uiコンにはこれらは無いが，上下左右と共通化してプログラムは動くようにさせる
    #define MASK_BUTTON_SHIKAKU 0x01
    #define MASK_BUTTON_SANKAKU 0x02
    #define MASK_BUTTON_BATU    0x04
    #define MASK_BUTTON_MARU    0x08

    #define BUTTON_UP    1
    #define BUTTON_RIGHT 2
    #define BUTTON_DOWN  3
    #define BUTTON_LEFT  4
    #define BUTTON_R1    5
    #define BUTTON_R2    6
    #define BUTTON_L1    7
    #define BUTTON_L2    8
#elif CON_TYPE == CON_ELECOM || CON_TYPE == CON_DS4
    #define MASK_BUTTON_X  0x0001
    #define MASK_BUTTON_Y  0x0002
    #define MASK_BUTTON_A  0x0004
    #define MASK_BUTTON_B  0x0008

    #define MASK_BUTTON_SHIKAKU 0x0001
    #define MASK_BUTTON_SANKAKU 0x0002
    #define MASK_BUTTON_BATU    0x0004
    #define MASK_BUTTON_MARU    0x0008

    #define MASK_BUTTON_L1     0x0010
    #define MASK_BUTTON_R1     0x0020
    #define MASK_BUTTON_L2     0x0040
    #define MASK_BUTTON_R2     0x0080

    #define MASK_BUTTON_PS     0x0100 // PS4のときはPSボタン
    #define MASK_BUTTON_PAD    0x0200 // PS4のときはパッド
    #define MASK_BUTTON_JOY_L   0x0100
    #define MASK_BUTTON_JOY_R   0x0200
    #define MASK_BUTTON_BACK    0x0400
    #define MASK_BUTTON_START   0x0800

    #define MASK_BUTTON_UP     0x1000
    #define MASK_BUTTON_RIGHT  0x2000
    #define MASK_BUTTON_DOWN   0x4000
    #define MASK_BUTTON_LEFT   0x8000

    #define BUTTON_UP    12
    #define BUTTON_RIGHT 13
    #define BUTTON_DOWN  14
    #define BUTTON_LEFT  15
    #define BUTTON_R1    5
    #define BUTTON_R2    7
    #define BUTTON_L1    4
    #define BUTTON_L2    6
#endif

class CommWorker{
    public:
        CommWorker(HardwareSerial*);
        uint8_t send(uint8_t sendOrder1, uint8_t sendOrder2); //送信するための関数(3byte送信)
        bool recv();                 //受信するための関数

        unsigned int recvOrder;
        unsigned int buttonState; //コントローラのボタンの情報を格納する変数
    private:
        HardwareSerial* serial; // 使うシリアル
};

#endif