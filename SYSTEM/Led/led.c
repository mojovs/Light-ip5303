//
// Created by meng on 2024/5/8.
//

#include "led.h"
void led_init() {
    GPIO_InitTypeDef gpioInitTypeDef;
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能时钟
    gpioInitTypeDef.Mode=GPIO_MODE_OUTPUT_PP;
    gpioInitTypeDef.Pin=GPIO_PIN_2;
    gpioInitTypeDef.Pull=GPIO_PULLUP;
    gpioInitTypeDef.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&gpioInitTypeDef);
}
