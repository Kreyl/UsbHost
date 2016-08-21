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
#include "color.h"

#define APP_NAME            "UsbHost Generation"

#define ARMLET_CNT          2

struct ArmletSetup_t {
    Color_t Clr;
    int32_t BlinkOn, BlinkOff;
    uint8_t Vibro;
    ArmletSetup_t() : Clr(clGreen), BlinkOn(0), BlinkOff(0), Vibro(0) {}
    ArmletSetup_t& operator = (const ArmletSetup_t &Right) {
        Clr = Right.Clr;
        BlinkOn = Right.BlinkOn;
        BlinkOff = Right.BlinkOff;
        Vibro = Right.Vibro;
        return *this;
    }
};

extern ArmletSetup_t ArmletSetup [ARMLET_CNT];

class App_t {
private:
    thread_t *PThread;
public:
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
