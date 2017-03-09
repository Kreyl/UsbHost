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

#define DEVICE_CNT          36
#define NO_REPLY_TIMEOUT_ms 4500
#define DEV_MODE_TRAINING   0
#define DEV_MODE_COMBAT     1

struct DevInfo_t {
    DevInfoData_t Data;
    systime_t Timestamp = 0;
    bool IsValid() {
        if(Timestamp == 0) return false;
        uint32_t msElapsed = ST2MS(chVTTimeElapsedSinceX(Timestamp));
        return (msElapsed < NO_REPLY_TIMEOUT_ms);
    }
//    void Print() { Uart.Printf("%u)
};

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

#if 1 // ========================= Device Manager ==============================
class DeviceMgr_t {
private:
    thread_reference_t ThdRef;
    uint8_t WaitCompletionI() {
        uint8_t r = FAILURE;
        if(chThdSuspendS(&ThdRef) == MSG_OK) {  // Awake ok, check reply
            r = Radio.LastPktRx.Data1;
        }
        return r;
    }
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
        chSysLock();
        QLen = 1;
        // Wait result
        uint8_t r = WaitCompletionI();
        chSysUnlock();
        return r;
    }
    uint8_t Cmd4Combat(RadioCmdType_t Cmd, uint8_t Data1 = 0, uint8_t Data2 = 0) {
        // Fill queue of commands
        QLen = 0;   // Just in case
        uint32_t Cnt = 0;
        for(int i=0; i<DEVICE_CNT; i++) {
            if(Info[i].IsValid() and Info[i].Data.Mode == DEV_MODE_COMBAT) {
                QPkt[Cnt].ID = i;
                QPkt[Cnt].Cmd = (uint8_t)Cmd;
                QPkt[Cnt].Data1 = Data1;
                QPkt[Cnt].Data2 = Data2;
                Cnt++;
            }
        } // for
        chSysLock();
        QLen = Cnt; // Do it here to make things atomic
        // Wait answer
        uint8_t r = OK;
        if(QLen != 0) r = WaitCompletionI();
        chSysUnlock();
        return r;
    }
    // Get pointer to pkt
    rPkt_t* GetNextPktFromQ() {
        if(QLen == 0) return nullptr;
        else {
            QLen--;
            return &QPkt[QLen];
        }
    }
    // Other thread will call this to awake us
    void Awake() {
        chSysLock();
        if(ThdRef->p_state == CH_STATE_SUSPENDED) chThdResumeI(&ThdRef, MSG_OK);
        chSysUnlock();
    }
};

extern DeviceMgr_t DevMgr;
#endif
#if 1 // Message structs
union MsgToDevice_t {
    uint32_t DWord[2];
    struct {
        uint8_t DevID, MsgID, ParamID, ParamV;
    } __packed;
} __packed;

union MsgToHost_t {
    struct {
        uint32_t DWord0, DWord1;
        uint16_t Word;
    } __packed;
    struct {
        uint8_t DevID, MsgID, MsgType;
        uint8_t Type, Group, HitCnt, ShotCnt, LastAttacker, State, GameMode;
    } __packed;
    void Printf(Shell_t *PShell) {
        PShell->Printf("%u %u %u %u %u %u %u %u %u %u\r\n",
                DevID, MsgID, MsgType,
                Type, Group, HitCnt, ShotCnt, LastAttacker, State, GameMode);
    }
} __packed;
#endif

// ==== Queues ====
#define Q2D_MSG_CNT     207

class QToDevices_t {
private:
    MsgToDevice_t IBuf[Q2D_MSG_CNT], *PRead=IBuf, *PWrite=IBuf;
public:
    int Cnt = 0;
    uint8_t Put(uint8_t DevID, uint8_t MsgID, uint8_t ParamID, uint8_t ParamV);
};
extern QToDevices_t QToDevices;

#define Q2H_MSG_CNT     99

class QToHost_t {
private:
    MsgToHost_t IBuf[Q2H_MSG_CNT], *PRead=IBuf, *PWrite=IBuf;
public:
    int Cnt = 0;
    uint8_t Put(uint8_t DevID, uint8_t MsgID, uint8_t Rslt);
    //uint8_t Put(uint8_t DevID, uint8_t MsgID, uint8_t Rslt, );
    uint8_t Get(MsgToHost_t *pmsg);
};
extern QToHost_t QToHost;
