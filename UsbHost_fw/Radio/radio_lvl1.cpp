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
    while(true) {
        uint8_t RxRslt = CC.ReceiveSync(360, &Pkt, &Rssi);
        if(RxRslt == OK) {
//            Uart.Printf("Rssi=%d\r", Rssi);
            if(UsbCDC.IsActive()) {
                UsbCDC.Printf("%u;   %d; %d; %d;   %d; %d; %d;   %d; %d; %d\r", Pkt.Time,
                        Pkt.gyro[0], Pkt.gyro[1], Pkt.gyro[2],
                        Pkt.acc[0],  Pkt.acc[1],  Pkt.acc[2],
                        Pkt.mag[0],  Pkt.mag[1],  Pkt.mag[2]);
            }
        }


#if 0        // Demo
        if(App.Mode == 0b0001) { // RX
            int8_t Rssi;
            Color_t Clr;
            uint8_t RxRslt = CC.ReceiveSync(RX_T_MS, &Pkt, &Rssi);
            if(RxRslt == OK) {
                Uart.Printf("\rRssi=%d", Rssi);
                Clr = clWhite;
                if     (Rssi < -100) Clr = clRed;
                else if(Rssi < -90) Clr = clYellow;
                else if(Rssi < -80) Clr = clGreen;
                else if(Rssi < -70) Clr = clCyan;
                else if(Rssi < -60) Clr = clBlue;
                else if(Rssi < -50) Clr = clMagenta;
            }
            else Clr = clBlack;
            Led.SetColor(Clr);
            chThdSleepMilliseconds(99);
        }
        else {  // TX
            DBG1_SET();
            CC.TransmitSync(&Pkt);
            DBG1_CLR();
//            chThdSleepMilliseconds(99);
        }
//#else
#endif

#if 0
        // ==== Transmitter ====
        if(App.MustTransmit) {
            if(App.ID != OldID) {
                OldID = App.ID;
                CC.SetChannel(ID2RCHNL(App.ID));
                Pkt.DWord = App.ID;
            }
            DBG1_SET();
            CC.TransmitSync(&Pkt);
            DBG1_CLR();
        }

        // ==== Receiver ====
        else {
            DBG2_SET();
            // Listen if nobody found, and do not if found
            int8_t Rssi;
            // Iterate channels
            for(int32_t i = ID_MIN; i <= ID_MAX; i++) {
                if(i == App.ID) continue;   // Do not listen self
                CC.SetChannel(ID2RCHNL(i));
                uint8_t RxRslt = CC.ReceiveSync(RX_T_MS, &Pkt, &Rssi);
                if(RxRslt == OK) {
//                    Uart.Printf("\rCh=%d; Rssi=%d", i, Rssi);
                    App.SignalEvt(EVTMSK_SOMEONE_NEAR);
                    break; // No need to listen anymore if someone already found
                }
            } // for
            CC.SetChannel(ID2RCHNL(App.ID));    // Set self channel back
            DBG2_CLR();
            TryToSleep(RX_SLEEP_T_MS);
        }
#endif
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

uint8_t rLevel1_t::TxRxSync(rPkt_t *PPkt) {
//    Uart.Printf("Pkt: %u; %u %u %u %u %u; Pwr=%u, Data=%u\r", PPkt->ID, PPkt->Brightness[0], PPkt->Brightness[1], PPkt->Brightness[2], PPkt->Brightness[3], PPkt->Brightness[4], PPkt->IRPwr, PPkt->IRData);
    PTxPkt = PPkt;  // copy pointer
    chEvtSignal(PThd, EVT_RADIO_NEW_CMD);
    eventmask_t EvtMsk = chEvtWaitAny(EVT_RADIO_OK | EVT_RADIO_TIMEOUT);
    if(EvtMsk & EVT_RADIO_OK) return OK;
    else return TIMEOUT;
}
#endif
