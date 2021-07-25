/* 別マイコンとの通信用クラス */
#ifndef COMM_h
#define COMM_h

#include "Arduino.h"
#include "RZ_A1H.h"

//ボタンの番号定義
enum BUTTONS
{
    BUTTON_X = 0,
    BUTTON_Y,
    BUTTON_A,
    BUTTON_B,
    BUTTON_L1,
    BUTTON_R1,
    BUTTON_L2,
    BUTTON_R2,
    BUTTON_JOYL,
    BUTTON_JOYR,
    BUTTON_BACK,
    BUTTON_START,
    BUTTON_UP,
    BUTTON_RIGHT,
    BUTTON_DOWN,
    BUTTON_LEFT
};

//足回りに送る命令信号の定義
//0~255まで指定可能 必要あらば追加
enum master_order_
{
    initializing = 0,  //初期化中
    initialized,       //初期化完了
    all_launcehd,      //全部発射完了
    pickking,          //回収中
    pickuped,          //回収完了
    move_azimuth,      //方位角動作中
    set_pointed,       //照準セット完了
    stay,              //足回りその場停止の命令
    free_move,         //足回り移動制限解除
    goto_allo_wrack,   //アローラック前までの移動命令
    goto_launch_point, //発射点までの移動命令
    error_stop = 91    //エラー　停止の命令
};

class CommHost{
    public:
        Comm(HardwareSerial*);
        bool comm_flag;

        uint8_t serialSend(uint8_t order); //送信するための関数(3byte送信)
        bool serialRecv();                 //受信するための関数

        int master_order;        //マスターからの命令を格納する変数(1byte)
        //int controller_data;   //コントローラのデータを格納する変数(2byte)
        bool buttonState[BUTTONS]; //コントローラのボタンの情報を格納する配列
        //int controller[16] = {};  

    private:
        uint8_t send_str[3];
        uint8_t recv_str[5]; //受信した情報を格納するための変数

        HardwareSerial* serial; // 使うシリアル
};

#endif