#include "uart_info.h"
#include <stdio.h>
#include <string.h>


char op_to_char(Op op) {
    switch (op) {
        case ADD:
            return '+';
        case SUB:
            return '-';
        case MUL:
            return '*';
        case DIV:
            return '/';
        default:
            return '?';
    }
}

void warning_left(int time) {
    char str[200];
    sprintf(str, "[warning] %d seconds left!\n", time);
    HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 0xffff);
}

void info_time_out() {
    char str[200];
    sprintf(str, "[INFO] Time out. Game over. Please press KEY0 to try again. \n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 0xffff);
}

void info_calc_res(int good) {
    char str[200];
    if (good) {
        sprintf(str, "[INFO] True! Do a good job! \n");
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        HAL_Delay(100);
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    } else {
        sprintf(str, "[INFO] Wrong! Please try again! \n");
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        HAL_Delay(100);
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
    }
    HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 0xffff);
}

void info_pass() {
    char str[200];
    sprintf(str, "[INFO] You pass the practice! Please press KEY0 to try again . \n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 0xffff);
}

void send_eq(Eq *eq) {
    char str[200];
    sprintf(str, "%d %c %d = ?\n", eq->num1, op_to_char(eq->op), eq->num2);
    HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 0xffff);
}

void hint_left_prob(int left) {
    char str[200];
    sprintf(str, "[HINT] You have %d problems left. \n", left);
    HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 0xffff);
}
