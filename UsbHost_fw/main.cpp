#include "hal.h"
#include "board.h"
#include "MsgQ.h"
#include "uart2.h"
#include "shell.h"
#include "kl_lib.h"
#include "led.h"
#include "Sequences.h"
#include "usb_cdc.h"
//#include "kl_i2c.h"
#include "Lora.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

const PinOutput_t PillPwr {PILL_PWR_PIN};
LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };

// [2; 20]
#define TX_PWR_dBm      5

#define LORA_BW         bwLora125kHz
#define LORA_SPREADRFCT sprfact128chipsPersym
#define LORA_CODERATE   coderate4s5
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
//    i2c1.Init();

    Lora.Init();
    Lora.SetChannel(868000000);

    Lora.SetupTxConfigLora(TX_PWR_dBm, LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit);
    Lora.SetupRxConfigLora(LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit, 64);

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
            case evtIdShellCmdRcvd:
                Led.StartOrRestart(lsqUSBCmd);
                while(((CmdUart_t*)Msg.Ptr)->TryParseRxBuff() == retvOk) OnCmd((Shell_t*)((CmdUart_t*)Msg.Ptr));
                break;

#if 1 // ======= USB =======
            case evtIdUsbCmdRcvd:
                Led.StartOrRestart(lsqUSBCmd);
                OnCmd((Shell_t*)&UsbCDC);
                UsbCDC.SignalCmdProcessed();
                break;

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

const char* strBW[3] = {"125kHz", "250kHz", "500kHz"};
SXLoraBW_t bw[3] = {bwLora125kHz, bwLora250kHz, bwLora500kHz};


const char* strSF[7] = {
        "64cps", "128cps", "256cps", "512cps", "1024cps", "2048cps", "4096cps"
};
SXSpreadingFactor_t sf[7] = {sprfact64chipsPersym, sprfact128chipsPersym, sprfact256chipsPersym,
        sprfact512chipsPersym, sprfact1024chipsPersym, sprfact2048chipsPersym, sprfact4096chipsPersym
};

const char* strCR[4] = {
        "4s5", "4s6", "4s7", "4s8"
};

SXCodingRate_t CR[4] = {
        coderate4s5, coderate4s6, coderate4s7, coderate4s8
};

#if 1 // ================= Command processing ====================
static uint8_t FBuf[LORA_FIFO_SZ];
void OnCmd(Shell_t *PShell) {
	Cmd_t *PCmd = &PShell->Cmd;
//    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) PShell->Ok();

    else if(PCmd->NameIs("TX")) {
        uint8_t Pwr, BWIndx, SFIndx, CRIndx;
        uint8_t FBuf[LORA_FIFO_SZ];
        if(PCmd->GetParams<uint8_t>(4, &Pwr, &BWIndx, &SFIndx, &CRIndx) == retvOk) {
            Lora.SetupTxConfigLora(Pwr, bw[BWIndx], sf[SFIndx], CR[CRIndx], hdrmodeExplicit);
            uint8_t b, Cnt=0;
            while(PCmd->GetNext<uint8_t>(&b) == retvOk) {
                FBuf[Cnt++] = b;
                if(Cnt == LORA_FIFO_SZ) break;
            }
            if(Cnt) {
                PShell->Print("Pwr=%u; Len=%u; %S; %S; %S; Dur=%u\r\n",
                        Pwr, Cnt, strBW[BWIndx], strSF[SFIndx], strCR[CRIndx],
                        Lora.TransmitByLora(FBuf, Cnt));
            }
            else PShell->CmdError();
        }
        else PShell->CmdError();
    }

    else if(PCmd->NameIs("SetPix")) {
        uint8_t b, Len=0;
        while(PCmd->GetNext<uint8_t>(&b) == retvOk) {
            FBuf[Len++] = b;
            if(Len == LORA_FIFO_SZ) break;
        }
        if(Len) {
            Lora.SetupTxConfigLora(TX_PWR_dBm, LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit);
            Lora.SetupRxConfigLora(LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit, 64);
            Lora.TransmitByLora(FBuf, Len);
            Len = LORA_FIFO_SZ;
            uint8_t Rslt = Lora.ReceiveByLora(FBuf, &Len, 1520);
            if(Rslt == retvOk) {
                PShell->Print("Ok SNR: %d; RSSI: %d\r", Lora.RxParams.SNR, Lora.RxParams.RSSI);
            }
            else if(Rslt == retvCRCError) PShell->Print("CRCErr\r");
            else PShell->Print("Timeout\r");
        }
        else PShell->CmdError();
    }

    else if(PCmd->NameIs("SetParams")) {
        uint8_t Pwr, BWIndx, SFIndx, CRIndx;
        if(PCmd->GetParams<uint8_t>(4, &Pwr, &BWIndx, &SFIndx, &CRIndx) == retvOk) {
            Lora.SetupTxConfigLora(Pwr, bw[BWIndx], sf[SFIndx], CR[CRIndx], hdrmodeExplicit);
            Lora.SetupRxConfigLora(bw[BWIndx], sf[SFIndx], CR[CRIndx], hdrmodeExplicit, 64);
            PShell->Print("Pwr=%u; %S; %S; %S\r\n", Pwr, strBW[BWIndx], strSF[SFIndx], strCR[CRIndx]);
        }
        else PShell->CmdError();
    }

    else if(PCmd->NameIs("Regs")) Lora.PrintRegs();
    else if(PCmd->NameIs("Sta")) Lora.PrintState();

    else PShell->CmdUnknown();
}
#endif
