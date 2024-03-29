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
 * @file    STM32F0xx/hal_lld.c
 * @brief   STM32F0xx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

/**
 * @brief   Initializes the backup domain.
 * @note    WARNING! Changing clock source impossible without resetting
 *          of the whole BKP domain.
 */
//static void hal_lld_backup_domain_init(void) {
//
//  /* Backup domain access enabled and left open.*/
//  PWR->CR |= PWR_CR_DBP;
//
//  /* Reset BKP domain if different clock source selected.*/
//  if ((RCC->BDCR & STM32_RTCSEL_MASK) != STM32_RTCSEL){
//    /* Backup domain reset.*/
//    RCC->BDCR = RCC_BDCR_BDRST;
//    RCC->BDCR = 0;
//  }
//
//  /* If enabled then the LSE is started.*/
//#if STM32_LSE_ENABLED
//#if defined(STM32_LSE_BYPASS)
//  /* LSE Bypass.*/
//  RCC->BDCR |= STM32_LSEDRV | RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
//#else
//  /* No LSE Bypass.*/
//  RCC->BDCR |= STM32_LSEDRV | RCC_BDCR_LSEON;
//#endif
//  while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0)
//    ;                                     /* Waits until LSE is stable.   */
//#endif
//
//#if STM32_RTCSEL != STM32_RTCSEL_NOCLOCK
//  /* If the backup domain hasn't been initialized yet then proceed with
//     initialization.*/
//  if ((RCC->BDCR & RCC_BDCR_RTCEN) == 0) {
//    /* Selects clock source.*/
//    RCC->BDCR |= STM32_RTCSEL;
//
//    /* RTC clock enabled.*/
//    RCC->BDCR |= RCC_BDCR_RTCEN;
//  }
//#endif /* STM32_RTCSEL != STM32_RTCSEL_NOCLOCK */
//}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(STM32_DMA_REQUIRED) || defined(__DOXYGEN__)
#if defined(STM32_DMA1_CH23_HANDLER) || defined(__DOXYGEN__)
/**
 * @brief   DMA1 streams 2 and 3 shared ISR.
 * @note    It is declared here because this device has a non-standard
 *          DMA shared IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH23_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Check on channel 2.*/
  dmaServeInterrupt(STM32_DMA1_STREAM2);

  /* Check on channel 3.*/
  dmaServeInterrupt(STM32_DMA1_STREAM3);

  OSAL_IRQ_EPILOGUE();
}
#endif /* defined(STM32_DMA1_CH23_HANDLER) */

#if defined(STM32_DMA1_CH4567_HANDLER) || defined(__DOXYGEN__)
/**
 * @brief   DMA1 streams 4, 5, 6 and 7 shared ISR.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH4567_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Check on channel 4.*/
  dmaServeInterrupt(STM32_DMA1_STREAM4);

  /* Check on channel 5.*/
  dmaServeInterrupt(STM32_DMA1_STREAM5);

#if STM32_DMA1_NUM_CHANNELS > 5
  /* Check on channel 6.*/
  dmaServeInterrupt(STM32_DMA1_STREAM6);
#endif

#if STM32_DMA1_NUM_CHANNELS > 6
  /* Check on channel 7.*/
  dmaServeInterrupt(STM32_DMA1_STREAM7);
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif /* defined(STM32_DMA1_CH4567_HANDLER) */

#if defined(STM32_DMA12_CH23_CH12_HANDLER) || defined(__DOXYGEN__)
/**
 * @brief   DMA1 streams 2 and 3, DMA2 streams 1 and 1 shared ISR.
 * @note    It is declared here because this device has a non-standard
 *          DMA shared IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA12_CH23_CH12_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Check on channel 2 of DMA1.*/
  dmaServeInterrupt(STM32_DMA1_STREAM2);

  /* Check on channel 3 of DMA1.*/
  dmaServeInterrupt(STM32_DMA1_STREAM3);

  /* Check on channel 1 of DMA2.*/
  dmaServeInterrupt(STM32_DMA2_STREAM1);

  /* Check on channel 2 of DMA2.*/
  dmaServeInterrupt(STM32_DMA2_STREAM2);

  OSAL_IRQ_EPILOGUE();
}
#endif /* defined(STM32_DMA12_CH23_CH12_HANDLER) */

#if defined(STM32_DMA12_CH4567_CH345_HANDLER) || defined(__DOXYGEN__)
/**
 * @brief   DMA1 streams 4, 5, 6 and 7, DMA2 streams 3, 4 and 5 shared ISR.
 * @note    It is declared here because this device has a non-standard
 *          DMA shared IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA12_CH4567_CH345_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Check on channel 4 of DMA1.*/
  dmaServeInterrupt(STM32_DMA1_STREAM4);

  /* Check on channel 5 of DMA1.*/
  dmaServeInterrupt(STM32_DMA1_STREAM5);

  /* Check on channel 6 of DMA1.*/
  dmaServeInterrupt(STM32_DMA1_STREAM6);

  /* Check on channel 7 of DMA1.*/
  dmaServeInterrupt(STM32_DMA1_STREAM7);

  /* Check on channel 3 of DMA2.*/
  dmaServeInterrupt(STM32_DMA2_STREAM3);

  /* Check on channel 4 of DMA2.*/
  dmaServeInterrupt(STM32_DMA2_STREAM4);

  /* Check on channel 5 of DMA2.*/
  dmaServeInterrupt(STM32_DMA2_STREAM5);

  OSAL_IRQ_EPILOGUE();
}
#endif /* defined(STM32_DMA12_CH4567_CH345_HANDLER) */
#endif /* defined(STM32_DMA_REQUIRED) */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* Reset of all peripherals.
     Note, GPIOs are not reset because initialized before this point in
     board files.*/
  rccResetAHB(~STM32_GPIO_EN_MASK);
  rccResetAPB1(0xFFFFFFFF);
  rccResetAPB2(~RCC_APB2RSTR_DBGMCURST);

  /* PWR clock enabled.*/
  rccEnablePWRInterface(true);

  /* Initializes the backup domain.*/
//  hal_lld_backup_domain_init();

  /* DMA subsystems initialization.*/
#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif

  /* IRQ subsystem initialization.*/
  irqInit();

  /* Programmable voltage detector enable.*/
#if STM32_PVD_ENABLE
  PWR->CR |= PWR_CR_PVDE | (STM32_PLS & STM32_PLS_MASK);
#endif /* STM32_PVD_ENABLE */
}
