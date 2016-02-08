/*
 * main.cpp
 *
 *  Created on: 26 дек. 2015 г.
 *      Author: Kreyl
 */

#include "main.h"
#include "SimpleSensors.h"
#include "usb_cdc.h"
#include "color.h"
#include "radio_lvl1.h"

App_t App;

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
    Uart.Printf("\r%S %S\r", APP_NAME, APP_VERSION);
    Clk.PrintFreqs();

#if USB_ENABLED
    UsbCDC.Init();
    chThdSleepMilliseconds(450);
    // Enable HSI48
    chSysLock();
    uint8_t r = Clk.SwitchTo(csHSI48);
    Clk.UpdateFreqValues();
    chSysUnlock();
    Clk.PrintFreqs();
    if(r == OK) {
        Clk.SelectUSBClock_HSI48();
        Clk.EnableCRS();
        UsbCDC.Connect();
    }
    else Uart.Printf("Hsi Fail\r");
#endif

    if(Radio.Init() != OK) {
//        Led.StartSequence(lsqFailure);
        chThdSleepMilliseconds(2700);
    }

    // Main cycle
    App.ITask();
}

__attribute__ ((__noreturn__))
void App_t::ITask() {
    while(true) {
        uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);

        if(EvtMsk & EVTMSK_USB_READY) {
            Uart.Printf("UsbReady\r");
        }

        if(EvtMsk & EVTMSK_USB_NEW_CMD) {
            OnCmd((Shell_t*)&UsbCDC);
            UsbCDC.SignalCmdProcessed();
        }
        if(EvtMsk & EVTMSK_UART_NEW_CMD) {
            OnCmd((Shell_t*)&Uart);
            Uart.SignalCmdProcessed();
        }
    } // while true
}

#if 1 // ======================= Command processing ============================
void App_t::OnCmd(Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
//    PShell->Printf(">%S\r", PCmd->Name);
    Uart.Printf("%S\r", PCmd->Name);
//    UsbCDC.Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(OK);
    }

//    else if(PCmd->NameIs("Set")) {
//        uint8_t Rslt = CMD_ERROR;
//        for(uint8_t i = 0; i < 3; i++) {
//            // Get array of params
//            if(PCmd->GetArray(LedParams[i].Arr, 5) == OK) {
////                LedParams[i].Print();
//                if(LedParams[i].Check(PShell) == OK) {
//                    Rslt = OK;
//                    uint8_t indx = LedParams[i].Indx - 1;
//                    chSysLock();
//                    LedWs.DesiredClr[indx].Set(LedParams[i].R, LedParams[i].G, LedParams[i].B);
//                    LedWs.SmoothValue[indx] = LedParams[i].Smooth;
//                    chSysUnlock();
//                }
//                else break;
//            }
//            else break;
//        } // for
//        if(Rslt == OK) LedWs.StartProcess();
//        PShell->Ack(Rslt);
//    }

    else PShell->Ack(CMD_UNKNOWN);
}
#endif
