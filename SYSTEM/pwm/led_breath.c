//
// Created by meng on 2024/6/7.
//

#include "led_breath.h"
#include "delay.h"
TIM_HandleTypeDef tim3ch1_handler;
TIM_OC_InitTypeDef tim3ch1_oc_initer;
u8 dir=1;   //1递增模式
u16 led0_pwm_val=0;

void led_breath_init(u16 arr, u16 presc) {
    tim3ch1_handler.Instance=TIM3;
    tim3ch1_handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    tim3ch1_handler.Init.Period=arr;
    tim3ch1_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    tim3ch1_handler.Init.Prescaler=presc;
    HAL_TIM_PWM_Init(&tim3ch1_handler);
    // PWM 模式 1––在递增计数模式下，只要 TIMx_CNT<TIMx_CCR1，通道1便为有效状态，
    // 否则为无效状态。在递减计数模式下，只要 TIMx_CNT>TIMx_CCR1，通道 1 便为
    // 无效状态 (OC1REF=0)，否则为有效状态 (OC1REF=1)。
    tim3ch1_oc_initer.OCMode=TIM_OCMODE_PWM1;
    tim3ch1_oc_initer.Pulse=arr/2;   //占空比为一半
    tim3ch1_oc_initer.OCPolarity=TIM_OCPOLARITY_LOW; //
    HAL_TIM_PWM_ConfigChannel(&tim3ch1_handler,&tim3ch1_oc_initer,TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&tim3ch1_handler,TIM_CHANNEL_1);
}
void TIM_SetTIM3Compare1(u32 compare) {
    TIM3->CCR1=compare;
}

void led_breath() {
    //通过调整比较值
        //调整占空比
        if(dir) led0_pwm_val++;
        else led0_pwm_val--;
        if(led0_pwm_val>300) dir=0; //500个时钟周期，大于300个周期，就转换成递减模式
        if(led0_pwm_val==0) dir=1;
        TIM_SetTIM3Compare1(led0_pwm_val);
}