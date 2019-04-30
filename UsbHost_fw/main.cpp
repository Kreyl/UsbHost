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
#include "kl_i2c.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

#define RW_LEN_MAX  54

const PinOutput_t PillPwr {PILL_PWR_PIN};
LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };

Color_t Clr[24];
uint8_t IBuf[72];
void DrawFrame();

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
    Led.StartOrRestart(lsqStart);

    PillPwr.Init();
    PillPwr.SetHi();

    i2c2.Init();
    i2c2.Resume();

    UsbCDC.Init();
    Clk.EnableCRS();
    Clk.SelectUSBClock_HSI48();
    UsbCDC.Connect();

//    TmrEverySecond.StartOrRestart();

    // Main cycle
    ITask();
}

uint32_t n=0;

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdEverySecond:
                Printf("s");
                Clr[n] = clGreen;
                if(n == 0) Clr[23] = clBlack;
                else Clr[n-1] = clBlack;
                n++;
                if(n >= 24) n = 0;
                DrawFrame();
                break;

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
    i2c2.Standby();
//    PillPwr.SetLo();
//    __NOP(); __NOP(); __NOP(); __NOP(); // Allow power to fade
//    PillPwr.Deinit();
}

void Resume() {
//    PillPwr.Init();
//    PillPwr.SetHi();
//    __NOP(); __NOP(); __NOP(); __NOP(); // Allow power to rise
    i2c2.Resume();
}

#define BLOCK_CNT   12

uint8_t i2c_color_msg[74];

//void DrawLedBlock(uint8_t Addr,

void DrawLedBlock(uint8_t Addr, Color_t *PClr) {
    // Send colors data
    i2c_color_msg[0] = 0x01;
    for (int led_num=0; led_num < BLOCK_CNT; ++led_num) {
        int led_offset = led_num * 3;
        i2c_color_msg[0+led_offset + 1] = PClr[led_num].R;
        i2c_color_msg[1+led_offset + 1] = PClr[led_num].G;
        i2c_color_msg[2+led_offset + 1] = PClr[led_num].B;
    }
    i2c_color_msg[37] = 0x26;
    memset(i2c_color_msg + 38, 0x01, 36);
    i2c2.Write(Addr, i2c_color_msg, 74);

    // Send FrameOut
    uint8_t data[4];
    data[0] = 0x25;
    data[1] = 0x01;
    data[2] = 0x00;
    data[3] = 0x01;

    i2c2.Write(Addr, data, 2);
    i2c2.Write(Addr, data+2, 2);
}

void DrawFrame() {
    DrawLedBlock(0x3C, &Clr[0]);
    DrawLedBlock(0x3F, &Clr[12]);
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

    else if(PCmd->NameIs("tst")) {
        DrawFrame();
        PShell->Ack(retvOk);
    }


    else if(PCmd->NameIs("set")) {
        if(PCmd->GetArray<uint8_t>(IBuf, 72) == retvOk) {
            // Map buf on colors
            uint8_t *p = IBuf;
            for(int i=0; i<24; i++) {
                Clr[i].FromRGB(p[0], p[1], p[2]);
                p += 3;
            }
            DrawFrame();
            PShell->Ack(retvOk);
        }
        else PShell->Ack(retvBadValue);
    }


    else if(PCmd->NameIs("Scan")) {
        i2c2.ScanBus(PShell);
    }

    // W <Addr> <Len <= 54 > (Data1, Data2, ..., DataLen)
    else if(PCmd->NameIs("W")) {
        uint8_t Addr, Len, Data[RW_LEN_MAX];
        if(PCmd->GetNext<uint8_t>(&Addr) == retvOk) {
            if(PCmd->GetNext<uint8_t>(&Len) == retvOk) {
                if(Len > RW_LEN_MAX) Len = RW_LEN_MAX;
                if(PCmd->GetArray<uint8_t>(Data, Len) == retvOk) {
                    Resume();
                    uint8_t Rslt = i2c2.Write(Addr, Data, Len);
                    Standby();
                    PShell->Ack(Rslt);
                }
                else PShell->Ack(retvCmdError);
            }
            else PShell->Ack(retvCmdError);
        }
        else PShell->Ack(retvCmdError);
    }

    // WriteRead: WR <Addr> <LenW> <LenR> (Data1, Data2, ..., DataLen)
    else if(PCmd->NameIs("WR")) {
        uint8_t Addr, LenW, LenR, Data[RW_LEN_MAX];
        if(PCmd->GetNext<uint8_t>(&Addr) == retvOk) {
            if(PCmd->GetNext<uint8_t>(&LenW) == retvOk) {
                if(LenW > RW_LEN_MAX) LenW = RW_LEN_MAX;
                if(PCmd->GetNext<uint8_t>(&LenR) == retvOk) {
                    if(LenR > RW_LEN_MAX) LenR = RW_LEN_MAX;
                    if(PCmd->GetArray<uint8_t>(Data, LenW) == retvOk) {
                        Resume();
                        uint8_t Rslt = i2c2.WriteRead(Addr, Data, LenW, Data, LenR);
                        Standby();
                        if(Rslt == retvOk) {
                            PShell->Print("%A\r\n", Data, LenR, ' ');
                        }
                        else PShell->Ack(Rslt);
                    }
                    else PShell->Ack(retvCmdError);
                }
                else PShell->Ack(retvCmdError);
            }
            else PShell->Ack(retvCmdError);
        }
        else PShell->Ack(retvCmdError);
    }

    else PShell->Ack(retvCmdUnknown);
}
#endif
