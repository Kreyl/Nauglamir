#pragma once

// ==== General ====
#define BOARD_NAME          "Nauglamir1"
#define APP_NAME            "Nauglamir"

#define SIMPLESENSORS_ENABLED   FALSE

// MCU type as defined in the ST header.
#define STM32F030x6


/* ST driver system settings. */
#define STM32_ST_IRQ_PRIORITY               2
#define STM32_ST_USE_TIMER                  16

// Freq of external crystal if any. Leave it here even if not used.
#define CRYSTAL_FREQ_HZ     12000000

#define STM32_TIMCLK1       (Clk.APBFreqHz)
#define I2C1_ENABLED        TRUE
#define I2C_USE_SEMAPHORE   FALSE
#define ADC_REQUIRED        FALSE

#if 1 // ========================== GPIO =======================================
// PortMinTim_t: GPIO, Pin, Tim, TimChnl, invInverted, omPushPull, TopValue
// UART
#define UART_GPIO       GPIOA
#define UART_TX_PIN     2
#define UART_RX_PIN     3

// LED
#define LED_G_PIN       { GPIOB, 1, TIM3, 4, invInverted, omOpenDrain, 255 }
#define LED_B_PIN       { GPIOB, 0, TIM3, 3, invInverted, omOpenDrain, 255 }
#define LED_R_PIN       { GPIOB, 5, TIM3, 2, invInverted, omOpenDrain, 255 }

// Acc mma8452
#define ACC_IRQ_GPIO    GPIOF
#define ACC_IRQ_PIN     0
#define ACC_PWR_PIN     GPIOA, 5

// I2C
#if I2C1_ENABLED
#define I2C1_GPIO       GPIOA
#define I2C1_SCL        9
#define I2C1_SDA        10
#define I2C_AF          AF4
#endif

#endif // GPIO

#if I2C1_ENABLED // ====================== I2C ================================
#define I2C1_BAUDRATE   400000
#define I2C_PILL        i2c1
#endif

#if ADC_REQUIRED // ======================= Inner ADC ==========================
// Clock divider: clock is generated from the APB2
#define ADC_CLK_DIVIDER     adcDiv2

// ADC channels
//#define BAT_CHNL          1

#define ADC_VREFINT_CHNL    17  // All 4xx, F072 and L151 devices. Do not change.
#define ADC_CHANNELS        { ADC_VREFINT_CHNL }
#define ADC_CHANNEL_CNT     1   // Do not use countof(AdcChannels) as preprocessor does not know what is countof => cannot check
#define ADC_SAMPLE_TIME     ast96Cycles
#define ADC_SAMPLE_CNT      8   // How many times to measure every channel

#define ADC_SEQ_LEN         (ADC_SAMPLE_CNT * ADC_CHANNEL_CNT)

#endif

#if 1 // =========================== DMA =======================================
#define STM32_DMA_REQUIRED  TRUE
// ==== Uart ====
#define UART_DMA_TX_MODE(Chnl) (STM32_DMA_CR_CHSEL(Chnl) | DMA_PRIORITY_LOW | STM32_DMA_CR_MSIZE_BYTE | STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MINC | STM32_DMA_CR_DIR_M2P | STM32_DMA_CR_TCIE)
#define UART_DMA_RX_MODE(Chnl) (STM32_DMA_CR_CHSEL(Chnl) | DMA_PRIORITY_MEDIUM | STM32_DMA_CR_MSIZE_BYTE | STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MINC | STM32_DMA_CR_DIR_P2M | STM32_DMA_CR_CIRC)
#define UART_DMA_TX     STM32_DMA_STREAM_ID(1, 4)
#define UART_DMA_RX     STM32_DMA_STREAM_ID(1, 5)
#define UART_DMA_CHNL   0   // Dummy

#if I2C1_ENABLED // ==== I2C ====
#define I2C1_DMA_TX     STM32_DMA_STREAM_ID(1, 2)
#define I2C1_DMA_RX     STM32_DMA_STREAM_ID(1, 3)
#define I2C1_DMA_CHNL   0   // Dummy
#endif

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
#define UART_TXBUF_SZ   520
#define UART_RXBUF_SZ   99

#define UARTS_CNT       1

#define CMD_UART_PARAMS \
    USART1, UART_GPIO, UART_TX_PIN, UART_GPIO, UART_RX_PIN, \
    UART_DMA_TX, UART_DMA_RX, UART_DMA_TX_MODE(UART_DMA_CHNL), UART_DMA_RX_MODE(UART_DMA_CHNL)

#endif
