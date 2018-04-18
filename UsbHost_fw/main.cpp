/*
 * main.cpp
 *
 *  Created on: 26 дек. 2015 г.
 *      Author: Kreyl
 */
#include "hal.h"
#include "MsgQ.h"
#include "shell.h"
#include "uart.h"
#include "board.h"
#include "led.h"
#include "usb_cdc.h"
#include "color.h"
#include "cc1101.h"
#include "led.h"
#include "Sequences.h"
#include "EvtMsgIDs.h"

EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
extern CmdUart_t Uart;
void OnCmd(Shell_t *PShell);
void ITask();

cc1101_t CC(CC_Setup0);

LedRGB_t Led { {LED_GPIO, LEDR_PIN, LED_TMR, LEDR_CHNL}, {LED_GPIO, LEDG_PIN, LED_TMR, LEDG_CHNL}, {LED_GPIO, LEDB_PIN, LED_TMR, LEDB_CHNL} };
bool CCok = false;

#define PKT_ARR_LEN     12
struct rPkt_t {
    uint16_t ID;
    union {
        uint8_t b[PKT_ARR_LEN];
        struct {
            uint8_t Cmd;
            uint8_t Value[4];
        } __packed;
    };
} __packed;

#define PKT_SZ  sizeof(rPkt_t)

int main(void) {
    // ==== Setup clock frequency ====
    Clk.EnablePrefetch();
    Clk.SetupBusDividers(ahbDiv2, apbDiv1);
    Clk.UpdateFreqValues();

    // Init OS
    halInit();
    chSysInit();

    // ==== Init hardware ====
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP | SYSCFG_CFGR1_USART1RX_DMA_RMP;
    Uart.Init(115200);
    Printf("\r%S %S\r", APP_NAME, BUILD_TIME);
    Clk.PrintFreqs();

    Led.Init();
    Led.SetColor(clYellow);

#if USB_ENABLED
    chThdSleepMilliseconds(207);
    // Enable HSI48
    chSysLock();
    while(Clk.SwitchTo(csHSI48) != retvOk) {
        PrintfI("Hsi48 Fail\r");
        Led.SetColor(clRed);
        chThdSleepS(MS2ST(207));
        Led.SetColor(clBlack);
        chThdSleepS(MS2ST(207));
    }
    Clk.UpdateFreqValues();
    Clk.SelectUSBClock_HSI48();
    chSysUnlock();
    Clk.PrintFreqs();

    UsbCDC.Init();
    UsbCDC.Connect();

    chSysLock();
    Clk.EnableCRS();
    chSysUnlock();
#endif

    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_Pwr0dBm);
        CC.SetPktSize(PKT_SZ);
        CC.SetChannel(0);
        CC.Recalibrate();
        Led.StartOrRestart(lsqStart);
        CCok = true;
    }
    else Led.StartOrRestart(lsqFailure);

    // Main cycle
    ITask();
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdShellCmd:
                OnCmd((Shell_t*)Msg.Ptr);
                ((Shell_t*)Msg.Ptr)->SignalCmdProcessed();
                break;

            default: break;
        } // switch

//        if(EvtMsk & EVT_USB_READY) {
//            Uart.Printf("UsbReady\r");
//            if(CCok) Led.StartSequence(lsqUSB);
//        }
//
//        if(EvtMsk & EVT_USB_NEW_CMD) {
//            Led.StartSequence(lsqUSBCmd);
//            OnCmd((Shell_t*)&UsbCDC);
//            UsbCDC.SignalCmdProcessed();
//        }
//        if(EvtMsk & EVT_UART_NEW_CMD) {
//            OnCmd((Shell_t*)&Uart);
//            Uart.SignalCmdProcessed();
//        }
    } // while true
}

#if 1 // ======================= Command processing ============================
void OnCmd(Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
//    __unused int32_t dw32 = 0;  // May be unused in some configurations
//    Uart.Printf("\r%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) PShell->Ack(retvOk);
    else if(PCmd->NameIs("Version")) PShell->Printf("%S %S\r", APP_NAME, BUILD_TIME);

    else if(PCmd->NameIs("SetID")) {
        rPkt_t Pkt;
        if(PCmd->GetArray(Pkt.b, PKT_ARR_LEN) != retvOk) goto SetIdEnd;
        if(PCmd->GetNext<uint16_t>(&Pkt.ID) != retvOk) goto SetIdEnd;
        // Transmit data and wait answer
        CC.Transmit(&Pkt, PKT_SZ);
        // XXX
        PShell->Ack(retvOk);
        return;
        SetIdEnd:
        PShell->Ack(retvCmdError);
    }

//    else if(PCmd->NameIs("GetState")) {
//        systime_t start = chVTGetSystemTimeX();
//        rPkt_t Pkt;
//        while(chVTTimeElapsedSinceX(start) < MS2ST(4500)) {
//            CC.Transmit(&Pkt);
//        }
//        PShell->Ack(OK);
//    }


    else PShell->Ack(retvCmdUnknown);
}
#endif
