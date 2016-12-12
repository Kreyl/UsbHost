/*
 * main.cpp
 *
 *  Created on: 26 ���. 2015 �.
 *      Author: Kreyl
 */

#include "main.h"
#include "usb_cdc.h"
#include "color.h"
#include "radio_lvl1.h"
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

#if USB_ENABLED
    UsbCDC.Init();
    chThdSleepMilliseconds(45);
    // Enable HSI48
    chSysLock();
    while(Clk.SwitchTo(csHSI48) != OK) {
        Uart.PrintfI("Hsi48 Fail\r");
        Led.SetColor(clRed);
        chThdSleepS(MS2ST(207));
        Led.SetColor(clBlack);
        chThdSleepS(MS2ST(207));
    }
    Clk.UpdateFreqValues();
    chSysUnlock();
    Clk.PrintFreqs();
    Clk.SelectUSBClock_HSI48();
    Clk.EnableCRS();
    UsbCDC.Connect();
#endif

    if(Radio.Init() != OK) {
        Led.StartSequence(lsqFailure);
        chThdSleepMilliseconds(2700);
    }
    else Led.StartSequence(lsqStart);

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
    uint8_t DevID;
//    PShell->Printf(">%S\r", PCmd->Name);
//    Uart.Printf("%S\r", PCmd->Name);
//    UsbCDC.Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(OK);
    }

    else if(PCmd->NameIs("GetInfo")) {
        if(PCmd->GetNextByte(&DevID) != OK) { PShell->Ack(CMD_ERROR); return; }
        if(DevID >= DEVICE_CNT) { PShell->Ack(CMD_ERROR); return; }
        if(Info[DevID].IsValid()) {
            Uart.Printf("%u, %u, %u, %u, %u, %u\r\n",
                    Info[DevID].Type, Info[DevID].Group, Info[DevID].Mode,
                    Info[DevID].State, Info[DevID].HitCnt, Info[DevID].LocalTime);
        }
        else PShell->Ack(TIMEOUT);
    }

    else if(PCmd->NameIs("Reset4Combat")) {
        // TODO
        PShell->Ack(OK);
    }

    else if(PCmd->NameIs("SetState")) {
        if(PCmd->GetNextByte(&DevID) != OK) { PShell->Ack(CMD_ERROR); return; }
        if(DevID >= DEVICE_CNT) { PShell->Ack(CMD_ERROR); return; }
        uint8_t DState=0;
        if(PCmd->GetNextByte(&DState) != OK) { PShell->Ack(CMD_ERROR); return; }
        // TODO
        PShell->Ack(OK);
    }

    else if(PCmd->NameIs("SetState4Combat")) {
        uint8_t DState=0;
        if(PCmd->GetNextByte(&DState) != OK) { PShell->Ack(CMD_ERROR); return; }
        // TODO
        PShell->Ack(OK);
    }

    else if(PCmd->NameIs("SetMode")) {
        if(PCmd->GetNextByte(&DevID) != OK) { PShell->Ack(CMD_ERROR); return; }
        if(DevID >= DEVICE_CNT) { PShell->Ack(CMD_ERROR); return; }
        uint8_t DMode=0;
        if(PCmd->GetNextByte(&DMode) != OK) { PShell->Ack(CMD_ERROR); return; }
        // TODO
        PShell->Ack(OK);
    }

    else if(PCmd->NameIs("SetParameter")) {
        if(PCmd->GetNextByte(&DevID) != OK) { PShell->Ack(CMD_ERROR); return; }
        if(DevID >= DEVICE_CNT) { PShell->Ack(CMD_ERROR); return; }
        uint8_t ParamID=0, DParamV=0;
        if(PCmd->GetNextByte(&ParamID) != OK) { PShell->Ack(CMD_ERROR); return; }
        if(PCmd->GetNextByte(&DParamV) != OK) { PShell->Ack(CMD_ERROR); return; }
        // TODO
        PShell->Ack(OK);
    }

    else PShell->Ack(CMD_UNKNOWN);
}
#endif
