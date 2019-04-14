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
#include "color.h"

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

#if 1 // =========================== Pkt_t =====================================
struct rPkt_t {
    union {
        uint32_t DWord[2];
        int32_t TimeAfterPress;
        struct {
            uint8_t R1, G1, B1, R2, G2, B2;
            uint16_t Wait_ms;
        } __attribute__ ((__packed__));
    } __attribute__ ((__packed__));
    uint8_t Cmd;
    uint8_t ID;
//    bool operator == (const rPkt_t &APkt) { return (DWord32 == APkt.DWord32); }
    rPkt_t& operator = (const rPkt_t &Right) {
        chSysLock();
        DWord[0] = Right.DWord[0];
        DWord[1] = Right.DWord[1];
        Cmd = Right.Cmd;
        ID = Right.ID;
        chSysUnlock();
        return *this;
    }
    rPkt_t() : TimeAfterPress(0), Cmd(0) {}
} __attribute__ ((__packed__));
#endif

#if 1 // ============================== Defines ================================
#define RCHNL                   0
#define BTNS_CNT_MAX            16
#define RPKT_LEN                sizeof(rPkt_t)
#define CC_TX_PWR               CC_Pwr0dBm

#define CMD_GET                 18
#define CMD_SET                 27
#define CMD_FLASH               36
#define CMD_OK                  0

#define RX_T_MS                 4
#define RX_SLEEP_T_MS           810
#define RETRY_CNT               4

#endif


class rLevel1_t {
private:
    void TryToSleep(uint32_t SleepDuration) {
//        if(SleepDuration >= MIN_SLEEP_DURATION_MS) CC.EnterPwrDown();
        chThdSleepMilliseconds(SleepDuration); // XXX
    }
    uint8_t BtnGet(uint8_t ID, int32_t *PTimeAfterPress);
public:
    int32_t Timer = 0;
    bool MustRescan = false;
    uint32_t BtnsEnabled = 1;
    int32_t TimeAfterPressTable[BTNS_CNT_MAX];
    rPkt_t CmdTable[BTNS_CNT_MAX];
    uint8_t Init();
    // Inner use
    void ITask();
};

extern rLevel1_t Radio;
