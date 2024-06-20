//
// Created by meng on 2024/6/7.
//

#include "tim.h"
#include "delay.h"
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒 16位)
u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态
u32	TIM2CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
TIM_HandleTypeDef  tim2_handler;
TIM_OC_InitTypeDef tim2_oc_initer;

void tim2_ch1_caped();

//PA1用于保持ip5303开启状态
//一般是用7200分频，72M/7200=10kHz，1s 10k
//arr那么设为10000,即为1s
void tim2_init(u32 arr, u32 presc) {
    tim2_handler.Instance=TIM2;
    tim2_handler.Init.Prescaler=presc;
    tim2_handler.Init.Period=arr;
    tim2_handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数
    tim2_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;

    //将通道2设置为比较输出模式
    tim2_oc_initer.OCMode=TIM_OCMODE_TIMING;    //只设置为时间模式，不影响引脚电平变化
    tim2_oc_initer.Pulse=10000-1;
    HAL_TIM_OC_ConfigChannel(&tim2_handler, &tim2_oc_initer, HAL_TIM_ACTIVE_CHANNEL_2);

    HAL_TIM_OC_Init(&tim2_handler);
    HAL_TIM_OC_Start_IT(&tim2_handler,HAL_TIM_ACTIVE_CHANNEL_2);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
    {
        GPIO_InitTypeDef gpio_initer;
        __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
        gpio_initer.Pin=GPIO_PIN_1;
        gpio_initer.Speed=GPIO_SPEED_MEDIUM;
        gpio_initer.Pull=GPIO_PULLUP;   //开始为拉高
        gpio_initer.Mode=GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(GPIOA,&gpio_initer);

        HAL_NVIC_SetPriority(TIM2_IRQn,2,2);    //设置中断优先级，抢占优先级2，子优先级2
        HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断通道
        HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
        HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断

    }
}
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim) {
    //定时器2
    if (htim == &tim2_handler) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            GPIO_InitTypeDef gpio_initer;
            __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
            __HAL_RCC_GPIOA_CLK_ENABLE();            //开启GPIOA时钟
            gpio_initer.Pin = GPIO_PIN_1;
            gpio_initer.Speed = GPIO_SPEED_MEDIUM;
            gpio_initer.Pull = GPIO_PULLUP;   //开始为拉高
            gpio_initer.Mode = GPIO_MODE_OUTPUT_PP;
            HAL_GPIO_Init(GPIOA, &gpio_initer);

            HAL_NVIC_SetPriority(TIM2_IRQn, 2, 2);    //设置中断优先级，抢占优先级2，子优先级2
            HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断通道
        }
    }
}

//定时器2底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器5句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    //定时器2
    if(htim == &tim2_handler) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            GPIO_InitTypeDef GPIO_Initure;
            __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
            __HAL_RCC_GPIOA_CLK_ENABLE();            //开启GPIOA时钟

            GPIO_Initure.Pin = GPIO_PIN_0;            //PA0
            GPIO_Initure.Mode = GPIO_MODE_AF_INPUT;      //复用推挽输出
            GPIO_Initure.Pull = GPIO_NOPULL;        //不作处理
            GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //高速
            HAL_GPIO_Init(GPIOA, &GPIO_Initure);

            HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);    //设置中断优先级，抢占优先级2，子优先级0
            HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断通道
        }
    }
}


void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&tim2_handler);
}
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
    static u8 pin_state=1;  //引脚状态，初始化时为高状态
    static u16 elapsed_cnt=0;   //溢出次数
    if(htim->Instance==TIM2){
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2){
            if(pin_state==0){
                HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);   //拉低100ms电平
                //设置捕捉时间
                //1s 10000次，那么100ms就是1000次
                __HAL_TIM_SET_COMPARE(htim,HAL_TIM_ACTIVE_CHANNEL_2,1000);
                pin_state=1;
            }
            else if(pin_state==1){
                if(elapsed_cnt<20) {
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   //大部分时间处于高电平状态
                    //1s 10000次，那么100ms就是1000次
                    __HAL_TIM_SET_COMPARE(htim, HAL_TIM_ACTIVE_CHANNEL_2, 10000); //延迟1s
                    elapsed_cnt++;
                }else{
                    pin_state=0; //20s过了，跳到低电平状态
                }

            }

        }


    }

}
//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&tim2_handler)) //如果是定时器2
    {
        //用于捕捉PA0长度，捕获人体检测信号
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) //定时器
        {
            tim2_ch1_cap_elpased();
        }
    }
}
//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
    //定时器2
    if(htim == &tim2_handler) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            tim2_ch1_caped();
        }
    }

}

void tim2_ch1_caped() {
    if ((TIM2CH1_CAPTURE_STA & 0X80) == 0)//还未成功捕获
    {
        if (TIM2CH1_CAPTURE_STA & 0X40)        //上升沿之后捕获到一个下降沿
        {
            TIM2CH1_CAPTURE_STA |= 0X80;        //标记成功捕获到一次 上升 高电平 下降沿
            TIM2CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&tim2_handler, TIM_CHANNEL_1);//获取当前的捕获值.
            TIM_RESET_CAPTUREPOLARITY(&tim2_handler, TIM_CHANNEL_1);   //一定要先清除原来的设置！！
            TIM_SET_CAPTUREPOLARITY(&tim2_handler, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);//配置TIM2通道1上升沿捕获
        } else                                //还未开始,第一次捕获上升沿
        {
            TIM2CH1_CAPTURE_STA = 0;            //清空
            TIM2CH1_CAPTURE_VAL = 0;
            TIM2CH1_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
            __HAL_TIM_DISABLE(&tim2_handler);        //关闭定时器1
            __HAL_TIM_SET_COUNTER(&tim2_handler, 0);
            TIM_RESET_CAPTUREPOLARITY(&tim2_handler, TIM_CHANNEL_1);   //一定要先清除原来的设置！！
            TIM_SET_CAPTUREPOLARITY(&tim2_handler, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);//定时器2通道1设置为下降沿捕获
            __HAL_TIM_ENABLE(&tim2_handler);//使能定时器5

        }
    }
}


void tim2_ch1_cap_elpased() {
    if ((TIM2CH1_CAPTURE_STA & 0X80) == 0)//还未成功捕获
    {
        if (TIM2CH1_CAPTURE_STA & 0X40)//已经捕获到高电平了
        {
            if ((TIM2CH1_CAPTURE_STA & 0X3F) == 0X3F)//高电平太长了
            {
                TIM2CH1_CAPTURE_STA |= 0X80;        //标记成功捕获了一次
                TIM2CH1_CAPTURE_VAL = 0XFFFF;
            } else TIM2CH1_CAPTURE_STA++; //溢出次数加1
        }
    }
}

