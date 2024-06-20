//
// Created by meng on 2024/6/12.
//

#include "pwm.h"
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim){
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM2_CLK_ENABLE();			//使能定时器2
    __HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟

    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_2;           	//
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}