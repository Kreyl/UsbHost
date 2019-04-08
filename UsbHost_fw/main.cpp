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
#include "main.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };
static TmrKL_t TmrEverySecond {MS2ST(1000), evtIdEverySecond, tktPeriodic};

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
    Uart.Init();
    Printf("\r%S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
    Clk.PrintFreqs();

    Led.Init();
    TmrEverySecond.StartOrRestart();

    if(Radio.Init() == retvOk) Led.StartOrRestart(lsqStart);
    else Led.StartOrRestart(lsqFailure);

    for(int i=0; i<BTNS_CNT_MAX; i++) Radio.ColorsTable[i] = clYellow;

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
            case evtIdEverySecond:
//                Printf("S %u\r", chVTGetSystemTimeX());
//                Radio.Timer += 10000;
                break;

            case evtIdUsbNewCmd:
            case evtIdShellCmd:
                Led.StartOrRestart(lsqUSBCmd); // After that, falling throug is intentional
                OnCmd((Shell_t*)Msg.Ptr);
                ((Shell_t*)Msg.Ptr)->SignalCmdProcessed();
                break;

//            case evtIdRadioRx: {
////                Printf("Rx: %d\r", Msg.Value);
//
//            } break;

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

    else if(PCmd->NameIs("RstTmr")) {
        chSysLock();
        for(uint32_t i=0; i<BTNS_CNT_MAX; i++) {
            Radio.TimeAfterPressTable[i] = -1;
        }
        Radio.Timer = chVTGetSystemTimeX();
        chSysUnlock();
        PShell->Ack(retvOk);
    }

    else if(PCmd->NameIs("GetBtns")) {
        PShell->Print("Btns: ");
        for(uint32_t i=0; i<BTNS_CNT_MAX; i++) {
            if(!(Radio.BtnsEnabled & (1<<i))) continue;
            PShell->Print("%d %d ", i+1, Radio.TimeAfterPressTable[i]);
        }
        PShell->Print("\r\n");
    }

    else if(PCmd->NameIs("SetClr")) {
        uint8_t ID = 0;
        Color_t Clr;
        if(PCmd->GetNext<uint8_t>(&ID) == retvOk and ID >= 1 and ID <= BTNS_CNT_MAX) {
            if(PCmd->GetArray((uint8_t*)&Clr, 3) == retvOk) {
                Radio.ColorsTable[ID-1] = Clr;
                PShell->Ack(retvOk);
            }
            else PShell->Ack(retvCmdError);
        }
        else PShell->Ack(retvCmdError);
    }

    else if(PCmd->NameIs("SetClrAll")) {
        Color_t Clr;
        if(PCmd->GetArray((uint8_t*)&Clr, 3) == retvOk) {
            for(uint32_t i=0; i<BTNS_CNT_MAX; i++) {
                Radio.ColorsTable[i] = Clr;
            }
            PShell->Ack(retvOk);
        }
        else PShell->Ack(retvCmdError);
    }

//    else if(PCmd->NameIs("Set")) {
//        uint8_t r = retvFail;;
//        uint32_t ID = 0;
//        StickSetup_t FSetup;
//        if(PCmd->GetNext<uint32_t>(&ID) == retvOk and ID <= 1) {
//            if(PCmd->GetNext<uint8_t>(&FSetup.Vibro) == retvOk) {   // Vibro
//                if(PCmd->GetArray((uint8_t*)&FSetup.Clr, 4) == retvOk) {
//                    chSysLock();
//                    StickSetup[ID] = FSetup;
//                    chSysUnlock();
//                    r = retvOk;
//                }
//            }
//        }
//        if(r != retvOk) PShell->Ack(retvCmdError);
//    }

    else PShell->Ack(retvCmdUnknown);
}
#endif
