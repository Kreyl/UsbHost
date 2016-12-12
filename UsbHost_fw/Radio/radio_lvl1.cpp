/*
 * radio_lvl1.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "evt_mask.h"
#include "main.h"
#include "cc1101.h"
#include "uart.h"

//#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOB
#define DBG_PIN1    0
#define DBG1_SET()  PinSetHi(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinSetLo(DBG_GPIO1, DBG_PIN1)
//#define DBG_GPIO2   GPIOB
//#define DBG_PIN2    9
//#define DBG2_SET()  PinSet(DBG_GPIO2, DBG_PIN2)
//#define DBG2_CLR()  PinClear(DBG_GPIO2, DBG_PIN2)
#else
#define DBG1_SET()
#define DBG1_CLR()
#endif

rLevel1_t Radio;
static rPkt_t PktTxInfo;

#if 1 // ================================ Task =================================
static THD_WORKING_AREA(warLvl1Thread, 256);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    Radio.ITask();
}

__noreturn
void rLevel1_t::ITask() {
    PktTxInfo.Cmd = cmdGetInfo; // Init pkt for info requests
    uint32_t n = 0;
    while(true) {
        // Check if cmd injection is waiting
        if(DevMgr.QLen != 0) {

        }
        else {
            // Ask everyone for info
            PktTxInfo.ID = n;
            CC.TransmitSync(&PktTxInfo);
            // Wait for answer
            uint8_t RxRslt = CC.ReceiveSync(9, &PktRx, &Rssi);
            if(RxRslt == OK) {
                Uart.Printf("ID=%u; Rssi=%d\r", Rssi);
                DevInfo_t *PInfo = &DevMgr.Info[n];
                PInfo->Data = PktRx.DevInfoData;    // Save what received
                PInfo->Timestamp = chVTGetSystemTimeX();
            } // if RxRslt ok
            n++;
            if(n >= DEVICE_CNT) n = 0;
        }
    } // while true
}
#endif // task

#if 1 // ============================
uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
//    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif    // Init radioIC
    if(CC.Init() == OK) {
        CC.SetTxPower(CC_PwrPlus5dBm);
        CC.SetPktSize(RPKT_LEN);
        CC.SetChannel(2);
//        CC.EnterPwrDown();
        // Thread
        PThd = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
        return OK;
    }
    else return FAILURE;
}
#endif
