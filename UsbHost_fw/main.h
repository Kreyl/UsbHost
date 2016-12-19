/*
 * main.h
 *
 *  Created on: 26 дек. 2015 г.
 *      Author: Kreyl
 */

#pragma once

#include "ch.h"
#include "kl_lib.h"
#include "uart.h"
#include "evt_mask.h"
#include "board.h"
#include "radio_lvl1.h"

#define APP_NAME            "UsbHost2_Robotlon"

#define DEVICE_CNT          32
#define NO_REPLY_TIMEOUT_ms 4500
#define DEV_MODE_TRAINING   0
#define DEV_MODE_COMBAT     1

//struct DevInfo_t {
//    systime_t Timestamp = 0;
//    bool IsValid() {
//        if(Timestamp == 0) return false;
//        uint32_t msElapsed = ST2MS(chVTTimeElapsedSinceX(Timestamp));
//        return (msElapsed < NO_REPLY_TIMEOUT_ms);
//    }
////    void Print() { Uart.Printf("%u)
//};

class App_t {
public:
    thread_t *PThread;
    // Eternal methods
    void InitThread() { PThread = chThdGetSelfX(); }
    void SignalEvt(eventmask_t Evt) {
        chSysLock();
        chEvtSignalI(PThread, Evt);
        chSysUnlock();
    }
    void SignalEvtI(eventmask_t Evt) { chEvtSignalI(PThread, Evt); }
    void OnCmd(Shell_t *PShell);
    // Inner use
    void ITask();
};

extern App_t App;
