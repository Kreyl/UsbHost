/*
 * board.h
 *
 *  Created on: 01.02.2017
 *      Author: Kreyl
 */

#pragma once

// ==== General ====
#define BOARD_NAME          "UsbHostLora"
#define APP_NAME            "UsbHostFlasks"

// MCU type as defined in the ST header.
#define STM32F072xB     // no matter, 8 or B

// Freq of external crystal if any. Leave it here even if not used.
#define CRYSTAL_FREQ_HZ         12000000

// OS timer settings
#define STM32_ST_IRQ_PRIORITY   2
#define STM32_ST_USE_TIMER      2
#define SYS_TIM_CLK             (Clk.APBFreqHz)    // Timer 5 is clocked by APB1


#define PILL_ENABLED            FALSE
#define BUTTONS_ENABLED         FALSE
#define SIMPLESENSORS_ENABLED   BUTTONS_ENABLED
#define ADC_REQUIRED            FALSE
#define I2C1_ENABLED            FALSE
#define I2C2_ENABLED            PILL_ENABLED

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
#define LED_G_PIN       { GPIOC, 7, TIM3, 2, invNotInverted, omPushPull, 255 }
#define LED_B_PIN       { GPIOC, 8, TIM3, 3, invNotInverted, omPushPull, 255 }

// I2C and Pill
#define I2C_PILL        i2c1
#define I2C_BAUDRATE_HZ 400000
#define I2C_GPIO        GPIOB
#define I2C_SCL         6
#define I2C_SDA         7
#define I2C_AF          AF1
#define PILL_PWR_PIN    GPIOB, 5, omPushPull

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
// CRS
#define CRS_PRESCALER   RCC_CRS_SYNC_DIV1
#define CRS_SOURCE      RCC_CRS_SYNC_SOURCE_USB
#define CRS_POLARITY    RCC_CRS_SYNC_POLARITY_RISING
#define CRS_RELOAD_VAL  ((48000000 / 1000) - 1) // Ftarget / Fsync - 1
#define CRS_ERROR_LIMIT 34
#define HSI48_CALIBRATN 32
#endif

#if ADC_REQUIRED // ======================= Inner ADC ==========================
#define ADC_MEAS_PERIOD_MS  36
// Clock divider: clock is generated from the APB2
#define ADC_CLK_DIVIDER     adcDiv2

// ADC channels
#define ADC_BAT_CHNL        0
#define ADC0_CHNL           10
#define ADC1_CHNL           11
#define ADC2_CHNL           12
#define ADC3_CHNL           13
#define ADC4_CHNL           14
#define ADC5_CHNL           15

#define ADC_VREFINT_CHNL    17  // All 4xx, F072 and L151 devices. Do not change.
#define ADC_CHANNELS        { ADC_BAT_CHNL, ADC0_CHNL, ADC1_CHNL, ADC2_CHNL, ADC3_CHNL, ADC4_CHNL, ADC5_CHNL, ADC_VREFINT_CHNL }
#define ADC_CHANNEL_CNT     8   // Do not use countof(AdcChannels) as preprocessor does not know what is countof => cannot check
#define ADC_SAMPLE_TIME     ast55d5Cycles
#define ADC_SAMPLE_CNT      8   // How many times to measure every channel

#define ADC_SEQ_LEN         (ADC_SAMPLE_CNT * ADC_CHANNEL_CNT)

#endif

#if 1 // =========================== DMA =======================================
#define STM32_DMA_REQUIRED  TRUE
// ==== Uart ====
#define UART_DMA_TX_MODE(Chnl) (STM32_DMA_CR_CHSEL(Chnl) | DMA_PRIORITY_LOW | STM32_DMA_CR_MSIZE_BYTE | STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MINC | STM32_DMA_CR_DIR_M2P | STM32_DMA_CR_TCIE)
#define UART_DMA_RX_MODE(Chnl) (STM32_DMA_CR_CHSEL(Chnl) | DMA_PRIORITY_MEDIUM | STM32_DMA_CR_MSIZE_BYTE | STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MINC | STM32_DMA_CR_DIR_P2M | STM32_DMA_CR_CIRC)
#define UART_DMA_TX     STM32_DMA_STREAM_ID(1, 2)
#define UART_DMA_RX     STM32_DMA_STREAM_ID(1, 3)
#define UART_DMA_CHNL   0   // Dummy

// ==== I2C1 ====
#define I2C1_DMA_TX     STM32_DMA1_STREAM2
#define I2C1_DMA_RX     STM32_DMA1_STREAM3
#define I2C1_DMA_CHNL   0 // Dummy

#define I2C2_DMA_TX     STM32_DMA1_STREAM4
#define I2C2_DMA_RX     STM32_DMA1_STREAM5
#define I2C2_DMA_CHNL   0 // Dummy

#if ADC_REQUIRED
#define ADC_DMA         STM32_DMA1_STREAM1
#define ADC_DMA_MODE    STM32_DMA_CR_CHSEL(0) |   /* dummy */ \
                        DMA_PRIORITY_LOW | \
                        STM32_DMA_CR_MSIZE_HWORD | \
                        STM32_DMA_CR_PSIZE_HWORD | \
                        STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                        STM32_DMA_CR_DIR_P2M |    /* Direction is peripheral to memory */ \
                        STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */
#endif // ADC

#endif // DMA

#if 1 // ========================== USART ======================================
#define PRINTF_FLOAT_EN FALSE
#define UART_TXBUF_SZ   2048
#define UART_RXBUF_SZ   4096
#define CMD_BUF_SZ      256

#define CMD_UART        USART1


#define CMD_UART_PARAMS \
    CMD_UART, UART_GPIO, UART_TX_PIN, UART_GPIO, UART_RX_PIN, \
    UART_DMA_TX, UART_DMA_RX, UART_DMA_TX_MODE(UART_DMA_CHNL), UART_DMA_RX_MODE(UART_DMA_CHNL), \
    uartclkHSI // independent clock

#endif
