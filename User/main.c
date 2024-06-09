#include "main.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"
#include "led_breath.h"
#include "light_control.h"
#include "people_detect.h"

/// PA0：读取BISS0001
/// PA1：给IP5303提供PWM，保持芯片工作
/// PA2：控制灯的亮灭
/// PA6：控制led灯
int main(void) {
    HAL_Init();
    Stm32_Clock_Init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    //72M/72=1M，1M/500=2000Hz  0.5ms一周期
    led_breath_init(500 - 1, 72 - 1);
    people_detect_init();
    light.tim_initer.Instance=TIM2;
    light.tim_initer.channel=TIM_CHANNEL_3;
    light.m_gpiox=GPIOA;
    light.m_pin=GPIO_PIN_2;

    //72M/72=1M，1M/1000=1000Hz  1ms一周期
    light.tim_initer.pres=720-1;
    light.tim_initer.arr=1000-1;
    light_init();
    //light.light_low(3);
    light.light_breath(7);
    while (1) {
        delay_ms(10);
        led_breath();
    }
    return 0;
}


