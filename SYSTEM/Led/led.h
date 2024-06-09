//
// Created by meng on 2024/5/8.
//

#ifndef SCOPE_LED_H
#define SCOPE_LED_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "sys.h"


#define LED_LIGHT PAout(2)
#define LED_FLAG PAout(6)
void led_init();

#endif //SCOPE_LED_H