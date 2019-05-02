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
#include "pill_mgr.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };
static TmrKL_t TmrEverySecond {MS2ST(1000), evtIdEverySecond, tktPeriodic};
extern rPkt_t TxPkt;
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
    i2c2.Init();
    PillMgr.Init();

    if(Radio.Init() == retvOk) Led.StartOrRestart(lsqStart);
    else Led.StartOrRestart(lsqFailure);
    chThdSleepMilliseconds(1008);

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

            case evtIdPillConnected:
                Printf("Pill: %u\r", PillMgr.Pill.DWord32);
//                Led.StartOrRestart(lsqPillIn);
                break;

            case evtIdPillDisconnected:
                Printf("Pill disconn\r");
//                Led.StartOrRestart(lsqNoPill);
                break;

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
//    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(retvOk);
    }

    else if(PCmd->NameIs("ShowRx")) {
        Radio.ShowRx = true;
        PShell->Ack(retvOk);
    }
    else if(PCmd->NameIs("HideRx")) {
        Radio.ShowRx = false;
        PShell->Ack(retvOk);
    }

    else if(PCmd->NameIs("set")) {
        uint16_t To;
        int8_t RssiThr;
        uint8_t PwrId;
        if(PCmd->GetNext<uint16_t>(&To) != retvOk) { PShell->Ack(retvCmdError); return; }
        if(PCmd->GetNext<int8_t>(&RssiThr) != retvOk) { PShell->Ack(retvCmdError); return; }
        if(PCmd->GetNext<uint8_t>(&PwrId) != retvOk) { PShell->Ack(retvCmdError); return; }
        if(PwrId > 11) PwrId = 11;
        chSysLock();
        Radio.TxPkt.From = 1;
        Radio.TxPkt.To = To;
        Radio.TxPkt.RssiThr = RssiThr;
        Radio.TxPkt.PowerLvlId = PwrId;
        Radio.MustTx = true;
        chSysUnlock();
        PShell->Ack(retvOk);
    }

    else if(PCmd->NameIs("stop")) {
        Radio.MustTx = false;
        PShell->Ack(retvOk);
    }

#if 1 // === Pill ===
    else if(PCmd->NameIs("ReadPill")) {
        int32_t DWord32;
        uint8_t Rslt = PillMgr.Read(0, &DWord32, 4);
        if(Rslt == retvOk) {
            PShell->Print("Read %d\r\n", DWord32);
        }
        else PShell->Ack(retvFail);
    }

    else if(PCmd->NameIs("WritePill")) {
        int32_t DWord32;
        if(PCmd->GetNext<int32_t>(&DWord32) == retvOk) {
            uint8_t Rslt = PillMgr.Write(0, &DWord32, 4);
            PShell->Ack(Rslt);
        }
        else PShell->Ack(retvCmdError);
    }
#endif

    else PShell->Ack(retvCmdUnknown);
}
#endif
