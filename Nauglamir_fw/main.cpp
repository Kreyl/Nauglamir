#include "board.h"
#include "led.h"
#include "kl_lib.h"
#include "MsgQ.h"
#include "main.h"
#include "acc_mma8452.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{&CmdUartParams};
static void ITask();
static void OnCmd(Shell_t *PShell);

#if 1 // ==== LEDs ====
#define LED_FREQ_HZ     630
#define LED_CNT         4
LedSmooth_t Led1(LED1_PIN, LED_FREQ_HZ);
LedSmooth_t Led2(LED2_PIN, LED_FREQ_HZ);
LedSmooth_t Led3(LED3_PIN, LED_FREQ_HZ);
LedSmooth_t Led4(LED4_PIN, LED_FREQ_HZ);
LedSmooth_t *Leds[LED_CNT] = {&Led1, &Led2, &Led3, &Led4};;

const LedSmoothChunk_t lsqStart[] = {
        {csSetup, 270, 153},
        {csSetup, 270, 20},
        {csEnd}
};

LedSmoothChunk_t lsqFlicker[LED_CNT][3] = {
        { // 0
            {csSetup, 360, 255},
            {csSetup, 360, 20},
            {csEnd},
        },
        { // 1
            {csSetup, 360, 255},
            {csSetup, 360, 20},
            {csEnd},
        },
        { // 2
            {csSetup, 360, 255},
            {csSetup, 360, 20},
            {csEnd},
        },
        { // 3
            {csSetup, 360, 255},
            {csSetup, 360, 20},
            {csEnd},
        },
};

static void StartNextLedSq(int32_t Indx);
#endif

Acc_t Acc(&i2c1);

static TmrKL_t TmrAccRead {TIME_MS2I(90), evtIdAccRead, tktPeriodic};
#endif

int main(void) {
    // ==== Init Vcore & clock system ====
    Clk.UpdateFreqValues();

    // === Init OS ===
    halInit();
    chSysInit();
    EvtQMain.Init();

    // ==== Init hardware ====
    Uart.Init();
    Printf("\r%S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
    Clk.PrintFreqs();

    // Power on Acc
    PinSetupOut(ACC_PWR_PIN, omPushPull);
    PinSetHi(ACC_PWR_PIN);
    chThdSleepMilliseconds(18);
    i2c1.Init();
//    i2c1.ScanBus();
    Acc.Init();

    for(int32_t i=0; i<LED_CNT; i++) {
        Leds[i]->Init();
        Leds[i]->StartOrRestart(lsqStart);
        Leds[i]->SetupSeqEndEvt(EvtMsg_t(evtIdLedDone, i));
        chThdSleepMilliseconds(270);
    }


//    TmrAccRead.StartOrRestart();

    // Main cycle
    ITask();
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdAccRead:
//                Acc.ReadAccelerations();
//                Printf("X: %d; Y: %d; Z: %d\r", Acc.Accelerations.xMSB, Acc.Accelerations.yMSB, Acc.Accelerations.zMSB);
                break;

            case evtIdLedDone: StartNextLedSq(Msg.Value); break;

            case evtIdShellCmd:
                OnCmd((Shell_t*)Msg.Ptr);
                ((Shell_t*)Msg.Ptr)->SignalCmdProcessed();
                break;
            default: Printf("Unhandled Msg %u\r", Msg.ID); break;
        } // Switch
    } // while true
} // ITask()

void StartNextLedSq(int32_t Indx) {
    uint32_t TMax = 900;
    uint32_t TMin = 270;
//    uint8_t BrtFadeInMax = 255;
//    uint8_t BrtFadeInMin = 240;
//    uint8_t BrtFadeOutMax = 90;
//    uint8_t BrtFadeOutMin = 27;
    LedSmoothChunk_t *ptr = lsqFlicker[Indx];
    ptr[0].Time_ms    = Random::Generate(TMin, TMax);
    ptr[0].Brightness = 255; //Random::Generate(BrtFadeInMin, BrtFadeInMax);
    ptr[1].Time_ms    = Random::Generate(TMin, TMax);
    ptr[1].Brightness = 27;//Random::Generate(BrtFadeOutMin, BrtFadeOutMax);
    Leds[Indx]->StartOrRestart(ptr);
//    Printf("%u: t1=%u; b1=%u; t2=%u; b2=%u\r", Indx, ptr[0].Time_ms, ptr[0].Brightness, ptr[1].Time_ms, ptr[1].Brightness);
}

#if 1 // ================= Command processing ====================
void OnCmd(Shell_t *PShell) {
	Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
//    Uart.Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(retvOk);
    }
    else if(PCmd->NameIs("Version")) PShell->Print("%S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));

    else PShell->Ack(retvCmdUnknown);
}
#endif
