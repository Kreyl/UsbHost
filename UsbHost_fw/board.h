/*
 * board.h
 *
 *  Created on: 12 ����. 2015 �.
 *      Author: Kreyl
 */

#pragma once

#include <inttypes.h>
#include "CRS_defins.h"

// ==== General ====
#define BOARD_NAME          "UsbHost2"
// MCU type as defined in the ST header.
#define STM32F072xB

#define CRYSTAL_FREQ_HZ 12000000    // Freq of external crystal - not used

#define SYS_TIM_CLK     (Clk.APBFreqHz * Clk.TimerClkMulti)

#define USB_ENABLED     TRUE

#if 1 // ========================== GPIO =======================================
// UART
#define UART_GPIO       GPIOA
#define UART_TX_PIN     9
#define UART_RX_PIN     10
#define UART_AF         AF1

// RGB LED
#define LED_RED_CH      { GPIOB, 0, TIM3, 3, invNotInverted, omPushPull, 255 }
#define LED_GREEN_CH    { GPIOB, 4, TIM3, 1, invNotInverted, omPushPull, 255 }
#define LED_BLUE_CH     { GPIOB, 1, TIM3, 4, invNotInverted, omPushPull, 255 }

// USB
#define USB_GPIO		GPIOA
#define USB_DM_PIN		11
#define USB_DP_PIN		12

// CC
#define CC_GPIO         GPIOA
#define CC_GDO2         2
#define CC_GDO0_IRQ     CC_GPIO, 3, pudNone
#define CC_SCK          5
#define CC_MISO         6
#define CC_MOSI         7
#define CC_CS           4
#define CC_SPI_AF       AF0
#endif

#if 1 // =========================== SPI =======================================
#define CC_SPI          SPI1
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
