/*
 * SnsPins.h
 *
 *  Created on: 17 ���. 2015 �.
 *      Author: Kreyl
 */

/* ================ Documentation =================
 * There are several (may be 1) groups of sensors (say, buttons and USB connection).
 *
 */

#ifndef PINSNSSETTINGS_H__
#define PINSNSSETTINGS_H__

#include "SimpleSensors.h"

#ifndef SIMPLESENSORS_ENABLED
#define SIMPLESENSORS_ENABLED   FALSE
#endif

#if SIMPLESENSORS_ENABLED
#define SNS_POLL_PERIOD_MS      72

// Handlers
extern void ProcessButtons(PinSnsState_t *PState, uint32_t Len);
extern void ProcessUsbDetect(PinSnsState_t *PState, uint32_t Len);
extern void ProcessCharging(PinSnsState_t *PState, uint32_t Len);

const PinSns_t PinSns[] = {
        // Buttons
        {BTN1_PIN, pudPullDown, ProcessButtons},
        {BTN2_PIN, pudPullDown, ProcessButtons},
        {BTN3_PIN, pudPullDown, ProcessButtons},
        // USB
        {USB_DETECT_PIN, pudPullDown, ProcessUsbDetect},
        // Charging
        {IS_CHARGING, pudPullUp, ProcessCharging}
};
#define PIN_SNS_CNT     countof(PinSns)

#endif  // if enabled

#endif //PINSNSSETTINGS_H__