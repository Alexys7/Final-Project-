/*
 *******************************************************************************
 * @file           : fsr.c
 * @brief          : FSR pressure sensor — ADC read + UART print
 * project         : EE 329 S'26 AX
 * authors         : alexys
 * version         : 0.1
 * date            : May 27, 2026
 * compiler        : STM32CubeIDE v.1.19.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * Description: Reads an FSR via ADC1 CH5 on PA0. Prints ADC value and
 *              estimated pressure state to PuTTY over LPUART1.
 *******************************************************************************
 * GPIO Wiring
 * |   Component     | GPIO Identifier | Connector Location | Config
 *-----------------------------------------------------------------------------
 * | FSR signal      | PA0             | CN10-29            | Analog IN
 * | 10k pull-down   | PA0 to GND      |                    |
 *******************************************************************************
 */
#include "fsr.h"
#include <stdio.h>
#include <string.h>

/* ── FSR_init ─────────────────────────────────────────────────────────────
 * Configures PA0 as analog input and sets up ADC1 channel 5 for
 * single-conversion polling mode.
 */
void FSR_init(void)
{
    /* 1. Enable clocks */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;       // GPIOA clock
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;         // ADC clock

    /* 2. PA0 → Analog mode (MODER = 0b11) */
    GPIOA->MODER |= (GPIO_MODER_MODE0);         // both bits set = analog

    /* 3. ADC common clock: HCLK/1 */
    ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
    ADC123_COMMON->CCR |=  ADC_CCR_CKMODE_0;   // HCLK/1

    /* 4. Exit deep-power-down, enable internal regulator */
    ADC1->CR &= ~ADC_CR_DEEPPWD;
    ADC1->CR |=  ADC_CR_ADVREGEN;
    for (volatile uint32_t d = 0; d < 1000; d++);  // wait ~20 us for regulator

    /* 5. Calibrate ADC (single-ended) */
    ADC1->CR &= ~ADC_CR_ADCALDIF;
    ADC1->CR |=  ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);           // wait for cal to finish

    /* 6. ADC config: 12-bit, right-aligned, single conversion */
    ADC1->CFGR &= ~ADC_CFGR_RES;              // 12-bit resolution
    ADC1->CFGR &= ~ADC_CFGR_CONT;             // single conversion mode
    ADC1->CFGR &= ~ADC_CFGR_ALIGN;            // right-aligned

    /* 7. Select channel 5 (PA0), sequence length = 1 */
    ADC1->SQR1 &= ~ADC_SQR1_L;               // 1 conversion
    ADC1->SQR1 &= ~ADC_SQR1_SQ1;
    ADC1->SQR1 |=  (5u << ADC_SQR1_SQ1_Pos); // CH5 = PA0

    /* 8. Sampling time: 47.5 cycles for CH5 */
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP5;
    ADC1->SMPR1 |=  (3u << ADC_SMPR1_SMP5_Pos);

    /* 9. Enable ADC */
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));     // wait until ready
}

/* ── FSR_read ─────────────────────────────────────────────────────────────
 * Triggers a single ADC conversion and returns the 12-bit result (0–4095).
 */
uint16_t FSR_read(void)
{
    ADC1->CR |= ADC_CR_ADSTART;               // start conversion
    while (!(ADC1->ISR & ADC_ISR_EOC));       // wait for end of conversion
    return (uint16_t)(ADC1->DR & 0x0FFF);
}

/* ── FSR_print ────────────────────────────────────────────────────────────
 * Prints ADC value and pressure status to PuTTY over LPUART1.
 * Output format:  "ADC: 2048 | Pressure: PRESSED\r\n"
 *              or "ADC:  043 | Pressure: not pressed\r\n"
 */
void FSR_print(UART_HandleTypeDef *huart, uint16_t adc_val)
{
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "ADC: %4u\r\n", adc_val);
    HAL_UART_Transmit(huart, (uint8_t *)buf, len, HAL_MAX_DELAY);
}
