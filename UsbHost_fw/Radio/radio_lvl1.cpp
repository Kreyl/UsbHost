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
#include "board.h"

#include "usb_cdc.h"

//#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOB
#define DBG_PIN1    12
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#define DBG_GPIO2   GPIOB
#define DBG_PIN2    13
#define DBG2_SET()  PinSet(DBG_GPIO2, DBG_PIN2)
#define DBG2_CLR()  PinClear(DBG_GPIO2, DBG_PIN2)
#endif

rLevel1_t Radio;

#if 1 // ================================ Task =================================
static THD_WORKING_AREA(warLvl1Thread, 256);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    Radio.ITask();
}

__noreturn
void rLevel1_t::ITask() {
    rPkt_t Pkt;
    int8_t Rssi;
    CC.SetChannel(1);
    while(true) {
        uint8_t RxRslt = CC.ReceiveSync(99, &Pkt, &Rssi);
        if(RxRslt == OK) {
//              Uart.Printf("Rssi=%d\r", Rssi);
//        chThdSleepMilliseconds(5);
            Uart.Printf("%u;%d;%d;%d;%d;%d;%d;%d;%d;%d\r",
                    Pkt.Time,
                    Pkt.gyro[0], Pkt.gyro[1], Pkt.gyro[2],
                    Pkt.acc[0],  Pkt.acc[1],  Pkt.acc[2],
                    Pkt.mag[0],  Pkt.mag[1],  Pkt.mag[2]);
        } // RxResult ok
    } // while true
}
#endif // task

#if 1 // ============================
uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif
    // Init radioIC
    if(CC.Init() == OK) {
        CC.SetTxPower(CC_PwrPlus7dBm);
        CC.SetPktSize(RPKT_LEN);
        CC.SetChannel(0);
        // Thread
        PThd = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
        return OK;
    }
    else return FAILURE;
}
#endif
