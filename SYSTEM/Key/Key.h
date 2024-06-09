//
// Created by meng on 2024/5/8.
//

#ifndef SCOPE_KEY_H
#define SCOPE_KEY_H

#include "stm32f1xx_hal.h"
#include "sys.h"
//按键类型
#define KEY_ID_NONE 0
#define KEY_ID_1 1
#define KEY_ID_2 2
#define KEY_ID_3 3

//读取按键值
#define KEY_1_VAL HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)
#define KEY_2_VAL HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)
#define KEY_3_VAL HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)
//按键模式
#define PRESS_ONCE 0    //按下就是一次
#define PRESS_CONTINIOUS 1    //按下可以连续输出

void key_init();
/// 检测按键
/// \param scan_mode 0:按下一次 1:按下可以连续输出
/// \return 哪个铵键有信号
int scan_key(int scan_mode);
void key2_test();

#endif //SCOPE_KEY_H