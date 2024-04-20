#include "uart.h"
#include "ch.h"
#include "hal.h"
#include "MsgQ.h"
#include "shell.h"
#include "kl_lib.h"
#include "Sequences.h"
#include "led.h"
#include "color.h"
#include "usb_cdc.h"
#include "Lora.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN, 1000 };

// [2; 20]
#define TX_PWR_dBm      11
// bwLora125kHz, bwLora250kHz, bwLora500kHz
#define LORA_BW         bwLora250kHz
// sprfact64chipsPersym, sprfact128chipsPersym, sprfact256chipsPersym, sprfact512chipsPersym,
// sprfact1024chipsPersym, sprfact2048chipsPersym, sprfact4096chipsPersym
#define LORA_SPREADRFCT sprfact1024chipsPersym
// coderate4s5, coderate4s6, coderate4s7, coderate4s8
#define LORA_CODERATE   coderate4s8
#endif


void main(void) {
    // ==== Init Clock system ====
    Clk.EnablePrefetch();
    Clk.SetupFlashLatency(48000000);
    Clk.SetupBusDividers(ahbDiv1, apbDiv1);
    Clk.SwitchTo(csHSI48);
    Clk.UpdateFreqValues();

    // === Init OS ===
    halInit();
    chSysInit();
    EvtQMain.Init();

    // ==== Init hardware ====
    Uart.Init();
    Printf("\r%S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
    Clk.PrintFreqs();

    Led.Init();
    Led.StartOrRestart(lsqStart);

    Lora.Init();
    Lora.SetChannel(868000000);
    Lora.SetupRxConfigLora(LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit, 64);
    Lora.SetupTxConfigLora(TX_PWR_dBm, LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit);

    UsbCDC.Init();
    Clk.EnableCRS();
    Clk.SelectUSBClock_HSI48();
    UsbCDC.Connect();

    ITask(); // Main cycle
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdShellCmdRcvd:
                while(((CmdUart_t*)Msg.Ptr)->TryParseRxBuff() == retvOk) OnCmd((Shell_t*)((CmdUart_t*)Msg.Ptr));
                break;

#if 1       // ======= USB =======
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

            case evtIdUsbDisconnect: {
                UsbCDC.Disconnect();
                Clk.DisableCRS();
                Printf("USB disconnect\r");
            } break;

            case evtIdUsbReady:
                Printf("USB ready\r");
                Led.StartOrRestart(lsqUsbReady);
                break;
#endif

            default: break;
        } // switch
    } // while true
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

#if 1 // =========================== Pkt_t =====================================
#pragma pack(push, 1)
union rPkt_t {
    struct {
        uint32_t reply;
        uint32_t salt_reply;
    };
    struct {
        uint8_t cmd;
        union {
            struct { int16_t grif, slyze, rave, huff; };
            struct {
                uint16_t year;
                uint8_t month, day, hours, minutes;
            };
        };
    };
};
#pragma pack(pop)

static const uint8_t kcmd_set_shown = 0;
static const uint8_t kcmd_set_hidden = 1;
static const uint8_t kcmd_set_time = 7;

#define RPKT_SALT   0xF1170511 // Fly to sly
#define RPKT_LEN    sizeof(rPkt_t)
rPkt_t rpkt;
static uint8_t rxbuf[LORA_FIFO_SZ];
#endif

void TryToTxRpkt(Shell_t *PShell) {
    for(uint8_t i=0; i<4; i++) { // Try several times
        PShell->Print("Try %u\r\n", i);
        Lora.SetupTxConfigLora(TX_PWR_dBm, LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit);
        Lora.TransmitByLora((uint8_t*)&rpkt, RPKT_LEN);
        uint8_t len = LORA_FIFO_SZ;
        Lora.SetupRxConfigLora(LORA_BW, LORA_SPREADRFCT, LORA_CODERATE, hdrmodeExplicit, 64);
        uint8_t Rslt = Lora.ReceiveByLora(rxbuf, &len, 270);
        rPkt_t *pkt_rx = (rPkt_t*)rxbuf;
        if(Rslt == retvOk and len == RPKT_LEN and pkt_rx->salt_reply == RPKT_SALT) {
            PShell->Print("Result: Ok  SNR: %d; RSSI: %d\r\n", Lora.RxParams.SNR, Lora.RxParams.RSSI);
            return;
//                PShell->Print("Rply: %X; SNR: %d; RSSI: %d\r", PktRx->Reply, Lora.RxParams.SNR, Lora.RxParams.RSSI);
        }
        else if(Rslt == retvCRCError) PShell->Print("Result: CRCErr\r\n");
        else PShell->Print("Result: Timeout\r\n");
        chThdSleepMilliseconds(270);
    } // for
    PShell->Print("Result: Fail\r\n");
}


#if 1 // ======================= Command processing ============================
void OnCmd(Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
//    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) PShell->Ok();
    else if(PCmd->NameIs("Version")) PShell->Print("Version: %S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
    else if(PCmd->NameIs("mem")) PrintMemoryInfo();

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

    else if(PCmd->NameIs("Set")) {
        int32_t arr[5];
        if(PCmd->GetArray(arr, 5) == retvOk) {
            rpkt.grif  = arr[0];
            rpkt.slyze = arr[1];
            rpkt.rave  = arr[2];
            rpkt.huff  = arr[3];
            rpkt.cmd = arr[4]? kcmd_set_hidden : kcmd_set_shown;
            TryToTxRpkt(PShell);
        }
        else PShell->BadParam();
    }

    else if(PCmd->NameIs("SetTime")) {
        uint16_t arr[5];
        if(PCmd->GetArray(arr, 5) == retvOk) {
            rpkt.cmd = kcmd_set_time;
            rpkt.year    = arr[0];
            rpkt.month   = arr[1];
            rpkt.day     = arr[2];
            rpkt.hours   = arr[3];
            rpkt.minutes = arr[4];
            TryToTxRpkt(PShell);
        }
        else PShell->BadParam();
    }

    // ==== Lora params ====
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

    else if(PCmd->NameIs("help")) {
        Printf( "SetTime <Year> <Month> <Day> <H> <M>\r"
                "Set <Grif> <Slyze> <Rave> <Huff> <is_hidden> - set points\r"
        );
    }

    else PShell->CmdUnknown();
}
#endif
