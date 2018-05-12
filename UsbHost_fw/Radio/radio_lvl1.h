/*
 * radio_lvl1.h
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#pragma once

#include "kl_lib.h"
#include "ch.h"
#include "cc1101.h"
#include "kl_buf.h"
#include "shell.h"
#include "MsgQ.h"

#if 0 // ========================= Signal levels ===============================
// Python translation for db
#define RX_LVL_TOP      1000
// Jolaf: str(tuple(1 + int(sqrt(float(i) / 65) * 99) for i in xrange(0, 65 + 1)))
//const int32_t dBm2Percent1000Tbl[66] = {10, 130, 180, 220, 250, 280, 310, 330, 350, 370, 390, 410, 430, 450, 460, 480, 500, 510, 530, 540, 550, 570, 580, 590, 610, 620, 630, 640, 650, 670, 680, 690, 700, 710, 720, 730, 740, 750, 760, 770, 780, 790, 800, 810, 820, 830, 840, 850, 860, 860, 870, 880, 890, 900, 910, 920, 920, 930, 940, 950, 960, 960, 970, 980, 990, 1000};
const int32_t dBm2Percent1000Tbl[86] = {
         10, 117, 162, 196, 225, 250, 273, 294, 314, 332,
        350, 366, 382, 397, 412, 426, 440, 453, 466, 478,
        490, 502, 514, 525, 536, 547, 558, 568, 578, 588,
        598, 608, 617, 627, 636, 645, 654, 663, 672, 681,
        689, 698, 706, 714, 722, 730, 738, 746, 754, 762,
        769, 777, 784, 792, 799, 806, 814, 821, 828, 835,
        842, 849, 856, 862, 869, 876, 882, 889, 895, 902,
        908, 915, 921, 927, 934, 940, 946, 952, 958, 964,
        970, 976, 982, 988, 994, 1000
};

static inline int32_t dBm2Percent(int32_t Rssi) {
    if(Rssi < -100) Rssi = -100;
    else if(Rssi > -15) Rssi = -15;
    Rssi += 100;    // 0...85
    return dBm2Percent1000Tbl[Rssi];
}

// Conversion Lvl1000 <=> Lvl250
#define Lvl1000ToLvl250(Lvl1000) ((uint8_t)((Lvl1000 + 3) / 4))

static inline void Lvl250ToLvl1000(uint16_t *PLvl) {
    *PLvl = (*PLvl) * 4;
}

// Sensitivity Constants, percent [1...1000]. Feel if RxLevel > SnsConst.
#define RLVL_NEVER              10000
#define RLVL_2M                 800     // 0...4m
#define RLVL_4M                 700     // 1...20m
#define RLVL_10M                600
#define RLVL_50M                1
#define RLVL_PELENGATOR         RLVL_4M // LED will lit if rlevel is higher

#endif

#define CC_TX_PWR   CC_PwrPlus5dBm

#if 1 // =========================== Pkt_t =====================================
union AccSpd_t {
    uint32_t DWord[3];
    struct {
        int16_t g[3];
        int16_t a[3];
    } __packed;
    void Print() { Printf("%d %d %d; %d %d %d;   ", a[0],a[1],a[2], g[0],g[1],g[2]); }
    AccSpd_t& operator = (const AccSpd_t &Right) {
        DWord[0] = Right.DWord[0];
        DWord[1] = Right.DWord[1];
        DWord[2] = Right.DWord[2];
        return *this;
    }
} __packed;

struct rPktAcg_t  {
    uint8_t Length;
    AccSpd_t Acg[6];
    rPktAcg_t& operator = (const rPktAcg_t &Right) {
        Length = Right.Length;
        Acg[0] = Right.Acg[0];
        Acg[1] = Right.Acg[1];
        Acg[2] = Right.Acg[2];
        Acg[3] = Right.Acg[3];
        Acg[4] = Right.Acg[4];
        Acg[5] = Right.Acg[5];
        return *this;
    }
    void Print() {
        for(int i=0; i<6; i++) Acg[i].Print();
        PrintfEOL();
    }
} __packed;

#define RPKTACG_LEN    sizeof(rPktAcg_t)
struct rPktReply_t {
    uint8_t Length;
    uint8_t Reply;
} __packed;

#define REPLY_PKT_LEN   1

#endif

#if 1 // ======================= Channels & cycles =============================
#define RCHNL_SRV       0
#define ID2RCHNL(ID)    (RCHNL_MIN + ID)
#endif

#if 1 // =========================== Timings ===================================
#define RX_T_MS                 11
#define RX_SLEEP_T_MS           810
#define MIN_SLEEP_DURATION_MS   18
#define RETRY_CNT               2

#endif

#define RMSG_Q_LEN      18
#define RMSGID_PKT      1
#define RMSGID_CHNL     2

class rLevel1_t {
private:
    void TryToSleep(uint32_t SleepDuration) {
//        if(SleepDuration >= MIN_SLEEP_DURATION_MS) CC.EnterPwrDown();
        chThdSleepMilliseconds(SleepDuration); // XXX
    }

public:
    int8_t Rssi;
    rPktAcg_t PktTx;
    rPktAcg_t PktRxAcg;
//    rPktReply_t PktReply;
    uint8_t Init();
    void SetChannel(uint8_t NewChannel);
    // Inner use
    void ITask();
};

extern rLevel1_t Radio;
extern EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQRadio;
