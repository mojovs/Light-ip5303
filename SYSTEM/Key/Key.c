//
// Created by meng on 2024/5/8.
//

#include "Key.h"
#include "delay.h"
#include "led.h"

void key_init() {
    GPIO_InitTypeDef gpioInitTypeDef;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpioInitTypeDef.Mode = GPIO_MODE_INPUT;
    gpioInitTypeDef.Pull = GPIO_PULLUP;
    gpioInitTypeDef.Speed = GPIO_SPEED_HIGH;
    gpioInitTypeDef.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &gpioInitTypeDef);

}

int scan_key(int scan_mode) {
    //上次按键按下了没
    static u8 last_key_down = 0;
    if (scan_mode== PRESS_CONTINIOUS) last_key_down = 0;  //如果连续按下，那么上次就没有按
    //判断模式
    if (last_key_down==0&& (KEY_1_VAL == 0 || KEY_2_VAL == 0 || KEY_3_VAL == 0)) {
        delay_ms(10);
        last_key_down = 1;  //设置为按过
        if (KEY_1_VAL == 0) {
            return KEY_ID_1;
        } else if (KEY_2_VAL == 0) {
            return KEY_ID_2;
        } else if (KEY_3_VAL == 0) {
            return KEY_ID_3;
        } else {
            return KEY_ID_NONE;
        }

    }
    //一个都没按
    else if (KEY_1_VAL== 1 && KEY_2_VAL== 1 && KEY_3_VAL== 1 ) {
        last_key_down = 0;
    }
    return KEY_ID_NONE;

}

//按键2按下会检测闪绿灯
void key2_test() {
    u8 key_val=0;
    while (1)
        key_val = scan_key(PRESS_ONCE);
    if (key_val == KEY_ID_2) {
        //LED_GREEN = !LED_GREEN;
    }
    delay_ms(10);
}


