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

#if 1 // ================================ Task =================================
static THD_WORKING_AREA(warLvl1Thread, 256);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    Radio.ITask();
}

static systime_t LastRxT[DEVICE_CNT];

__noreturn
void rLevel1_t::ITask() {
    rPkt_t PktTxGetInfo(1, 0, 0);  // 0 as paramID means data request
    uint8_t RxRslt;
    while(true) {
        // Process request queue
        MsgToDevice_t MsgD;
        while(QToDevices.Get(&MsgD) == OK) {
            // Prepare RPkt
            rPkt_t PktTx(MsgD.DevID, MsgD.ParamID, MsgD.ParamV);
            MsgToHost_t MsgH(MsgD.DevID, MsgD.MsgID, FAILURE);
            for(int i=0; i<RETRY_CNT; i++) {
                CC.Transmit(&PktTx);
                RxRslt = CC.Receive(RX_T_MS, &PktRx, &Rssi);
                if(RxRslt == OK) {
                    //Uart.Printf("Rssi %d\r", Rssi);
                    MsgH.SetInfo(PktRx.DevInfo);
                    MsgH.MsgType = OK;
                    LastRxT[MsgD.DevID - 1] = chVTGetSystemTimeX();
                    break; // Stop trying
                } // if rslt
            } // for retry
            QToHost.Put(MsgH);
        } // while Q is not empty

        // Ask everyone for info, one by one
        CC.Transmit(&PktTxGetInfo);
        // Wait for answer
        RxRslt = CC.Receive(RX_T_MS, &PktRx, &Rssi);
        if(RxRslt == OK) {
//            Uart.Printf("GetInfo ID=%u; Rssi=%d\r", PktRx.ID, Rssi);
            LastRxT[PktTxGetInfo.ID - 1] = chVTGetSystemTimeX();  // Reset RxTime info
            DevInfoList.PutData(PktRx);
        }
        else { // No answer
            if(ST2MS(chVTTimeElapsedSinceX(LastRxT[PktTxGetInfo.ID - 1])) > NO_REPLY_TIMEOUT_ms) {
                LastRxT[PktTxGetInfo.ID - 1] = chVTGetSystemTimeX();  // Reset RxTime info
                DevInfoList.PutTimeout(PktTxGetInfo.ID);
            }
        }

        // Switch to next ID
        PktTxGetInfo.ID++;
        if(PktTxGetInfo.ID > DEV_ID_MAX) PktTxGetInfo.ID = DEV_ID_MIN;
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
        CC.SetTxPower(CC_PwrPlus10dBm);
        CC.SetPktSize(RPKT_LEN);
        CC.SetChannel(2);
        // Thread
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
        return OK;
    }
    else return FAILURE;
}
#endif
