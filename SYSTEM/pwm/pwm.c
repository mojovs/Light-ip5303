//
// Created by meng on 2024/6/7.
//

#include "pwm.h"

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef  gpioInitTypeDef;
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    //开启GPIOA 6
    gpioInitTypeDef.Pin=GPIO_PIN_6;
    gpioInitTypeDef.Mode=GPIO_MODE_AF_PP;   //复用推挽
    gpioInitTypeDef.Pull=GPIO_PULLUP;
    gpioInitTypeDef.Speed=GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA,&gpioInitTypeDef);

}