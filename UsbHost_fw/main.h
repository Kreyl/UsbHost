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

#define DEVICE_CNT          31
#define NO_REPLY_TIMEOUT_ms 4500
#define DEV_MODE_TRAINING   0
#define DEV_MODE_COMBAT     1

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

class DeviceMgr_t {
public:
    // ==== Device info ====
    DevInfo_t Info[DEVICE_CNT];
    // ==== Command handling ====
    uint32_t QLen=0;
    rPkt_t QPkt[DEVICE_CNT];
    uint8_t Cmd4One(uint8_t DevID, RadioCmdType_t Cmd, uint8_t Data1, uint8_t Data2 = 0) {
        // Prepare rPkt to tx
        QPkt[0].ID = DevID;
        QPkt[0].Cmd = (uint8_t)Cmd;
        QPkt[0].Data1 = Data1;
        QPkt[0].Data2 = Data2;
        QLen = 1;
        // Wait result
        return 0;
    }
    uint8_t Cmd4Combat(RadioCmdType_t Cmd, uint8_t Data1 = 0, uint8_t Data2 = 0) {
        // Fill queue of commands
        QLen=0;
        for(int i=0; i<DEVICE_CNT; i++) {
            if(Info[i].IsValid() and Info[i].Mode == DEV_MODE_COMBAT) {
                QPkt[QLen].ID = i;
                QPkt[QLen].Cmd = (uint8_t)Cmd;
                QPkt[QLen].Data1 = Data1;
                QPkt[QLen].Data2 = Data2;
                QLen++;
            }
        } // for
        // Wait answer
        if(QLen != 0) {

        }
        return 0;
    }
};

extern DeviceMgr_t DevMgr;

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
