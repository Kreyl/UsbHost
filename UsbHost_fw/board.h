/*
 * board.h
 *
 *  Created on: 01.02.2017
 *      Author: Kreyl
 */

#ifndef BOARD_H__
#define BOARD_H__

// ==== General ====
#define BOARD_NAME          "UsbHostLora"
#define APP_NAME            "UsbHostFlasks"

// MCU type as defined in the ST header.
#define STM32F072xB     // no matter, 8 or B

// Freq of external crystal if any. Leave it here even if not used.
#define CRYSTAL_FREQ_HZ         12000000

// Ch timer
#define STM32_ST_IRQ_PRIORITY   2
#define STM32_ST_USE_TIMER      2
#define SYS_TIM_CLK             (Clk.APBFreqHz)

#define SIMPLESENSORS_ENABLED   FALSE
#define BUTTONS_ENABLED         FALSE
#define ADC_REQUIRED            FALSE
#define I2C1_ENABLED            FALSE
#define I2C_USE_SEMAPHORE       FALSE

#if 1 // ========================== GPIO =======================================
// PortMinTim_t: GPIO, Pin, Tim, TimChnl, invInverted, omPushPull, TopValue

// UART
#define UART_GPIO       GPIOA
#define UART_TX_PIN     9
#define UART_RX_PIN     10

// USB
#define USB_DM          GPIOA, 11
#define USB_DP          GPIOA, 12

// LED
#define LED_R_PIN       { GPIOC, 6, TIM3, 1, invNotInverted, omPushPull, 255 }
#define LED_G_PIN       { GPIOC, 8, TIM3, 2, invNotInverted, omPushPull, 255 }
#define LED_B_PIN       { GPIOC, 7, TIM3, 3, invNotInverted, omPushPull, 255 }

// ==== Lora ====
#define SX_SPI          SPI1
#define SX_SCK          GPIOA,  5, omPushPull, pudNone, AF0
#define SX_MISO         GPIOA,  6, omPushPull, pudNone, AF0
#define SX_MOSI         GPIOA,  7, omPushPull, pudNone, AF0
#define SX_NSS          GPIOA, 4
#define SX_NRESET       GPIOC, 3
#define SX_DIO0_GPIO    GPIOA
#define SX_DIO0_PIN     0
#define SX_DIO1_GPIO    GPIOA
#define SX_DIO1_PIN     1
#define SX_DIO2_GPIO    GPIOA
#define SX_DIO2_PIN     2
#define SX_DIO3_GPIO    GPIOA
#define SX_DIO3_PIN     3
#define SX_DIO4_GPIO    GPIOC
#define SX_DIO4_PIN     4
#define SX_DIO5_GPIO    GPIOC
#define SX_DIO5_PIN     5

#endif // GPIO

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
// ==== Uart ====
#define UART_DMA_TX_MODE(Chnl) (STM32_DMA_CR_CHSEL(Chnl) | DMA_PRIORITY_LOW | STM32_DMA_CR_MSIZE_BYTE | STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MINC | STM32_DMA_CR_DIR_M2P | STM32_DMA_CR_TCIE)
#define UART_DMA_RX_MODE(Chnl) (STM32_DMA_CR_CHSEL(Chnl) | DMA_PRIORITY_MEDIUM | STM32_DMA_CR_MSIZE_BYTE | STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MINC | STM32_DMA_CR_DIR_P2M | STM32_DMA_CR_CIRC)
#define UART_DMA_TX     STM32_DMA_STREAM_ID(1, 2)
#define UART_DMA_RX     STM32_DMA_STREAM_ID(1, 3)
#define UART_DMA_CHNL   0   // Dummy

#endif // DMA

#if 1 // ========================== USART ======================================
#define PRINTF_FLOAT_EN FALSE
#define UART_TXBUF_SZ   2048
#define UART_RXBUF_SZ   1024
#define CMD_BUF_SZ      256

#define CMD_UART        USART1


#define CMD_UART_PARAMS \
    CMD_UART, UART_GPIO, UART_TX_PIN, UART_GPIO, UART_RX_PIN, \
    UART_DMA_TX, UART_DMA_RX, UART_DMA_TX_MODE(UART_DMA_CHNL), UART_DMA_RX_MODE(UART_DMA_CHNL), \
    uartclkHSI // independent clock

#endif

#endif //BOARD_H__
