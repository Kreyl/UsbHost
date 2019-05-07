/*
 * radio_lvl1.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "cc1101.h"
#include "MsgQ.h"
#include "led.h"
#include "Sequences.h"
#include "usb_cdc.h"
#include "main.h"

cc1101_t CC(CC_Setup0, CCIrqHandler);
extern UsbCDC_t UsbCDC;

//#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOB
#define DBG_PIN1    9
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

static rPkt_t RxPkt;
int8_t Rssi;

static THD_WORKING_AREA(warLvl1Thread, 1024);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    Radio.ITask();
}

__noreturn
void rLevel1_t::ITask() {
    while(true) {
        // Transmit cmd to lockets
        if(MustTx and TxPkt.To == 0) {
            systime_t start = chVTGetSystemTimeX();
            do {
                CC.Recalibrate();
                CC.Transmit(&TxPkt, RPKT_LEN);
            }
            while(chVTTimeElapsedSinceX(start) < 2007 and MustTx);
        }

        // Receive
        CC.Recalibrate();
        uint8_t RxRslt = CC.Receive(360, &RxPkt, RPKT_LEN, &Rssi);
        if(RxRslt == retvOk) {
            if(ShowRx) {
                Printf("%u: Thr: %d; Pwr: %u; Rssi: %d\r", RxPkt.From, RxPkt.RssiThr, RxPkt.Value, Rssi);
                if(UsbCDC.IsActive()) UsbCDC.Print("%u: Thr: %d; Pwr: %u; Rssi: %d\r", RxPkt.From, RxPkt.RssiThr, RxPkt.Value, Rssi);
            }
            if(MustTx and TxPkt.To == RxPkt.From) {
                // Check if stop TX
                if(TxPkt.Value == RxPkt.Value and TxPkt.RssiThr == RxPkt.RssiThr) MustTx = false;
                else {
                    CC.Transmit(&TxPkt, RPKT_LEN);
                    if(UsbCDC.IsActive()) UsbCDC.Print("TX\r");
                }
            }
        }
    } // while true
}

uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
//    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif    // Init radioIC

    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_TX_PWR);
        CC.SetPktSize(RPKT_LEN); // Max sz
        CC.SetChannel(1);
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), NORMALPRIO, (tfunc_t)rLvl1Thread, NULL);
        return retvOk;
    }
    else return retvFail;
}


