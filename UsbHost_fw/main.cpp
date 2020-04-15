#include "hal.h"
#include "board.h"
#include "MsgQ.h"
#include "uart.h"
#include "shell.h"
#include "kl_lib.h"
#include "led.h"
#include "Sequences.h"
#include "usb_cdc.h"
#include "kl_i2c.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

const PinOutput_t PillPwr {PILL_PWR_PIN};
LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };

Spi_t ISpi{SPI1};
#define RW_LEN_MAX          1024
uint8_t Data[RW_LEN_MAX];
void CsHi() { PinSetHi(GPIOA, 3); }
void CsLo() { PinSetLo(GPIOA, 3); }
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
    Led.StartOrRestart(lsqStart);

//    PillPwr.Init();
//    PillPwr.SetHi();
//    i2c2.Init();

    PinSetupOut      (GPIOA, 3, omPushPull);
    PinSetupAlterFunc(GPIOA, 5, omPushPull, pudNone, AF0);
    PinSetupAlterFunc(GPIOA, 6, omPushPull, pudNone, AF0);
    PinSetupAlterFunc(GPIOA, 7, omPushPull, pudNone, AF0);
    CsHi();
    ISpi.Setup(boMSB, cpolIdleLow, cphaFirstEdge, 1000000);
    ISpi.Enable();

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
                Led.StartOrRestart(lsqUSBCmd); // After that, falling throug is intentional
                OnCmd((Shell_t*)Msg.Ptr);
                ((Shell_t*)Msg.Ptr)->SignalCmdProcessed();
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
//                Led.StartOrRestart(lsqUsbReady);
                break;
#endif

            default: break;
        } // switch
    } // while true
} // ITask()

void Standby() {
//    i2c2.Standby();
//    PillPwr.SetLo();
//    __NOP(); __NOP(); __NOP(); __NOP(); // Allow power to fade
//    PillPwr.Deinit();
}

void Resume() {
//    PillPwr.Init();
//    PillPwr.SetHi();
//    __NOP(); __NOP(); __NOP(); __NOP(); // Allow power to rise
//    i2c2.Resume();
}

#if 1 // ================= Command processing ====================
void OnCmd(Shell_t *PShell) {
	Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
//    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(retvOk);
    }

    else if(PCmd->NameIs("SpiF")) {
        uint32_t F;
        if(PCmd->GetNext<uint32_t>(&F) == retvOk) {
            ISpi.Disable();
            ISpi.Setup(boMSB, cpolIdleLow, cphaFirstEdge, F);
            ISpi.Enable();
            PShell->Ack(retvOk);
        }
    }

    // W <Len <= 54 > (Data1, Data2, ..., DataLen)
    else if(PCmd->NameIs("SpiW")) {
        uint32_t Len;
        if(PCmd->GetNext<uint32_t>(&Len) == retvOk) {
        if(Len > RW_LEN_MAX) Len = RW_LEN_MAX;
            if(PCmd->GetArray<uint8_t>(Data, Len) == retvOk) {
                CsLo();
                uint8_t *p = Data;
                while(Len--) {
                    uint8_t b = ISpi.ReadWriteByte(*p++);
                    PShell->Print("0x%02X ", b);
                }
                PShell->Print("\r\n");
                PShell->Ack(retvOk);
                CsHi();
            }
            else PShell->Ack(retvCmdError);
        }
        else PShell->Ack(retvCmdError);
    }

    else PShell->Ack(retvCmdUnknown);
}
#endif
