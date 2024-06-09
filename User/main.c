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

    light_init(GPIOA, GPIO_PIN_2);
    people_detect_init();
    led_breath_init(500 - 1, 36 - 1);
    //light.light_low(3);
    light.light_gradient(0,300,20);
    while (1) {
        delay_ms(10);
        led_breath();
    }
    return 0;
}


