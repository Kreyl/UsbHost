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
#define BOARD_NAME          "UsbHost3"
#define APP_NAME            "UsbHost3 AcgGlove"

// MCU type as defined in the ST header.
#define STM32F072xB

#define CRYSTAL_FREQ_HZ 12000000    // Freq of external crystal - not used

#define SYS_TIM_CLK     (Clk.APBFreqHz)

#define USB_ENABLED     FALSE

#if 1 // ========================== GPIO =======================================
// UART
#define UART_GPIO       GPIOA
#define UART_TX_PIN     9
#define UART_RX_PIN     10

// LED
#define LED_GPIO        GPIOB
#define LEDR_PIN        0
#define LEDG_PIN        4
#define LEDB_PIN        1

// Radio: SPI, PGpio, Sck, Miso, Mosi, Cs, Gdo0
#define CC_Setup0       SPI1, GPIOA, 5,6,7, 4, 3

// USB
#define USB_GPIO		GPIOA
#define USB_DM_PIN		11
#define USB_DP_PIN		12

#endif

#if 1 // ========================= Timer =======================================
// LED
#define LED_TMR         TIM3
#define LEDR_CHNL       3
#define LEDG_CHNL       1
#define LEDB_CHNL       4
#endif

#if 1 // =========================== SPI =======================================
#endif

#if 1 // ========================== USART ======================================
#define PRINTF_FLOAT_EN FALSE
#define CMD_UART        USART1
#define UART_USE_INDEPENDENT_CLK    TRUE
#define UART_TXBUF_SZ   1024
#endif

#if 1 // ========================== USB ========================================
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
#define UART_DMA_TX     STM32_DMA1_STREAM2
#define UART_DMA_RX     STM32_DMA1_STREAM3
#define UART_DMA_CHNL   0   // Dummy

#endif
