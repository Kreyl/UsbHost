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
DeviceMgr_t DevMgr;
QToDevices_t QToDevices;
QToHost_t QToHost;

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

    if(Radio.Init() == OK) Led.StartSequence(lsqStart);
    else {
        Led.StartSequence(lsqFailure);
        chThdSleepMilliseconds(4500);
    }

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
        // Unload there everything!
        PShell->Printf("Cnt %u\r\n", QToHost.Cnt);
        MsgToHost_t fmsg;
        while(QToHost.Get(&fmsg) == OK) fmsg.Printf(PShell);
    }

    else if(PCmd->NameIs("SetParam")) {
        uint8_t DevID, MsgID;
        if(PCmd->GetNextByte(&DevID) != OK) return;
        if(DevID >= DEVICE_CNT) { QToHost.Put(DevID, 0, CMD_ERROR); return; }
        if(PCmd->GetNextByte(&MsgID) != OK) { QToHost.Put(DevID, 0, CMD_ERROR); return; }
        uint8_t ParamID=0, ParamV=0;
        if(PCmd->GetNextByte(&ParamID) != OK) { QToHost.Put(DevID, MsgID, CMD_ERROR); return; }
        if(PCmd->GetNextByte(&ParamV) != OK) { QToHost.Put(DevID, MsgID, CMD_ERROR); return; }
        QToDevices.Put(DevID, MsgID, ParamID, ParamV);
        //r = DevMgr.Cmd4One(DevID, cmdSetParameter, ParamID, DParamV);
        //PShell->Ack(r);
    }

    else PShell->Ack(CMD_UNKNOWN);
}
#endif

#if 1 // ============================ Queues ===================================
uint8_t QToDevices_t::Put(uint8_t DevID, uint8_t MsgID, uint8_t ParamID, uint8_t ParamV) {
    Uart.Printf("H2D: ID %u; Msg %u; ParID %u; ParV %u\r\n", DevID, MsgID, ParamID, ParamV);
    QToHost.Put(DevID, MsgID, OK);

    return OK;
}

uint8_t QToHost_t::Put(uint8_t DevID, uint8_t MsgID, uint8_t Rslt) {
    if(Cnt > Q2H_MSG_CNT) return OVERFLOW;   // Overflow
    chSysLock();
    PWrite->DevID = DevID;
    PWrite->MsgID = MsgID;
    PWrite->MsgType = Rslt;
    if(++PWrite > (IBuf + (Q2H_MSG_CNT - 1))) PWrite = IBuf;   // Circulate buffer
    Cnt++;
    chSysUnlock();
    return OK;
}

uint8_t QToHost_t::Get(MsgToHost_t *pmsg) {
    if(Cnt == 0) return EMPTY;
    chSysLock();
    pmsg->DWord0 = PRead->DWord0;
    pmsg->DWord1 = PRead->DWord1;
    pmsg->Word = PRead->Word;
    if(++PRead > (IBuf + (Q2H_MSG_CNT - 1))) PRead = IBuf;     // Circulate buffer
    Cnt--;
    chSysUnlock();
    return OK;
}

#endif
