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

#define APP_NAME            "UsbHost2_Robotlon"

#define DEVICE_CNT          33
#define NO_REPLY_TIMEOUT_ms 4500

struct DevInfo_t {
    union {
        uint32_t DWord;
        struct  {
            unsigned Type: 2;
            unsigned Group: 3;
            unsigned Mode: 1;
            unsigned State: 3;
            unsigned HitCnt: 8;
            unsigned LocalTime: 15;
        } __packed;
    } __packed;
    systime_t Timestamp = 0;
    bool IsValid() {
        if(Timestamp == 0) return false;
        uint32_t msElapsed = ST2MS(chVTTimeElapsedSinceX(Timestamp));
        return (msElapsed < NO_REPLY_TIMEOUT_ms);
    }
//    void Print() { Uart.Printf("%u)
};

class App_t {
private:
    thread_t *PThread;
public:
    DevInfo_t Info[DEVICE_CNT];
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
