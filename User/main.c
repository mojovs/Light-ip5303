#include "main.h"
#include "sys.h"
#include "delay.h"
#include "led_base.h"
#include "led.h"
#include "light.h"
#include "capture.h"
//#include "people_detect.h"
#include "retarget.h"
#include "uart.h"


/// PA0：读取BISS0001
/// PA1：给IP5303提供PWM，保持芯片工作
/// PA2：控制灯的亮灭
/// PA6：控制led灯
int main(void) {

    HAL_Init();
    Stm32_Clock_Init(RCC_PLL_MUL9);
    delay_init(72);
    uart_init(115200);
    RetargetInit(&uart_handle);
    printf("start\r\n");
    //tim3
    led_init();
    //led_breath_init(500 - 1, 72 - 1);
    //light_init();
    //led.led_breath(5,4);
    led.led_gradient(0,100,3);


    u32 temp=0;
    int ret=0;

    TIM2_CH1_Cap_Init();


    while (1) {
        delay_ms(10);

        if(TIM2CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
        {
            temp=TIM2CH1_CAPTURE_STA&0X3F;
            temp*=0XFFFF;		 	    //溢出时间总和
            temp+=TIM2CH1_CAPTURE_VAL;      //得到总的高电平时间
            printf("TEMP:%d\r\n",temp);
            //light_low(3);

            TIM2CH1_CAPTURE_STA=0;          //开启下一次捕获
        }

    }
    return 0;
}

