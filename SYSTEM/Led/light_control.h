//
// Created by meng on 2024/6/7.
// 大功率LED灯控制
//
//

#ifndef LIGHT_IP5303_LIGHT_CONTROL_H
#define LIGHT_IP5303_LIGHT_CONTROL_H
#include "sys.h"

//#define LIGHT_ON  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)
//#define LIGHT_OFF  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)

typedef enum _LIGHT_LEVEL{
    LIGHT_LOW,
    LIGHT_MEDIUM,
    LIGHT_HIGH,
    LIGHT_VERY_HIGH,

}LIGHT_LEVEL;

typedef struct _Light{
    GPIO_TypeDef *m_gpiox;  //gpio口
    u16 m_pin;  //引脚
    LIGHT_LEVEL light_level;    //灯光级别
    void (*light_init)(GPIO_TypeDef *gpiox, u16 pin);
    void (*light_very_high)(u32 second); //最大亮度，电压3.5V
    void (*light_high)(u32 sec);       //电压3.2V
    void (*light_medium)(u32 sec);     //电压3V
    void (*light_low)(u32 sec);        //电压2.5V
    void (*light_on)(u32 sec,LIGHT_LEVEL level);        //灯光开启
    void (*light_slow_start)(void);        //灯光渐变开启
    void (*light_slow_shutdown)(void);       //灯光渐变关闭
    void (*light_gradient)(u16 from,u16 to,u32 sec);       //灯光渐变过渡
    void (*light_breath)(u8 level);       //灯光呼吸，呼吸宽度
}Light;

extern Light light;
//初始化Light结构体
void light_init(GPIO_TypeDef *gpiox, u16 pin);
//最大亮度，电压3.5V
void light_very_high(u32 sec);

void light_high(u32 sec);       //电压3.2V
void light_medium(u32 sec);     //电压3V
void light_low(u32 sec);        //电压2.5V

void light_on(u32 sec,LIGHT_LEVEL level);        //电压2.5V
void light_slow_start(void);        //灯光渐变开启
void light_slow_shutdown(void);       //灯光渐变关闭
/**
* @brief 灯光减变过渡
* @param from 开始的比例 from/1000
* @param to 要到达的亮度 to/1000
*/
void light_gradient(u16 from, u16 to, u32 sec);

/**
* @brief 灯光呼吸
* @param level 控制呼吸宽度
*/
void light_breath(u8 level);

#endif //LIGHT_IP5303_LIGHT_CONTROL_H