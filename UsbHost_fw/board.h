/*
 * board.h
 *
 *  Created on: 12 сент. 2015 г.
 *      Author: Kreyl
 */

#pragma once

#include <inttypes.h>
#include "CRS_defins.h"

// ==== General ====
#define BOARD_NAME          "UsbHost"
// MCU type as defined in the ST header.
#define STM32F072xB

#define CRYSTAL_FREQ_HZ     12000000    // Freq of external crystal - not used

#define SYS_TIM_CLK         (Clk.APBFreqHz * Clk.TimerClkMulti)

#if 1 // ========================== GPIO =======================================
// UART
#define UART_GPIO       GPIOA
#define UART_TX_PIN     9
#define UART_RX_PIN     10
#define UART_AF         AF1

// LEDs
#define LED_GPIO        GPIOB
#define LED1_PIN        0
#define LED2_PIN        1

// USB
#define USB_GPIO		GPIOA
#define USB_DM_PIN		11
#define USB_DP_PIN		12

#endif

#if 1 // ========================= Timer =======================================
#endif

#if 1 // =========================== SPI =======================================
#endif

#if 1 // ========================== USART ======================================
#define UART            USART1
#define UART_TX_REG     UART->TDR
#define UART_RX_REG     UART->RDR
#endif

#if 1 // ========================== USB ========================================
#define USBDrv          USBD1   // USB driver to use

// CRS
#define CRS_PRESCALER   RCC_CRS_SYNC_DIV1
#define CRS_SOURCE      RCC_CRS_SYNC_SOURCE_USB
#define CRS_POLARITY    RCC_CRS_SYNC_POLARITY_RISING
#define CRS_RELOAD_VAL  ((48000000 / 1000) - 1) // Ftarget / Fsync - 1
#define CRS_ERROR_LIMIT 34
#define HSI48_CALIBRATN 32
#endif

#if 1 // =========================== DMA =======================================
#define STM32_DMA_REQUIRED  TRUE
// Uart
#define UART_DMA_TX     STM32_DMA1_STREAM4
#define UART_DMA_RX     STM32_DMA1_STREAM5
#define UART_DMA_CHNL   0   // Dummy

#endif
