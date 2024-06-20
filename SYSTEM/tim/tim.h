//
// Created by meng on 2024/6/7.
//

#ifndef LIGHT_IP5303_TIM_H
#define LIGHT_IP5303_TIM_H
#include "sys.h"
extern TIM_HandleTypeDef  tim2_handler;
extern TIM_OC_InitTypeDef tim2_oc_initer;
extern u8 is_people_detected;   //有没有检测到人
extern u8  TIM2CH1_CAPTURE_STA;	//输入捕获状态
extern u32	TIM2CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
void tim2_init(u32 arr, u32 presc);

/**
* @brief  捕获PA0引脚的定时器溢出中断
* @return
*/
void tim2_ch1_cap_elpased();

#endif //LIGHT_IP5303_TIM_H