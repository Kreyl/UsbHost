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
//uint8_t OnRadioRx();
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQRadio;

extern LedRGB_t Led;

#if 1 // ================================ Task =================================
static THD_WORKING_AREA(warLvl1Thread, 2048);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    Radio.ITask();
}

__noreturn
void rLevel1_t::ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQRadio.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case RMSG_SETCHNL:
                CC.SetChannel(Msg.Value);
                break;

            case RMSG_SEND_PARAM: {
                // Build pkt
                PktTx.ID = Msg.b[0];
                PktTx.Cmd = Msg.b[1];
                PktTx.Param = Msg.b[2];
                PktTx.Print();
                // Transmit
                DBG1_SET();
                CC.Recalibrate();
                CC.Transmit(&PktTx, RPKT_SZ);
                DBG1_CLR();
                // Receive reply
                uint8_t RxRslt = CC.Receive(180, &PktRx, RPKT_SZ, &Rssi);
                if(RxRslt == retvOk) {
//                    Printf("Rssi=%d\r", Rssi);
//                    Printf("%u %u; %A\r\n", PktRx.ID, PktRx.Cmd, PktRx.b, 14, ' ');
                    if(UsbCDC.IsActive()) {
                        UsbCDC.Printf("%u %u; %A\r\n", PktRx.ID, PktRx.Cmd, PktRx.b, 14, ' ');
                    }
                    Led.StartOrRestart(lsqRx);
                } // if RxRslt ok
            } break;

            default: break;
        } // switch
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
        CC.SetPktSize(RPKT_LEN); // Max sz
        CC.SetChannel(4);
        // Thread
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), NORMALPRIO, (tfunc_t)rLvl1Thread, NULL);
        return retvOk;
    }
    else return retvFail;
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
