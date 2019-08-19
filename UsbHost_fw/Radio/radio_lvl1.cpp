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
        StartOfCycle:
        // Transmit if PktTx is ready
        if(IPktTx.From != 0) {
            // Try to communicate for some time
            systime_t start = chVTGetSystemTimeX();
            while(chVTTimeElapsedSinceX(start) < MS2ST(306)) {
                CC.Recalibrate();
                CC.Transmit(&IPktTx, RPKT_LEN);
                if(CC.Receive(5, &IPktRx, RPKT_LEN, &Rssi) == retvOk) {
                    if(IPktRx.From == IPktTx.To and IPktRx.To == SELF_ID) { // reply from target, stop trying
                        ProcessReplyAndStop();
                        goto StartOfCycle;
                    }
                }
            } // while
            // Pkt was transmitted, but no answer received. If Mesh used, try to wait more
            if(IPktTx.PktID != PKTID_DO_NOT_RETRANSMIT) { // Mesh request, wait for a while
                for(int i=0; i<20; i++) {
                    CC.Recalibrate();
                    if(CC.Receive(306, &IPktRx, RPKT_LEN, &Rssi) == retvOk) {
                        if(IPktRx.From == IPktTx.To and IPktRx.To == SELF_ID) { // reply from target, stop trying
                            ProcessReplyAndStop();
                            goto StartOfCycle;
                        }
                    }
                } // for
            } // if mesh
            SayTimeoutAndStop(); // Silence answers our cries
        } // if PktTx is ready

        if(ShowRx) DoShowRx();
        else chThdSleepMilliseconds(153);
    } // while true
}

void rLevel1_t::ProcessReplyAndStop() {
    switch(IPktRx.Cmd) {
        case rcmdPong:
            Printf("Pong %u %u\r\n", IPktRx.Pong.Reply, IPktRx.Pong.MaxLvlID);
            if(UsbCDC.IsActive()) UsbCDC.Print("Pong %u %u\r\n", IPktRx.Pong.Reply, IPktRx.Pong.MaxLvlID);
            break;

        case rcmdLocketGetParam:
            Printf("LckPar %u %u\r\n", IPktRx.LocketParam.ParamID, IPktRx.LocketParam.Value);
            if(UsbCDC.IsActive()) UsbCDC.Print("LckPar %u %u\r\n", IPktRx.LocketParam.ParamID, IPktRx.LocketParam.Value);
            break;

        default:
            Printf("Reply %u\r\n", IPktRx.Cmd);
            if(UsbCDC.IsActive()) UsbCDC.Print("Reply %u\r\n", IPktRx.Cmd);
            break;
    } // switch
    IPktTx.From = 0; // Use From=0 as a flag to stop communication
}

void rLevel1_t::SayTimeoutAndStop() {
    Printf("Timeout\r\n");
    if(UsbCDC.IsActive()) UsbCDC.Print("Timeout\r\n");
    IPktTx.From = 0; // Use From=0 as a flag to stop communication
}

void rLevel1_t::PrepareAndTransmitRpkt(RCmd_t ACmd, Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
    PktTx.From = 1;
    PktTx.TransmitterID = 1;
    PktTx.Cmd = ACmd;
    if((PCmd->GetNextString() == retvOk) and (PCmd->Token[0] == 'M' or PCmd->Token[0] == 'm')) {
        PktTx.PktID = PktID;
        PktID += 2;
        if(PktID > PKTID_TOP_VALUE) PktID = 1;
    }
    else PktTx.PktID = PKTID_DO_NOT_RETRANSMIT;
    // Transmit
    chSysLock();
    IPktTx = PktTx;
    chSysUnlock();
}

void rLevel1_t::DoShowRx() {
    CC.Recalibrate();
    uint8_t RxRslt = CC.Receive(360, &IPktRx, RPKT_LEN, &Rssi);
    if(RxRslt == retvOk) {
        Printf("From: %u; To: %u; TrrID: %u; PktID: %u;", IPktRx.From, IPktRx.To, IPktRx.TransmitterID, IPktRx.PktID);
        if(UsbCDC.IsActive()) UsbCDC.Print("From: %u; To: %u; TrrID: %u; PktID: %u;", IPktRx.From, IPktRx.To, IPktRx.TransmitterID, IPktRx.PktID);

        // Show payload
        switch(IPktRx.Cmd) {
            case rcmdPing:
                Printf("Cmd: Ping\r\n");
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: Ping\r\n");
                break;
            case rcmdPong:
                Printf("Cmd: Pong; Reply: %u; MaxLvlID: %u\r\n", IPktRx.Pong.Reply, IPktRx.Pong.MaxLvlID);
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: Pong; Reply: %u; MaxLvlID: %u\r\n", IPktRx.Pong.Reply, IPktRx.Pong.MaxLvlID);
                break;
            case rcmdBeacon:
                Printf("Cmd: Beacon; RssiThr: %d; Damage: %u\r\n", IPktRx.Beacon.RssiThr, IPktRx.Beacon.Damage);
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: Beacon; RssiThr: %d; Damage: %u\r\n", IPktRx.Beacon.RssiThr, IPktRx.Beacon.Damage);
                break;
            case rcmdScream:
                Printf("Cmd: Scream\r\n");
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: Scream\r\n");
                break;
            case rcmdLustraParams:
                Printf("Cmd: LusPars; Power: %u; RssiThr: %d; Damage: %u\r\n", IPktRx.LustraParams.Power, IPktRx.LustraParams.RssiThr, IPktRx.LustraParams.Damage);
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: LusPars; Power: %u; RssiThr: %d; Damage: %u\r\n", IPktRx.LustraParams.Power, IPktRx.LustraParams.RssiThr, IPktRx.LustraParams.Damage);
                break;
            case rcmdLocketSetParam:
                Printf("Cmd: LckSetPar; ParamID: %u; Value: %u\r\n", IPktRx.LocketParam.ParamID, IPktRx.LocketParam.Value);
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: LckSetPar; ParamID: %u; Value: %u\r\n", IPktRx.LocketParam.ParamID, IPktRx.LocketParam.Value);
                break;
            case rcmdLocketGetParam:
                Printf("Cmd: LckGetPar; ParamID: %u; Value: %u\r\n", IPktRx.LocketParam.ParamID, IPktRx.LocketParam.Value);
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: LckGetPar; ParamID: %u; Value: %u\r\n", IPktRx.LocketParam.ParamID, IPktRx.LocketParam.Value);
                break;
            case rcmdLocketExplode:
                Printf("Cmd: Explode\r\n");
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: Explode\r\n");
                break;
            case rcmdLocketDieAll:
                Printf("Cmd: DieAll\r\n");
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: DieAll\r\n");
                break;
            case rcmdLocketDieChoosen:
                Printf("Cmd: Die\r\n");
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: Die\r\n");
                break;
            default:
                Printf("Cmd: %u\r\n", IPktRx.Cmd);
                if(UsbCDC.IsActive()) UsbCDC.Print("Cmd: %u\r\n", IPktRx.Cmd);
                break;
        } // switch
    } // if RX ok
}

uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
//    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif    // Init radioIC

    IPktTx.From = 0; // Invalidate pkt

    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_TX_PWR);
//        CC.SetTxPower(CC_PwrMinus30dBm);
        CC.SetPktSize(RPKT_LEN); // Max sz
        CC.SetChannel(1);
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), NORMALPRIO, (tfunc_t)rLvl1Thread, NULL);
        return retvOk;
    }
    else return retvFail;
}
