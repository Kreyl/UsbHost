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

rPkt_t Pkt;
int8_t Rssi;

static THD_WORKING_AREA(warLvl1Thread, 1024);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    while(true) {
        for(uint32_t i=0; i<2; i++) {
            // ==== TX ====
            CC.SetChannel(i);
            StickSetup[i].Clr.ToRGB(&Pkt.R, &Pkt.G, &Pkt.B);
            Pkt.W = StickSetup[i].Clr.W;
            Pkt.VibroPwr = StickSetup[i].Vibro;

            CC.Recalibrate();
            CC.Transmit(&Pkt, RPKT_LEN);

            // ==== RX ====
            while(true) {   // Receive data until it remains
                CC.Recalibrate();
                uint8_t RxRslt = CC.Receive(7, &Pkt, RPKT_LEN, &Rssi);
                if(RxRslt == retvOk) {
//                    Printf("%d: %d\r", i, Rssi);
                    Printf("%d;%d;%d;%d;%d;%d;%d;%d;%d\r\n",
                                                    i, Pkt.Time, Pkt.Btn,
                                                    Pkt.gyro[0], Pkt.gyro[1], Pkt.gyro[2],
                                                    Pkt.acc[0], Pkt.acc[1], Pkt.acc[2]);
//                    if(UsbCDC.IsActive()) {
//                        UsbCDC.Print("%d;%d;%d;%d;%d;%d;%d;%d;%d\r\n",
//                                i, Pkt.Time, Pkt.Btn,
//                                Pkt.gyro[0], Pkt.gyro[1], Pkt.gyro[2],
//                                Pkt.acc[0], Pkt.acc[1], Pkt.acc[2]);
//                    }
                } // RxResult ok
                else break;
            } // while true
        } // for
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
        CC.SetChannel(0);
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), NORMALPRIO, (tfunc_t)rLvl1Thread, NULL);
        return retvOk;
    }
    else return retvFail;
}
