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

#define DEVICE_CNT          33
#define DEV_ID_MIN          1
#define DEV_ID_MAX          32

#define NO_REPLY_TIMEOUT_ms 4500
#define DEV_MODE_TRAINING   0
#define DEV_MODE_COMBAT     1

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

#if 1 // ======================== Message structs ==============================
union MsgToDevice_t {
    uint32_t DWord;
    struct {
        uint8_t DevID, MsgID, ParamID, ParamV;
    } __packed;
    MsgToDevice_t& operator = (const MsgToDevice_t &Right) {
        DWord = Right.DWord;
        return *this;
    }
    void Printf() {
        Uart.Printf("%u %u %u %u\r", DevID, MsgID, ParamID, ParamV);
    }
} __packed;

union MsgToHost_t {
    struct {
        uint32_t DWord0, DWord1;
        uint8_t IByte;
    } __packed;
    struct {
        uint8_t DevID, MsgID, MsgType;
        uint8_t Type, Group, HitCnt, ShotCnt, LastAttacker, State;
    } __packed;

    MsgToHost_t() : DWord0(0), DWord1(0), IByte(0) {}
    MsgToHost_t(uint8_t ADevID, uint8_t AMsgID, uint8_t AMsgType) :
        DevID(ADevID), MsgID(AMsgID), MsgType(AMsgType) {}
    void SetInfo(DevInfo_t &info) {
        Type = info.Type;
        Group = info.Group;
        HitCnt = info.HitCnt;
        ShotCnt = info.ShotCnt;
        LastAttacker = info.LastAttacker;
        State = info.State;
    }
    MsgToHost_t& operator = (const MsgToHost_t &Right) {
        DWord0 = Right.DWord0;
        DWord1 = Right.DWord1;
        IByte = Right.IByte;
        return *this;
    }
    void Printf(Shell_t *PShell) {
        PShell->Printf("%u %u %u %u %u %u %u %u %u\r\n",
                DevID, MsgID, MsgType,
                Type, Group, HitCnt, ShotCnt, LastAttacker, State);
    }
} __packed;
#endif

// ==== Queues ====
#define Q2D_MSG_CNT     207

class QToDevices_t {
private:
    MsgToDevice_t IBuf[Q2D_MSG_CNT], *PRead=IBuf, *PWrite=IBuf;
    int Cnt = 0;
public:
    uint8_t Put(uint8_t DevID, uint8_t MsgID, uint8_t ParamID, uint8_t ParamV);
    uint8_t Get(MsgToDevice_t *PMsg);
};
extern QToDevices_t QToDevices;

#define Q2H_MSG_CNT     207

class QToHost_t {
private:
    MsgToHost_t IBuf[Q2H_MSG_CNT], *PRead=IBuf, *PWrite=IBuf;
public:
    int Cnt = 0;
    uint8_t Put(uint8_t DevID, uint8_t MsgID, uint8_t Rslt);
    uint8_t Put(MsgToHost_t &Msg);
    void AddOrReplaceSameID(MsgToHost_t &Msg);
    uint8_t Get(MsgToHost_t *pmsg);
};
extern QToHost_t QToHost;
