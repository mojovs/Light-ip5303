//
// Created by meng on 2024/6/7.
//

#include "light_control.h"
#include "delay.h"

Light light;

//普通模式
void light_init(GPIO_TypeDef *gpiox, u16 pin) {
    //初始化引脚，设置为上升沿中断
    GPIO_InitTypeDef gpio_initer;
    gpio_initer.Pin = pin;
    gpio_initer.Speed = GPIO_SPEED_HIGH;
    gpio_initer.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initer.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(gpiox, &gpio_initer);


    //开始绑定结构体
    light.m_gpiox = gpiox;
    light.m_pin = pin;

    light.light_breath = light_breath;
    light.light_gradient = light_gradient;
    light.light_high = light_high;
    light.light_very_high = light_very_high;
    light.light_medium = light_medium;
    light.light_low = light_low;

    light.light_on = light_on;
    light.light_slow_start = light_slow_start;
    light.light_slow_shutdown = light_slow_shutdown;
    light.light_breath = light_breath;
}

void light_very_high(u32 sec) {
    HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_SET);  //拉高
    delay_s(sec);
    HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_RESET);  //拉高

}

void light_high(u32 sec) {
    for (int i = 0; i < sec; i++) { //1s内
        for (int j = 0; j < 1000; j++) { //1ms内
            HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_SET);  //拉高
            delay_us(500);
            HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_RESET);  //拉高
            delay_us(500);
        }
    }

}

void light_medium(u32 sec) {
    for (int i = 0; i < sec; i++) { //1s内
        for (int j = 0; j < 1000; j++) { //1ms内
            HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_SET);  //拉高
            delay_us(100);
            HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_RESET);  //拉高
            delay_us(900);
        }
    }
}

void light_low(u32 sec) {
    for (int i = 0; i < sec; i++) { //1s内
        for (int j = 0; j < 1000; j++) { //1ms内
            HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_SET);  //拉高
            delay_us(50);
            HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_RESET);  //拉高
            delay_us(950);
        }
    }
}

void light_on(u32 sec, LIGHT_LEVEL level) {

}

void light_slow_start(void) {

}

void light_slow_shutdown(void) {

}

void light_gradient(u16 from, u16 to, u32 sec) {
    u8 step = 2;
    //每次亮灭加起来是1ms，所以次数是多少ms数
    u16 cnt = sec*1000;
    u16 cnt_per_delta=0;    //想要从from递加、减到to，那么每加或减1时，需要循环的次数

    //亮度增大
    if (to > from) {
        cnt_per_delta=cnt/(to-from);  //
        for (int i = 0; i < to-from;i++){
            from++;
            //
            for(int k=0;k<cnt_per_delta;k++) {
                HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_SET);  //拉高
                delay_us(from);

                HAL_GPIO_WritePin(light.m_gpiox, light.m_pin, GPIO_PIN_RESET);  //拉高
                delay_us(1000 - from);
            }
        }
    } else {
        cnt = (from - to) / step;
    }

}

void light_breath(u8 level) {

}
