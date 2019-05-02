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

    else if(PCmd->NameIs("SetBtnList")) {
        uint32_t IBtnsEnabled = 0;
        uint8_t IID = 0;
        while(PCmd->GetNext<uint8_t>(&IID) == retvOk) {
            if(IID < 1 or IID > BTNS_CNT_MAX) {
                PShell->Ack(retvBadValue);
                return;
            }
            IBtnsEnabled |= 1<<(IID-1);
        }
        Radio.BtnsEnabled = IBtnsEnabled;
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
        rPkt_t Pkt;
        if(PCmd->GetNext<uint8_t>(&Pkt.ID) == retvOk and Pkt.ID >= 1 and Pkt.ID <= BTNS_CNT_MAX) {
            if(PCmd->GetNext<uint8_t>(&Pkt.R1) == retvOk and
               PCmd->GetNext<uint8_t>(&Pkt.G1) == retvOk and
               PCmd->GetNext<uint8_t>(&Pkt.B1) == retvOk) {
                Pkt.Cmd = CMD_SET;
                Radio.CmdTable[Pkt.ID-1] = Pkt;
                PShell->Ack(retvOk);
            }
            else PShell->Ack(retvCmdError);
        }
        else PShell->Ack(retvCmdError);
    }

    else if(PCmd->NameIs("Flash")) {
        rPkt_t Pkt;
        if(PCmd->GetNext<uint8_t>(&Pkt.ID) == retvOk and Pkt.ID >= 1 and Pkt.ID <= BTNS_CNT_MAX) {
            if(PCmd->GetNext<uint8_t>(&Pkt.R1) == retvOk and
               PCmd->GetNext<uint8_t>(&Pkt.G1) == retvOk and
               PCmd->GetNext<uint8_t>(&Pkt.B1) == retvOk and
               PCmd->GetNext<uint16_t>(&Pkt.Wait_ms) == retvOk and
               PCmd->GetNext<uint8_t>(&Pkt.R2) == retvOk and
               PCmd->GetNext<uint8_t>(&Pkt.G2) == retvOk and
               PCmd->GetNext<uint8_t>(&Pkt.B2) == retvOk
            ) {
                Pkt.Cmd = CMD_FLASH;
                Radio.CmdTable[Pkt.ID-1] = Pkt;
                PShell->Ack(retvOk);
            }
            else PShell->Ack(retvCmdError);
        }
        else PShell->Ack(retvCmdError);
    }

    else if(PCmd->NameIs("SetClrAll")) {
        rPkt_t Pkt;
        if(PCmd->GetNext<uint8_t>(&Pkt.R1) == retvOk and
           PCmd->GetNext<uint8_t>(&Pkt.G1) == retvOk and
           PCmd->GetNext<uint8_t>(&Pkt.B1) == retvOk) {
            Pkt.Cmd = CMD_SET;
            for(uint32_t i=0; i<BTNS_CNT_MAX; i++) {
                Pkt.ID = i+1;
                Radio.CmdTable[i] = Pkt;
            }
            PShell->Ack(retvOk);
        }
        else PShell->Ack(retvCmdError);
    }

    else PShell->Ack(retvCmdUnknown);
}
#endif
