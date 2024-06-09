//
// Created by meng on 2024/6/7.
//

#ifndef LIGHT_IP5303_LED_BREATH_H
#define LIGHT_IP5303_LED_BREATH_H
#include "sys.h"
extern  TIM_HandleTypeDef tim3ch1_handler;
extern  TIM_OC_InitTypeDef tim3ch1_oc_initer;

//初始化pwm
void led_breath_init(u16 arr, u16 presc);
/// 设置占空比
void TIM_SetTIM3Compare1(u32 compare);
/// led开始呼吸
void led_breath();

#endif //LIGHT_IP5303_LED_BREATH_H