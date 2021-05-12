#include "hal.h"
#include "board.h"
#include "MsgQ.h"
#include "uart2.h"
#include "shell.h"
#include "kl_lib.h"
#include "led.h"
#include "Sequences.h"
#include "usb_cdc.h"
#include "kl_i2c.h"
#include "kl_crc.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

const PinOutput_t PillPwr {PILL_PWR_PIN};
LedRGB_t Led { LED_R_PIN, LED_G_PIN, LED_B_PIN };

Spi_t ISpi{SPI1};
const uint8_t SpiParams = 0x05; // MSB, IdleLow, FirstEdge, divider=101 => 64
#define BIG_BUF_SZ          2048
uint8_t BigBuf[BIG_BUF_SZ];
#define TIMEOUT_LONG_ms     9999UL

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
            case evtIdUartCmdRcvd:
                if(((CmdUart_t*)Msg.Ptr)->TryParseRxBuff() == retvOk) OnCmd((Shell_t*)((CmdUart_t*)Msg.Ptr));
                break;

            case evtIdUsbCmdRcvd:
                OnCmd((Shell_t*)&UsbCDC);
                UsbCDC.SignalCmdProcessed();
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
    Led.StartOrRestart(lsqUSBCmd);
	Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
//    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ok();
    }

    else if(PCmd->NameIs("send64")) {
        PShell->Print("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef");
    }

    else if(PCmd->NameIs("SpiF")) {
        uint32_t F;
        if(PCmd->GetNext<uint32_t>(&F) == retvOk) {
            ISpi.Disable();
            ISpi.Setup(boMSB, cpolIdleLow, cphaFirstEdge, F);
            ISpi.Enable();
            PShell->Ok();
        }
    }

    else if(PCmd->NameIs("SpiW")) {
        uint8_t b;
        CsLo();
        ISpi.Enable();
        while(PCmd->GetNext<uint8_t>(&b) == retvOk) {
            b = ISpi.ReadWriteByte(b);
            PShell->Print("0x%02X ", b);
        }
        ISpi.Disable();
        CsHi();
        PShell->EOL();
        PShell->Ok();
    }

    else if(PCmd->NameIs("SpiWCRC")) {
        uint8_t b;
        Crc::ChunkStart();
        CsLo();
        ISpi.Enable();
        while(PCmd->GetNext<uint8_t>(&b) == retvOk) {
            Crc::ChunkPutNext(b);
            b = ISpi.ReadWriteByte(b);
            PShell->Print("0x%02X ", b);
        }
        // Put CRC
        uint16_t crc = Crc::ChunkGetResult();
        b = ISpi.ReadWriteByte(crc & 0x00FF);
        PShell->Print("0x%02X ", b);
        b = ISpi.ReadWriteByte((crc >> 8) & 0x00FF);
        PShell->Print("0x%02X ", b);

        ISpi.Disable();
        CsHi();
        PShell->EOL();
        PShell->Ok();
    }

    else if(PCmd->NameIs("SpiBWCRC")) {
        uint8_t b;
        uint16_t w;
        Crc::ChunkStart();
        CsLo();
        ISpi.Enable();
        if(PCmd->GetNext<uint8_t>(&b) != retvOk)  { PShell->BadParam(); return; }
        if(PCmd->GetNext<uint16_t>(&w) != retvOk)  { PShell->BadParam(); return; }
        Crc::ChunkPutNext(b);
        Crc::ChunkPutNext(0xFF & (w >> 0));
        Crc::ChunkPutNext(0xFF & (w >> 8));

        PShell->Print("0x%02X ", ISpi.ReadWriteByte(b));
        PShell->Print("0x%02X ", ISpi.ReadWriteByte(0xFF & (w >> 0)));
        PShell->Print("0x%02X ", ISpi.ReadWriteByte(0xFF & (w >> 8)));

        // Put CRC
        uint16_t crc = Crc::ChunkGetResult();
        PShell->Print("0x%02X ", ISpi.ReadWriteByte(crc & 0x00FF));
        PShell->Print("0x%02X ", ISpi.ReadWriteByte((crc >> 8) & 0x00FF));

        ISpi.Disable();
        CsHi();
        PShell->EOL();
        PShell->Ok();
    }

    else if(PCmd->NameIs("SpiFile")) {
        uint32_t Len;
        if(PCmd->GetNext<uint32_t>(&Len) != retvOk) { PShell->BadParam(); return; }
        Printf("Len: %u\r", Len);
        // Receive data
        if(UsbCDC.StartBinaryReception(TIMEOUT_LONG_ms) == retvOk) {
            CsLo();
            while(Len != 0) {
                uint32_t BytesToRcv = (Len > BIG_BUF_SZ)? BIG_BUF_SZ : Len;
                Printf("L: %u; btr: %u\r", Len, BytesToRcv);
                if(UsbCDC.ContinueBinaryReception(BigBuf, BytesToRcv, TIMEOUT_LONG_ms) == retvOk) {
//                    Printf("%S\r", BigBuf);
                    ISpi.Transmit(SpiParams, BigBuf, BytesToRcv);
                    Len -= BytesToRcv;
//                    chThdSleepMilliseconds(504);
                }
                else {
                    CsHi();
                    PShell->Timeout();
                    return;
                }
            } // while
            CsHi();
            PShell->Ok();
        } // StartBinaryReception
        else PShell->Timeout();
    }

    else if(PCmd->NameIs("SpiChunks")) {
        uint8_t Pre;
        int32_t TotalSz, ChunkSz, Delay;
        if(PCmd->GetNext<uint8_t>(&Pre)     != retvOk) { PShell->BadParam(); return; }
        if(PCmd->GetNext<int32_t>(&TotalSz) != retvOk) { PShell->BadParam(); return; }
        if(PCmd->GetNext<int32_t>(&ChunkSz) != retvOk) { PShell->BadParam(); return; }
        if(PCmd->GetNext<int32_t>(&Delay)   != retvOk) { PShell->BadParam(); return; }
        // Receive data
        if(UsbCDC.StartBinaryReception(TIMEOUT_LONG_ms) == retvOk) {
            while(TotalSz > 0) {
                int32_t ChunkToRcv = (TotalSz > ChunkSz)? ChunkSz : TotalSz;
                CsLo();
                uint8_t b = Pre;
                ISpi.Transmit(SpiParams, &b, 1); // b will be overwritten with reply
                Crc::ChunkStart();
                Crc::ChunkPutNext(Pre);
                while(ChunkToRcv > 0) {
                    uint32_t BytesToRcv = (ChunkToRcv > BIG_BUF_SZ)? BIG_BUF_SZ : ChunkToRcv;
                    Printf("TSz: %d; CSz: %d; btr: %d\r", TotalSz, ChunkToRcv, BytesToRcv);
                    if(UsbCDC.ContinueBinaryReception(BigBuf, BytesToRcv, TIMEOUT_LONG_ms) == retvOk) {
                        Crc::ChunkPutNext(BigBuf, BytesToRcv);
                        ISpi.Transmit(SpiParams, BigBuf, BytesToRcv);
                        ChunkToRcv -= BytesToRcv;
                    }
                    else {
                        CsHi();
                        PShell->Timeout();
                        return;
                    }
                }
                // Put Crc
                uint16_t crc = Crc::ChunkGetResult();
                ISpi.Transmit(SpiParams, (uint8_t*)&crc, 2);
                CsHi();
                chThdSleepMilliseconds(Delay);
                TotalSz -= ChunkSz;
            }
            PShell->Ok();
        }
        else PShell->Timeout();
    }

    else if(PCmd->NameIs("SpiChunksPoll")) {
        uint8_t bCmd;
        int32_t TotalSz, ChunkSz;
        if(PCmd->GetNext<uint8_t>(&bCmd)     != retvOk) { PShell->BadParam(); return; }
        if(PCmd->GetNext<int32_t>(&TotalSz) != retvOk) { PShell->BadParam(); return; }
        if(PCmd->GetNext<int32_t>(&ChunkSz) != retvOk) { PShell->BadParam(); return; }
        // Receive data
        if(UsbCDC.StartBinaryReception(TIMEOUT_LONG_ms) == retvOk) {
            while(TotalSz > 0) {
                int32_t ChunkToRcv = (TotalSz > ChunkSz)? ChunkSz : TotalSz;
                CsLo();
                uint8_t b = bCmd;
                ISpi.Transmit(SpiParams, &b, 1); // b will be overwritten with reply
                Crc::ChunkStart();
                Crc::ChunkPutNext(bCmd);
                while(ChunkToRcv > 0) {
                    uint32_t BytesToRcv = (ChunkToRcv > BIG_BUF_SZ)? BIG_BUF_SZ : ChunkToRcv;
//                    Printf("TSz: %d; CSz: %d; btr: %d\r", TotalSz, ChunkToRcv, BytesToRcv);
                    if(UsbCDC.ContinueBinaryReception(BigBuf, BytesToRcv, TIMEOUT_LONG_ms) == retvOk) {
                        Crc::ChunkPutNext(BigBuf, BytesToRcv);
                        ISpi.Transmit(SpiParams, BigBuf, BytesToRcv);
                        ChunkToRcv -= BytesToRcv;
                    }
                    else {
                        CsHi();
                        PShell->Timeout();
                        return;
                    }
                }
                // Put Crc
                uint16_t crc = Crc::ChunkGetResult();
                ISpi.Transmit(SpiParams, (uint8_t*)&crc, 2);
                CsHi();
                TotalSz -= ChunkSz;

                // Poll
                b = 1;
                uint8_t id = 0;
                while(!(id == 0xFE and b == 0)) {
                    chThdSleepMilliseconds(11);
                    CsLo();
                    id = ISpi.ReadWriteByte(0);
                    PShell->Print("0x%02X ", id);
                    PShell->Print("0x%02X ", ISpi.ReadWriteByte(0));
                    PShell->Print("0x%02X ", ISpi.ReadWriteByte(0));
                    b = ISpi.ReadWriteByte(0);
                    PShell->Print("0x%02X\r", b);
                    CsHi();
                    chThdSleepMilliseconds(11);
                }
            }
            PShell->Ok();
        }
        else PShell->Timeout();
    }

    else PShell->CmdUnknown();
}
#endif
