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

cc1101_t CC(CC_Setup0);

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

uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
//    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif    // Init radioIC

    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_TX_PWR);
        CC.SetPktSize(RPKT_LEN); // Max sz
        CC.SetChannel(4);
        return retvOk;
    }
    else return retvFail;
}

uint8_t rLevel1_t::TxRxSync() {
//    Uart.Printf("Pkt: %u; %u %u %u %u %u; Pwr=%u, Data=%u\r", PPkt->ID, PPkt->Brightness[0], PPkt->Brightness[1], PPkt->Brightness[2], PPkt->Brightness[3], PPkt->Brightness[4], PPkt->IRPwr, PPkt->IRData);
    // Make several retries
    int Retries = 4;
    while(true) {
        Printf("Try %d\r", Retries);
        // Transmit pkt
        DBG1_SET();
        CC.Recalibrate();
        CC.Transmit(&PktTx, RPKT_LEN);
        DBG1_CLR();

        // Wait answer
        uint8_t RxRslt = CC.Receive(54, &PktRx, RPKT_LEN, &Rssi);
        if(RxRslt == retvOk) {
            Printf("\rRssi=%d", Rssi);
            // Check if good answer received, repeat if not
            if(PktRx.ID == PktTx.ID and PktRx.Status == retvOk) return retvOk;
        }

        // Timeout or bad answer
        if(--Retries <= 0) return retvFail;
        // Wait random time
        int Delay = Random::Generate(4, 27);
        chThdSleepMilliseconds(MS2ST(Delay));
    } // while(true)
}
