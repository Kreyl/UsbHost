/*
 * main.cpp
 *
 *  Created on: 26 дек. 2015 г.
 *      Author: Kreyl
 */

#include "main.h"
#include "usb_cdc.h"
#include "color.h"
#include "led.h"
#include "Sequences.h"

App_t App;

LedRGB_t Led { {LED_GPIO, LEDR_PIN, LED_TMR, LEDR_CHNL}, {LED_GPIO, LEDG_PIN, LED_TMR, LEDG_CHNL}, {LED_GPIO, LEDB_PIN, LED_TMR, LEDB_CHNL} };

int main(void) {
    // ==== Setup clock frequency ====
    Clk.EnablePrefetch();
    Clk.SetupBusDividers(ahbDiv2, apbDiv1);
    Clk.UpdateFreqValues();

    // Init OS
    halInit();
    chSysInit();
    App.InitThread();

    // ==== Init hardware ====
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP | SYSCFG_CFGR1_USART1RX_DMA_RMP;
    Uart.Init(115200, UART_GPIO, UART_TX_PIN, UART_GPIO, UART_RX_PIN);
    Uart.Printf("\r%S %S\r", APP_NAME, BUILD_TIME);
    Clk.PrintFreqs();

    Led.Init();

    if(Radio.Init() == OK) Led.StartSequence(lsqStart);
    else Led.StartSequence(lsqFailure);
    chThdSleepMilliseconds(810);

#if USB_ENABLED
    UsbCDC.Init();
    chThdSleepMilliseconds(45);
    // Enable HSI48
    uint8_t ClkState = FAILURE;
    while(true) {
        chSysLock();
        ClkState = Clk.SwitchTo(csHSI48);
        chSysUnlock();
        if(ClkState == OK) {
            Led.StartSequence(lsq48Ok);
            break;
        }
        else {
            Uart.PrintfI("Hsi48 Fail\r");
            Led.StartSequence(lsq48Fail);
            chThdSleepMilliseconds(MS2ST(540));
        }
    }
    Clk.UpdateFreqValues();
    Clk.PrintFreqs();
    Clk.SelectUSBClock_HSI48();
    Clk.EnableCRS();
    UsbCDC.Connect();
#endif

    // Main cycle
    App.ITask();
}

__noreturn
void App_t::ITask() {
    while(true) {
        uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);

        if(EvtMsk & EVT_USB_READY) {
            Uart.Printf("UsbReady\r");
            Led.StartSequence(lsqUSB);
        }

        if(EvtMsk & EVT_USB_NEW_CMD) {
            Led.StartSequence(lsqUSBCmd);
            OnCmd((Shell_t*)&UsbCDC);
            UsbCDC.SignalCmdProcessed();
        }
        if(EvtMsk & EVT_UART_NEW_CMD) {
            OnCmd((Shell_t*)&Uart);
            Uart.SignalCmdProcessed();
        }
    } // while true
}

#if 1 // ======================= Command processing ============================
void App_t::OnCmd(Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
    __unused int32_t dw32 = 0;  // May be unused in some configurations
//    PShell->Printf(">%S\r", PCmd->Name);
//    Uart.Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(OK);
    }

    else if(PCmd->NameIs("Version")) {
        PShell->Printf("%S %S\r\n", APP_NAME, BUILD_TIME);
    }

    else if(PCmd->NameIs("GetInfo")) {
        rPkt_t Pkt;
        Pkt.Cmd = 0;    // GetInfo
        uint8_t Rslt = Radio.TxAndGetAnswer(&Pkt);
        if(Rslt == OK) {
            rPkt_t *p = &Radio.LastPktRx;
            PShell->Printf("Info %d %d %d %d %d %d %d %d\r\n",
                        p->t[0], p->t[1], p->t[2], p->t[3], p->t[4], p->t[5], p->t[6], p->t[7]);
        }
        else PShell->Ack(Rslt);
    }

    else if(PCmd->NameIs("SetParam")) {
        rPkt_t Pkt;
        Pkt.Cmd = 1;    // SetParam
        if(PCmd->GetNextByte(&Pkt.Data1) != OK) { PShell->Ack(CMD_ERROR); return; }
        if(PCmd->GetNextByte(&Pkt.Data2) != OK) { PShell->Ack(CMD_ERROR); return; }
//        Uart.Printf("%S %d %d\r", PCmd->Name, Pkt.Data1, Pkt.Data2);
        uint8_t Rslt = Radio.TxAndGetAnswer(&Pkt);
        PShell->Ack(Rslt);
    }

    else PShell->Ack(CMD_UNKNOWN);
}
#endif
