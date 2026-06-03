#ifndef MAIN_H
#define MAIN_H

#include "stm32l4xx_hal.h"
#include "fsr.h"

#define B1_Pin                      GPIO_PIN_13
#define B1_GPIO_Port                GPIOC

#define LD2_Pin                     GPIO_PIN_7
#define LD3_Pin                     GPIO_PIN_14
#define LD2_GPIO_Port               GPIOB
#define LD3_GPIO_Port               GPIOB

#define STLK_TX_Pin                 GPIO_PIN_7      // PG7 → LPUART1_TX
#define STLK_RX_Pin                 GPIO_PIN_8      // PG8 → LPUART1_RX
#define STLK_TX_GPIO_Port           GPIOG
#define STLK_RX_GPIO_Port           GPIOG

#define USB_PowerSwitchOn_Pin       GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin         GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port   GPIOG

#define USB_SOF_Pin                 GPIO_PIN_8
#define USB_SOF_GPIO_Port           GPIOA
#define USB_VBUS_Pin                GPIO_PIN_9
#define USB_VBUS_GPIO_Port          GPIOA
#define USB_ID_Pin                  GPIO_PIN_10
#define USB_ID_GPIO_Port            GPIOA
#define USB_DM_Pin                  GPIO_PIN_11
#define USB_DM_GPIO_Port            GPIOA
#define USB_DP_Pin                  GPIO_PIN_12
#define USB_DP_GPIO_Port            GPIOA

void Error_Handler(void);

#endif /* MAIN_H */
