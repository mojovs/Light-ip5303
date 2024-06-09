//
// Created by meng on 2024/6/7.
//

#include "light_control.h"
#include "delay.h"

Light light;
//普通模式
void light_init() {

    //绑定函数
    light.light_breath = light_breath;
    light.light_gradient = light_gradient;
    light.light_high = light_high;
    light.light_very_high = light_very_high;
    light.light_medium = light_medium;
    light.light_low = light_low;

    light.light_on = light_on;
    light.light_slow_start = light_slow_start;
    light.light_slow_shutdown = light_slow_shutdown;

    light.tim_initer.set_compare=set_compare;

    //初始化pwm
    light.tim_initer.handler.Instance=light.tim_initer.Instance;
    light.tim_initer.handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    light.tim_initer.handler.Init.Period=light.tim_initer.arr;
    light.tim_initer.handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    light.tim_initer.handler.Init.Prescaler=light.tim_initer.pres;
    HAL_TIM_PWM_Init(&light.tim_initer.handler);
    // PWM 模式 1––在递增计数模式下，只要 TIMx_CNT<TIMx_CCR1，通道1便为有效状态，
    // 否则为无效状态。在递减计数模式下，只要 TIMx_CNT>TIMx_CCR1，通道 1 便为
    // 无效状态 (OC1REF=0)，否则为有效状态 (OC1REF=1)。
    light.tim_initer.oc_initer.OCMode=TIM_OCMODE_PWM1;
    light.tim_initer.oc_initer.Pulse=light.tim_initer.arr/2;   //初始占空比为一半
    light.tim_initer.oc_initer.OCPolarity=TIM_OCPOLARITY_HIGH; //输出高电平
    HAL_TIM_PWM_ConfigChannel(&light.tim_initer.handler,&light.tim_initer.oc_initer,light.tim_initer.channel);

    HAL_TIM_PWM_Start(&light.tim_initer.handler,light.tim_initer.channel);
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

    u8 dir=1;   //1递增模式
    u16 led0_pwm_val=0;
    //调整占空比
    while (1) {
        delay_ms(5);
        if (dir) led0_pwm_val++;
        else led0_pwm_val--;
        if (led0_pwm_val > light.tim_initer.arr*level/8) dir = 0; //500个时钟周期，大于300个周期，就转换成递减模式
        if (led0_pwm_val == 0) dir = 1;
        light.tim_initer.set_compare(led0_pwm_val);
    }
}

void set_compare(u32 comp) {
    light.tim_initer.Instance->CCR3=comp;   //设置tim2ch3通道

}
