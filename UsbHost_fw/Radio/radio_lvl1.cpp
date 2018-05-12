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

#define DBG_PINS

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
//uint8_t OnRadioRx();
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQRadio;

#if 1 // ================================ Task =================================
static THD_WORKING_AREA(warLvl1Thread, 2048);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    Radio.ITask();
}

//systime_t St;

__noreturn
void rLevel1_t::ITask() {
    PktTx.Length = RPKTACG_LEN - 1;
    while(true) {
//        EvtMsg_t Msg = EvtQRadio.Fetch(TIME_INFINITE);
//        switch(Msg.ID) {
//            case evtIdNewAcgRslt:
//                CC.Recalibrate();
//                DBG1_SET();
//                CC.Transmit(&PktTx, RPKTACG_LEN);
//                DBG1_CLR();
//                break;

//            default: break;
//        }
        uint8_t Len = 0;
        CC.Recalibrate();
        uint8_t RxRslt = CC.ReceiveLong(180, &PktRxAcg, &Len, &Rssi);
        if(RxRslt == retvOk) {
//            Printf("Rssi=%d\r", Rssi);
//            systime_t Ela = chVTTimeElapsedSinceX(St);
//            St = chVTGetSystemTimeX();
//            Printf("Ela: %u\r", Ela);
            if(UsbCDC.IsActive()) {
                UsbCDC.Printf("%d %d %d; %d %d %d;  %d %d %d; %d %d %d;  %d %d %d; %d %d %d;  %d %d %d; %d %d %d;  %d %d %d; %d %d %d;  %d %d %d; %d %d %d\r\n",
                PktRxAcg.Acg[0].a[0], PktRxAcg.Acg[0].a[1], PktRxAcg.Acg[0].a[2], PktRxAcg.Acg[0].g[0], PktRxAcg.Acg[0].g[1], PktRxAcg.Acg[0].g[2],
                PktRxAcg.Acg[1].a[0], PktRxAcg.Acg[1].a[1], PktRxAcg.Acg[1].a[2], PktRxAcg.Acg[1].g[0], PktRxAcg.Acg[1].g[1], PktRxAcg.Acg[1].g[2],
                PktRxAcg.Acg[2].a[0], PktRxAcg.Acg[2].a[1], PktRxAcg.Acg[2].a[2], PktRxAcg.Acg[2].g[0], PktRxAcg.Acg[2].g[1], PktRxAcg.Acg[2].g[2],
                PktRxAcg.Acg[3].a[0], PktRxAcg.Acg[3].a[1], PktRxAcg.Acg[3].a[2], PktRxAcg.Acg[3].g[0], PktRxAcg.Acg[3].g[1], PktRxAcg.Acg[3].g[2],
                PktRxAcg.Acg[4].a[0], PktRxAcg.Acg[4].a[1], PktRxAcg.Acg[4].a[2], PktRxAcg.Acg[4].g[0], PktRxAcg.Acg[4].g[1], PktRxAcg.Acg[4].g[2],
                PktRxAcg.Acg[5].a[0], PktRxAcg.Acg[5].a[1], PktRxAcg.Acg[5].a[2], PktRxAcg.Acg[5].g[0], PktRxAcg.Acg[5].g[1], PktRxAcg.Acg[5].g[2]);
            }


//            PktRxAcg.Print();
//            // Transmit reply
//            CC.Transmit(&PktReply, PktReply.Length+1);
//            EvtMsg_t Msg(evtIdRadioRx, &PktRx);
//            EvtQMain.SendNowOrExit(Msg);
        } // if RxRslt ok
    } // while
}
#endif // task

#if 1 // ============================
uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
//    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif    // Init radioIC

    EvtQRadio.Init();

    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_TX_PWR);
        CC.SetPktSize(RPKTACG_LEN); // Max sz
//        CC.SetChannel(Settings.RChnl);
        // Thread
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), NORMALPRIO, (tfunc_t)rLvl1Thread, NULL);
        return retvOk;
    }
    else return retvFail;
}

void rLevel1_t::SetChannel(uint8_t NewChannel) {
    chSysLock();
    CC.SetChannel(NewChannel);
    chSysUnlock();
}
#endif
