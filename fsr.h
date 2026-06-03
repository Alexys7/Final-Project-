#ifndef FSR_H
#define FSR_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

#define FSR_THRESHOLD   200     // ADC counts below this = not pressed (0–4095)

void     FSR_init(void);
uint16_t FSR_read(void);
void     FSR_print(UART_HandleTypeDef *huart, uint16_t adc_val);

#endif /* FSR_H */
