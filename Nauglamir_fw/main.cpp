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
#endif

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

#if 1 // ==== Acc ====
Acc_t Acc(&i2c1);
static void ProcessAcc();
static TmrKL_t TmrAccRead {TIME_MS2I(11), evtIdAccRead, tktPeriodic};

// Flickering params
#define ACC_STEADY_VALUE        1100
#define BRT_TOP_VALUE           255
#define BRT_LO_VALUE            22
#define BRT_TOP_VALUE_x1024     (BRT_TOP_VALUE * 1024)
#define BRT_LO_VALUE_x1024      (BRT_LO_VALUE * 1024)
#define BRT_STEP                306
#define T_TOP_VALUE             900
#define T_LO_VALUE              207
uint32_t TMax = T_TOP_VALUE;
uint32_t TMin = 180;
uint32_t BrtHi_x1024 = BRT_TOP_VALUE * 1024;    // Max brightness possible, scaled
// Average
#define AVERAGE_SZ  90
class MovingAverage_t {
private:
    uint32_t IValue[AVERAGE_SZ];
    uint32_t Indx = 0;
public:
    uint32_t AddAndGetAve(uint32_t AValue) {
        IValue[Indx] = AValue;
        if(++Indx >= AVERAGE_SZ) Indx = 0;
        uint32_t r = 0;
        for(auto n : IValue) r += n;
        r /= AVERAGE_SZ;
        return r;
    }
} Avera;
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


    TmrAccRead.StartOrRestart();

    // Main cycle
    ITask();
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdAccRead: ProcessAcc(); break;

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
    LedSmoothChunk_t *ptr = lsqFlicker[Indx];
    ptr[0].Time_ms    = Random::Generate(TMin, TMax);
    ptr[0].Brightness = BrtHi_x1024 / 1024; // Scale it down
    ptr[1].Time_ms    = Random::Generate(TMin, TMax);
    ptr[1].Brightness = BRT_LO_VALUE;
    Leds[Indx]->StartOrRestart(ptr);
//    Printf("%u: t1=%u; b1=%u; t2=%u; b2=%u\r", Indx, ptr[0].Time_ms, ptr[0].Brightness, ptr[1].Time_ms, ptr[1].Brightness);
}

void ProcessAcc() {
    Acc.ReadAccelerations();
//    Printf("X: %d; Y: %d; Z: %d\r", Acc.Accelerations.xMSB, Acc.Accelerations.yMSB, Acc.Accelerations.zMSB);
    // Calculate acc module
    uint32_t a = Acc.Accelerations.xMSB * Acc.Accelerations.xMSB +
            Acc.Accelerations.yMSB * Acc.Accelerations.yMSB +
            Acc.Accelerations.zMSB * Acc.Accelerations.zMSB;
    uint32_t e = (a > ACC_STEADY_VALUE)? (a - ACC_STEADY_VALUE) : (ACC_STEADY_VALUE - a);
    uint32_t Ave = Avera.AddAndGetAve(e);

    // Change top brightness depending on a value
    if(e > 108)  {
        if((BrtHi_x1024 + BRT_STEP*30) < BRT_TOP_VALUE_x1024) BrtHi_x1024 += BRT_STEP*30;
        else BrtHi_x1024 = BRT_TOP_VALUE_x1024;
    }
    else { // Idle
        if((BrtHi_x1024 - BRT_STEP) > BRT_LO_VALUE_x1024) BrtHi_x1024 -= BRT_STEP;
        else BrtHi_x1024 = BRT_LO_VALUE_x1024;
    }

    // Change flicker speed depending on a value
    uint32_t TMaxNew = Proportion<int32_t>(90, 500, T_TOP_VALUE, T_LO_VALUE, (e>500)? 500:e);
//    Printf("a: %u; ave: %u;  Brt: %u; TMax: %u\r", a, Ave, BrtHi_x1024, TMaxNew);
//    Printf("a: %u; e: %u;   Brt: %u; TMax: %u\r", a, e, BrtHi_x1024, TMaxNew);
    Printf("a: %u; e: %u; Ave: %u;   Brt: %u; TMax: %u\r", a, e, Ave, BrtHi_x1024, TMaxNew);
    TMax = TMaxNew;
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
