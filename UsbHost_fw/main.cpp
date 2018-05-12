/*
 * main.cpp
 *
 *  Created on: 26 дек. 2015 г.
 *      Author: Kreyl
 */

#include "hal.h"
#include "board.h"
#include "MsgQ.h"
#include "shell.h"
#include "uart.h"
#include "led.h"
#include "kl_lib.h"
#include "Sequences.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
extern CmdUart_t Uart;
void OnCmd(Shell_t *PShell);
void ITask();

LedRGB_t Led { {LED_GPIO, LEDR_PIN, LED_TMR, LEDR_CHNL}, {LED_GPIO, LEDG_PIN, LED_TMR, LEDG_CHNL}, {LED_GPIO, LEDB_PIN, LED_TMR, LEDB_CHNL} };

#endif

int main(void) {
    // ==== Setup clock frequency ====
//    Clk.EnablePrefetch();
//    Clk.SetupBusDividers(ahbDiv2, apbDiv1);
    Clk.UpdateFreqValues();

    // Init OS
    halInit();
    chSysInit();

    // ==== Init hardware ====
    Uart.Init(115200);
    Printf("\r%S %S\r", APP_NAME, BUILD_TIME);
    Clk.PrintFreqs();

//    Led.Init();

//    if(Radio.Init() == OK) Led.StartSequence(lsqStart);
//    else Led.StartSequence(lsqFailure);
//    chThdSleepMilliseconds(810);

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
    } // while true
}

#if UART_RX_ENABLED // ================= Command processing ====================
void OnCmd(Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
//    Uart.Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(retvOk);
    }

    else PShell->Ack(retvCmdUnknown);
}
#endif
