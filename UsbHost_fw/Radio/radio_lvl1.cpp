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
        for(uint8_t i=0; i<12; i++) Pkt.b[i] = i;
        CC.Recalibrate();
        CC.Transmit(&Pkt, 12);
        chThdSleepMilliseconds(18);

//        CC.Recalibrate();
//        uint8_t RxRslt = CC.Receive(360, &Pkt, RPKT_LEN, &Rssi);
//        if(RxRslt == retvOk) {
//            Printf("\rRssi=%d", Rssi);
//            //Printf("%d;%d;%d;%d;%d;%d;%d;%d\r\n", PktRx.Time, PktRx.Btn, PktRx.gyro[0], PktRx.gyro[1], PktRx.gyro[2], PktRx.acc[0], PktRx.acc[1], PktRx.acc[2]);
////            if(UsbCDC.IsActive()) {
////                UsbCDC.Print("%d;%d;%d;%d;%d;%d;%d;%d\r\n", PktRx.Time, PktRx.Btn, PktRx.gyro[0], PktRx.gyro[1], PktRx.gyro[2], PktRx.acc[0], PktRx.acc[1], PktRx.acc[2]);
//        }
    }
}

uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
//    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif    // Init radioIC

    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_PwrPlus10dBm);
        CC.SetPktSize(RPKT_LEN); // Max sz
        CC.SetChannel(0);
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), NORMALPRIO, (tfunc_t)rLvl1Thread, NULL);
        return retvOk;
    }
    else return retvFail;
}
