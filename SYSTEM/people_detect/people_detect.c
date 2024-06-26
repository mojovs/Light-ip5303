//
// Created by meng on 2024/6/7.
//

#include "people_detect.h"
#include "delay.h"
#include "led_base.h"
#include "stdio.h"

void people_detect_init() {
    //初始化引脚，设置为上升沿中断
    GPIO_InitTypeDef gpio_initer;
    gpio_initer.Pin = GPIO_PIN_0;
    gpio_initer.Speed = GPIO_SPEED_HIGH;
    gpio_initer.Mode = GPIO_MODE_IT_RISING;
    gpio_initer.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio_initer);
    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    //如果检测到biss0001信号
    delay_ms(50);  //消抖
    static u32 cnt=0;
    cnt++;
    if (GPIO_Pin == GPIO_PIN_0) {
        //初始化灯
        light.light_gradient(0,500,4);
        light.light_gradient(500,0,4);
        printf("people:%d \r\n",cnt);

    }
}