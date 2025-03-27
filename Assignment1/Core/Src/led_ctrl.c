#include <string.h>

#include "main.h"

#include "huffman.h"
#include "led_ctrl.h"


const int DELAY_TIME = 150;

void blink_basic(int id, int times) {
    for (int i = 0; i < times; ++i) {
        HAL_Delay(10);
        switch (id) {
            case 0: {
                HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
                HAL_Delay(DELAY_TIME);
                HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
                HAL_Delay(DELAY_TIME);
                break;
            }
            case 1: {
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                HAL_Delay(DELAY_TIME);
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                HAL_Delay(DELAY_TIME);
                break;
            }
            case 2:
                HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                HAL_Delay(DELAY_TIME);
                HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                HAL_Delay(DELAY_TIME);
                break;
            default:
                break;
        }
    }
}

void blink_one_seq(const char *seq) {
    for (int i = 0; i < strlen(seq); ++i) {
        if (seq[i] == '0') {
            blink_basic(0, 1);
        } else if (seq[i] == '1') {
            blink_basic(1, 1);
        }
    }
}

void blink_seq(MyData *d) {
    d->in_p = 0;
    while (d->in_p < strlen(d->inRes)) {
        char ch = de_one_inRes(d);
        if (ch != ' ') {
            blink_one_seq(huffman_code[ch - 'a']);
            blink_basic(2, 2);
        } else {
            blink_basic(2, 3);
            break;
        }
    }
    ls1 = BL_IDLE;
}

void blink_err() {
    blink_basic(2, 3);
    ls1 = BL_IDLE;
}
