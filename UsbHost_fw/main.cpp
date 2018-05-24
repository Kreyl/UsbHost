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
extern CmdUart_t Uart;
void OnCmd(Shell_t *PShell);
void ITask();

LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };

#endif

int main(void) {
    // ==== Init Clock system ====
    Clk.EnablePrefetch();
    Clk.SetupFlashLatency(48000000);
    Clk.SwitchTo(csHSI48);
    Clk.UpdateFreqValues();

    // === Init OS ===
    halInit();
    chSysInit();

    // ==== Init hardware ====
    EvtQMain.Init();
    Uart.Init(115200);
    Printf("\r%S %S\r", APP_NAME, BUILD_TIME);
    Clk.PrintFreqs();

    // LEDs
    Led.Init();

    if(Radio.Init() == retvOk) Led.StartOrRestart(lsqStart);
    else Led.StartOrRestart(lsqFailure);

    UsbCDC.Init();
    Clk.EnableCRS();
    Clk.SelectUSBClock_HSI48();
    UsbCDC.Connect();

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


#if UART_RX_ENABLED // ================= Command processing ====================
void OnCmd(Shell_t *PShell) {
	Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(retvOk);
    }

    else if(PCmd->NameIs("SetChannel")) {
        uint32_t Chnl;
        if(PCmd->GetNext<uint32_t>(&Chnl) == retvOk) {
            if(Chnl <= 99) {
                PShell->Ack(EvtQRadio.SendNowOrExit(EvtMsg_t(RMSG_SETCHNL, Chnl)));
            }
            else PShell->Ack(retvBadValue);
        }
        else PShell->Ack(retvBadValue);
    }

    else if(PCmd->NameIs("Send")) {
        uint8_t ID, Cmd, Param;
        if(PCmd->GetNext<uint8_t>(&ID) == retvOk) {
            if(PCmd->GetNext<uint8_t>(&Cmd) == retvOk) {
                if(PCmd->GetNext<uint8_t>(&Param) == retvOk) {
                    EvtMsg_t Msg;
                    Msg.ID = RMSG_SEND_PARAM;
                    Msg.b[0] = ID;
                    Msg.b[1] = Cmd;
                    Msg.b[2] = Param;
                    PShell->Ack(EvtQRadio.SendNowOrExit(Msg));
                }
                else PShell->Ack(retvBadValue);
            }
            else PShell->Ack(retvBadValue);
        }
        else PShell->Ack(retvBadValue);
    }

    else PShell->Ack(retvCmdUnknown);
}
#endif
