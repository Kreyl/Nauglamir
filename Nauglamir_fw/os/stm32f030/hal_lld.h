/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    STM32F0xx/hal_lld.h
 * @brief   STM32F0xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_LSEDRV.
 *          - STM32_LSE_BYPASS (optionally).
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32F030x4, STM32F030x6, STM32F030x8, STM32F030xC,
 *            STM32F070x6, STM32F070xB for Value Line devices.
 *          - STM32F031x6, STM32F051x8, STM32F071xB, STM32F091xC
 *            for Access Line devices.
 *          - STM32F042x6, STM32F072xB for USB Line devices.
 *          - STM32F038xx, STM32F048xx, STM32F058xx, STM32F078xx,
 *            STM32F098xx for Low Voltage Line devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef HAL_LLD_H
#define HAL_LLD_H

/*
 * Registry definitions.
 */
#include "stm32_registry.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Platform identification macros
 * @{
 */
#if defined(STM32F030x4) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32F030x4 Entry Level Value Line devices"

#elif defined(STM32F030x6)
#define PLATFORM_NAME           "STM32F030x6 Entry Level Value Line devices"

#elif defined(STM32F030x8)
#define PLATFORM_NAME           "STM32F030x8 Entry Level Value Line devices"

#elif defined(STM32F030xC)
#define PLATFORM_NAME           "STM32F030xC Entry Level Value Line devices"

#elif defined(STM32F070x6)
#define PLATFORM_NAME           "STM32F070x6 Entry Level Value Line devices"

#elif defined(STM32F070xB)
#define PLATFORM_NAME           "STM32F070xB Entry Level Value Line devices"

#elif defined(STM32F031x6)
#define PLATFORM_NAME           "STM32F031x6 Entry Level Access Line devices"

#elif defined(STM32F051x8)
#define PLATFORM_NAME           "STM32F051x8 Entry Level Access Line devices"

#elif defined(STM32F071xB)
#define PLATFORM_NAME           "STM32F071xB Entry Level Access Line devices"

#elif defined(STM32F091xC)
#define PLATFORM_NAME           "STM32F091xC Entry Level Access Line devices"

#elif defined(STM32F042x6)
#define PLATFORM_NAME           "STM32F042x6 Entry Level USB Line devices"

#elif defined(STM32F072xB)
#define PLATFORM_NAME           "STM32F072xB Entry Level USB Line devices"

#elif defined(STM32F038xx)
#define PLATFORM_NAME           "STM32F038xx Entry Level Low Voltage Line devices"

#elif defined(STM32F048xx)
#define PLATFORM_NAME           "STM32F048xx Entry Level Low Voltage Line devices"

#elif defined(STM32F058xx)
#define PLATFORM_NAME           "STM32F058xx Entry Level Low Voltage Line devices"

#elif defined(STM32F078xx)
#define PLATFORM_NAME           "STM32F078xx Entry Level Low Voltage Line devices"

#elif defined(STM32F098xx)
#define PLATFORM_NAME           "STM32F098xx Entry Level Low Voltage Line devices"

#else
#error "STM32F0xx device unsupported or not specified"
#endif
/** @} */

/**
 * @name    Absolute Maximum Ratings
 * @{
 */
/**
 * @brief   Maximum system clock frequency.
 */
#define STM32_SYSCLK_MAX        48000000

/**
 * @brief   Maximum HSE clock frequency.
 */
#define STM32_HSECLK_MAX        32000000

/**
 * @brief   Minimum HSE clock frequency.
 */
#define STM32_HSECLK_MIN        1000000

/**
 * @brief   Maximum LSE clock frequency.
 */
#define STM32_LSECLK_MAX        1000000

/**
 * @brief   Minimum LSE clock frequency.
 */
#define STM32_LSECLK_MIN        32768

/**
 * @brief   Maximum PLLs input clock frequency.
 */
#define STM32_PLLIN_MAX         25000000

/**
 * @brief   Minimum PLLs input clock frequency.
 */
#define STM32_PLLIN_MIN         1000000

/**
 * @brief   Maximum PLL output clock frequency.
 */
#define STM32_PLLOUT_MAX        48000000

/**
 * @brief   Minimum PLL output clock frequency.
 */
#define STM32_PLLOUT_MIN        16000000

/**
 * @brief   Maximum APB clock frequency.
 */
#define STM32_PCLK_MAX          48000000
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
#define STM32_HSICLK            8000000     /**< High speed internal clock. */
#define STM32_HSI14CLK          14000000    /**< 14MHz speed internal clock.*/
#define STM32_HSI48CLK          48000000    /**< 48MHz speed internal clock.*/
#define STM32_LSICLK            40000       /**< Low speed internal clock.  */
/** @} */

/**
 * @name    PWR_CR register bits definitions
 * @{
 */
#define STM32_PLS_MASK          (7 << 5)    /**< PLS bits mask.             */
#define STM32_PLS_LEV0          (0 << 5)    /**< PVD level 0.               */
#define STM32_PLS_LEV1          (1 << 5)    /**< PVD level 1.               */
#define STM32_PLS_LEV2          (2 << 5)    /**< PVD level 2.               */
#define STM32_PLS_LEV3          (3 << 5)    /**< PVD level 3.               */
#define STM32_PLS_LEV4          (4 << 5)    /**< PVD level 4.               */
#define STM32_PLS_LEV5          (5 << 5)    /**< PVD level 5.               */
#define STM32_PLS_LEV6          (6 << 5)    /**< PVD level 6.               */
#define STM32_PLS_LEV7          (7 << 5)    /**< PVD level 7.               */
/** @} */

/**
 * @name    RCC_CFGR register bits definitions
 * @{
 */
#define STM32_SW_HSI            (0 << 0)    /**< SYSCLK source is HSI.      */
#define STM32_SW_HSE            (1 << 0)    /**< SYSCLK source is HSE.      */
#define STM32_SW_PLL            (2 << 0)    /**< SYSCLK source is PLL.      */
#define STM32_SW_HSI48          (3 << 0)    /**< SYSCLK source is HSI48.    */

#define STM32_HPRE_DIV1         (0 << 4)    /**< SYSCLK divided by 1.       */
#define STM32_HPRE_DIV2         (8 << 4)    /**< SYSCLK divided by 2.       */
#define STM32_HPRE_DIV4         (9 << 4)    /**< SYSCLK divided by 4.       */
#define STM32_HPRE_DIV8         (10 << 4)   /**< SYSCLK divided by 8.       */
#define STM32_HPRE_DIV16        (11 << 4)   /**< SYSCLK divided by 16.      */
#define STM32_HPRE_DIV64        (12 << 4)   /**< SYSCLK divided by 64.      */
#define STM32_HPRE_DIV128       (13 << 4)   /**< SYSCLK divided by 128.     */
#define STM32_HPRE_DIV256       (14 << 4)   /**< SYSCLK divided by 256.     */
#define STM32_HPRE_DIV512       (15 << 4)   /**< SYSCLK divided by 512.     */

#define STM32_PPRE_DIV1         (0 << 8)    /**< HCLK divided by 1.         */
#define STM32_PPRE_DIV2         (4 << 8)    /**< HCLK divided by 2.         */
#define STM32_PPRE_DIV4         (5 << 8)    /**< HCLK divided by 4.         */
#define STM32_PPRE_DIV8         (6 << 8)    /**< HCLK divided by 8.         */
#define STM32_PPRE_DIV16        (7 << 8)    /**< HCLK divided by 16.        */

#define STM32_PLLSRC_HSI_DIV2   (0 << 15)   /**< PLL clock source is HSI/2. */
#define STM32_PLLSRC_HSI        (1 << 15)   /**< PLL clock source is HSI    */
#define STM32_PLLSRC_HSE        (2 << 15)   /**< PLL clock source is HSE.   */
#define STM32_PLLSRC_HSI48      (3 << 15)   /**< PLL clock source is HSI48. */

#define STM32_MCOSEL_NOCLOCK    (0 << 24)   /**< No clock on MCO pin.       */
#define STM32_MCOSEL_HSI14      (1 << 24)   /**< HSI14 clock on MCO pin.    */
#define STM32_MCOSEL_LSI        (2 << 24)   /**< LSI clock on MCO pin.      */
#define STM32_MCOSEL_LSE        (3 << 24)   /**< LSE clock on MCO pin.      */
#define STM32_MCOSEL_SYSCLK     (4 << 24)   /**< SYSCLK on MCO pin.         */
#define STM32_MCOSEL_HSI        (5 << 24)   /**< HSI clock on MCO pin.      */
#define STM32_MCOSEL_HSE        (6 << 24)   /**< HSE clock on MCO pin.      */
#define STM32_MCOSEL_PLLDIV2    (7 << 24)   /**< PLL/2 clock on MCO pin.    */
#define STM32_MCOSEL_HSI48      (8 << 24)   /**< HSI48 clock on MCO pin.    */

#define STM32_MCOPRE_DIV1       (0 << 28)   /**< MCO divided by 1.          */
#define STM32_MCOPRE_DIV2       (1 << 28)   /**< MCO divided by 2.          */
#define STM32_MCOPRE_DIV4       (2 << 28)   /**< MCO divided by 4.          */
#define STM32_MCOPRE_DIV8       (3 << 28)   /**< MCO divided by 8.          */
#define STM32_MCOPRE_DIV16      (4 << 28)   /**< MCO divided by 16.         */
#define STM32_MCOPRE_DIV32      (5 << 28)   /**< MCO divided by 32.         */
#define STM32_MCOPRE_DIV64      (6 << 28)   /**< MCO divided by 64.         */
#define STM32_MCOPRE_DIV128     (7 << 28)   /**< MCO divided by 128.        */

#define STM32_PLLNODIV_MASK     (1 << 31)   /**< MCO PLL divider mask.      */
#define STM32_PLLNODIV_DIV2     (0 << 31)   /**< MCO PLL is divided by two. */
#define STM32_PLLNODIV_DIV1     (1 << 31)   /**< MCO PLL is divided by one. */
/** @} */

/**
 * @name    RCC_CFGR2 register bits definitions
 * @{
 */
#define STM32_PRE_DIV1          (0 << 0)    /**< PLLSRC divided by 1.       */
#define STM32_PRE_DIV2          (1 << 0)    /**< SYSCLK divided by 2.       */
#define STM32_PRE_DIV3          (2 << 0)    /**< SYSCLK divided by 3.       */
#define STM32_PRE_DIV4          (3 << 0)    /**< PLLSRC divided by 4.       */
#define STM32_PRE_DIV5          (4 << 0)    /**< SYSCLK divided by 5.       */
#define STM32_PRE_DIV6          (5 << 0)    /**< SYSCLK divided by 6.       */
#define STM32_PRE_DIV7          (6 << 0)    /**< PLLSRC divided by 7.       */
#define STM32_PRE_DIV8          (7 << 0)    /**< SYSCLK divided by 8.       */
#define STM32_PRE_DIV9          (8 << 0)    /**< SYSCLK divided by 9.       */
#define STM32_PRE_DIV10         (9 << 0)    /**< PLLSRC divided by 10.      */
#define STM32_PRE_DIV11         (10 << 0)   /**< SYSCLK divided by 11.      */
#define STM32_PRE_DIV12         (11 << 0)   /**< SYSCLK divided by 12.      */
#define STM32_PRE_DIV13         (12 << 0)   /**< PLLSRC divided by 13.      */
#define STM32_PRE_DIV14         (13 << 0)   /**< SYSCLK divided by 14.      */
#define STM32_PRE_DIV15         (14 << 0)   /**< SYSCLK divided by 15.      */
#define STM32_PRE_DIV16         (15 << 0)   /**< PLLSRC divided by 16.      */
/** @} */

/**
 * @name    RCC_CFGR3 register bits definitions
 * @{
 */
#define STM32_USART1SW_MASK     (3 << 0)    /**< USART1 clock source mask.  */
#define STM32_USART1SW_PCLK     (0 << 0)    /**< USART1 clock is PCLK.      */
#define STM32_USART1SW_SYSCLK   (1 << 0)    /**< USART1 clock is SYSCLK.    */
#define STM32_USART1SW_LSE      (2 << 0)    /**< USART1 clock is LSE.       */
#define STM32_USART1SW_HSI      (3 << 0)    /**< USART1 clock is HSI.       */
#define STM32_I2C1SW_MASK       (1 << 4)    /**< I2C clock source mask.     */
#define STM32_I2C1SW_HSI        (0 << 4)    /**< I2C clock is HSI.          */
#define STM32_I2C1SW_SYSCLK     (1 << 4)    /**< I2C clock is SYSCLK.       */
#define STM32_CECSW_MASK        (1 << 6)    /**< CEC clock source mask.     */
#define STM32_CECSW_HSI         (0 << 6)    /**< CEC clock is HSI/244.      */
#define STM32_CECSW_LSE         (1 << 6)    /**< CEC clock is LSE.          */
#define STM32_CECSW_OFF         0xFFFFFFFF  /**< CEC clock is not required. */
#define STM32_USBSW_MASK        (1 << 7)    /**< USB clock source mask.     */
#define STM32_USBSW_HSI48       (0 << 7)    /**< USB clock is HSI48.        */
#define STM32_USBSW_PCLK        (1 << 7)    /**< USB clock is PCLK.         */
/** @} */

/**
 * @name    RCC_BDCR register bits definitions
 * @{
 */
#define STM32_RTCSEL_MASK       (3 << 8)    /**< RTC clock source mask.     */
#define STM32_RTCSEL_NOCLOCK    (0 << 8)    /**< No clock.                  */
#define STM32_RTCSEL_LSE        (1 << 8)    /**< LSE used as RTC clock.     */
#define STM32_RTCSEL_LSI        (2 << 8)    /**< LSI used as RTC clock.     */
#define STM32_RTCSEL_HSEDIV     (3 << 8)    /**< HSE divided by 32 used as
                                                 RTC clock.                 */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables the PWR/RCC initialization in the HAL.
 */
#if !defined(STM32_NO_INIT) || defined(__DOXYGEN__)
#define STM32_NO_INIT                       FALSE
#endif

/**
 * @brief   Enables or disables the programmable voltage detector.
 */
#if !defined(STM32_PVD_ENABLE) || defined(__DOXYGEN__)
#define STM32_PVD_ENABLE                    FALSE
#endif

/**
 * @brief   Sets voltage level for programmable voltage detector.
 */
#if !defined(STM32_PLS) || defined(__DOXYGEN__)
#define STM32_PLS                           STM32_PLS_LEV0
#endif

/**
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM32_HSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI_ENABLED                   FALSE
#endif

/**
 * @brief   Enables or disables the HSI14 clock source.
 */
#if !defined(STM32_HSI14_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI14_ENABLED                 FALSE
#endif

/**
 * @brief   Enables or disables the HSI48 clock source.
 */
#if !defined(STM32_HSI48_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI48_ENABLED                 FALSE
#endif

/**
 * @brief   Enables or disables the LSI clock source.
 */
#if !defined(STM32_LSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSI_ENABLED                   FALSE
#endif

/**
 * @brief   Enables or disables the HSE clock source.
 */
#if !defined(STM32_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSE_ENABLED                   FALSE
#endif

/**
 * @brief   Enables or disables the LSE clock source.
 */
#if !defined(STM32_LSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSE_ENABLED                   FALSE
#endif

/**
 * @brief   Main clock source selection.
 * @note    If the selected clock source is not the PLL then the PLL is not
 *          initialized and started.
 * @note    The default value is calculated for a 48MHz system clock from
 *          a 8MHz crystal using the PLL.
 */
#if !defined(STM32_SW) || defined(__DOXYGEN__)
#define STM32_SW                            STM32_SW_PLL
#endif

/**
 * @brief   Clock source for the PLL.
 * @note    This setting has only effect if the PLL is selected as the
 *          system clock source.
 * @note    The default value is calculated for a 48MHz system clock from
 *          a 8MHz crystal using the PLL.
 */
#if !defined(STM32_PLLSRC) || defined(__DOXYGEN__)
#define STM32_PLLSRC                        STM32_PLLSRC_HSE
#endif

/**
 * @brief   Crystal PLL pre-divider.
 * @note    This setting has only effect if the PLL is selected as the
 *          system clock source.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 8MHz crystal using the PLL.
 */
#if !defined(STM32_PREDIV_VALUE) || defined(__DOXYGEN__)
#define STM32_PREDIV_VALUE                  1
#endif

/**
 * @brief   PLL multiplier value.
 * @note    The allowed range is 2...16.
 * @note    The default value is calculated for a 48MHz system clock from
 *          a 8MHz crystal using the PLL.
 */
#if !defined(STM32_PLLMUL_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLMUL_VALUE                  6
#endif

/**
 * @brief   AHB prescaler value.
 * @note    The default value is calculated for a 48MHz system clock from
 *          a 8MHz crystal using the PLL.
 */
#if !defined(STM32_HPRE) || defined(__DOXYGEN__)
#define STM32_HPRE                          STM32_HPRE_DIV1
#endif

/**
 * @brief   APB1 prescaler value.
 */
#if !defined(STM32_PPRE) || defined(__DOXYGEN__)
#define STM32_PPRE                          STM32_PPRE_DIV1
#endif

/**
 * @brief   MCO pin setting.
 */
#if !defined(STM32_MCOSEL) || defined(__DOXYGEN__)
#define STM32_MCOSEL                        STM32_MCOSEL_NOCLOCK
#endif

/**
 * @brief   MCO divider setting.
 */
#if !defined(STM32_MCOPRE) || defined(__DOXYGEN__)
#define STM32_MCOPRE                        STM32_MCOPRE_DIV1
#endif

/**
 * @brief   MCO PLL divider setting.
 */
#if !defined(STM32_PLLNODIV) || defined(__DOXYGEN__)
#define STM32_PLLNODIV                      STM32_PLLNODIV_DIV2
#endif

/**
 * @brief USB Clock source.
 */
#if !defined(STM32_USBSW) || defined(__DOXYGEN__)
#define STM32_USBSW                         STM32_USBSW_HSI48
#endif

/**
 * @brief   CEC clock source.
 */
#if !defined(STM32_CECSW) || defined(__DOXYGEN__)
#define STM32_CECSW                         STM32_CECSW_HSI
#endif

/**
 * @brief   I2C1 clock source.
 */
#if !defined(STM32_I2C1SW) || defined(__DOXYGEN__)
#define STM32_I2C1SW                        STM32_I2C1SW_HSI
#endif

/**
 * @brief   USART1 clock source.
 */
#if !defined(STM32_USART1SW) || defined(__DOXYGEN__)
#define STM32_USART1SW                      STM32_USART1SW_PCLK
#endif

/**
 * @brief   RTC clock source.
 */
#if !defined(STM32_RTCSEL) || defined(__DOXYGEN__)
#define STM32_RTCSEL                        STM32_RTCSEL_LSI
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(STM32F0xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32F0xx_MCUCONF not defined"
#endif

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
#include "stm32_isr.h"
#include "stm32_dma.h"
#include "stm32_exti.h"
#include "stm32_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void stm32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */
