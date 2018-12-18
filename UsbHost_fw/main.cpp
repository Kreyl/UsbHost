#include "hal.h"
#include "board.h"
#include "MsgQ.h"
#include "uart.h"
#include "shell.h"
#include "kl_lib.h"
#include "led.h"
#include "Sequences.h"
#include "radio_lvl1.h"
#include "usb_cdc.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };

#endif

int main(void) {
    // ==== Init Clock system ====
//    Clk.EnablePrefetch();
//    Clk.SetupFlashLatency(48000000);
//    Clk.SwitchTo(csHSI48);
    Clk.UpdateFreqValues();

    // === Init OS ===
    halInit();
    chSysInit();

    // ==== Init hardware ====
    EvtQMain.Init();
    Uart.Init();
    for(int i=0; i<7; i++) {
        while(!(USART1->ISR & USART_ISR_TXE));
        USART1->TDR = 'a';
    }
//    Printf("\r%S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
//    Clk.PrintFreqs();

    // LEDs
//    Led.Init();

//    if(Radio.Init() == retvOk) Led.StartOrRestart(lsqStart);
//    else Led.StartOrRestart(lsqFailure);
//
//    UsbCDC.Init();
//    Clk.EnableCRS();
//    Clk.SelectUSBClock_HSI48();
//    UsbCDC.Connect();

    // Main cycle
    ITask();
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdUsbNewCmd:
            case evtIdShellCmd:
                Led.StartOrRestart(lsqUSBCmd); // After that, falling throug is intentional
                OnCmd((Shell_t*)Msg.Ptr);
                ((Shell_t*)Msg.Ptr)->SignalCmdProcessed();
                break;

            case evtIdRadioRx: {
//                Printf("Rx: %d\r", Msg.Value);

            } break;

#if 1 // ======= USB =======
            case evtIdUsbConnect:
                Printf("USB connect\r");
                Clk.EnableCRS();
                Clk.SelectUSBClock_HSI48();
                UsbCDC.Connect();
                break;
            case evtIdUsbDisconnect:
                Printf("USB disconnect\r");
                UsbCDC.Disconnect();
                Clk.DisableCRS();
                break;
            case evtIdUsbReady:
                Printf("USB ready\r");
                Led.StartOrRestart(lsqUsbReady);
                break;
#endif

            default: break;
        } // switch
    } // while true
} // ITask()


#if 1 // ================= Command processing ====================
void OnCmd(Shell_t *PShell) {
	Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
//    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(retvOk);
    }

    else if(PCmd->NameIs("Set")) {
        uint8_t Rslt = retvCmdError;
        // Read cmd
        if(PCmd->GetNext<uint8_t>(&Radio.PktTx.ID) != retvOk) goto SetEnd; // Get ID
        if(PCmd->GetArray(Radio.PktTx.State.Brightness, 5) != retvOk) goto SetEnd; // Get brightnesses
        // Get IR params
        if(PCmd->GetNext<uint8_t>(&Radio.PktTx.State.IRPwr) != retvOk) goto SetEnd;
        if(PCmd->GetNext<uint8_t>(&Radio.PktTx.State.IRData) != retvOk) goto SetEnd;
        // Transmit data and wait answer
        Rslt = Radio.TxRxSync();
        SetEnd:
        PShell->Ack(Rslt);
    } // Set

    else PShell->Ack(retvCmdUnknown);
}
#endif
