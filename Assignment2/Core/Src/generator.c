#include "generator.h"
#include <math.h>
#include <stdlib.h>
#include "data.h"
#include "uart_info.h"


const int MAX_NUM = 100 + 1;

const int PRIME_NUM = 25;
const int prime[25] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
     31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

extern UART_HandleTypeDef huart1;

Op op_generator(MyData *d1) {
    int cur_op = rand() % 4;
    int has_op = d1->has_op;
    while ((has_op != 0b1111) && ((has_op >> cur_op) & 1)) {
        cur_op = rand() % 4;
    }
    d1->has_op |= 1 << cur_op;
    return (Op) cur_op;
}

void add_generator(Eq *eq) {
    int num1, num2;
    num1 = rand() % MAX_NUM;
    num2 = rand() % (MAX_NUM - num1);
    eq->num1 = num1;
    eq->num2 = num2;
    eq->result = eq->num1 + eq->num2;
}

void sub_generator(Eq *eq) {
    int num1, num2;
    num1 = rand() % MAX_NUM;
    num2 = rand() % MAX_NUM;
    if (num1 >= num2) {
        eq->num1 = num1;
        eq->num2 = num2;
    } else {
        eq->num1 = num2;
        eq->num2 = num1;
    }
    eq->result = eq->num1 - eq->num2;
}

void mul_generator(Eq *eq) {
    int num1, num2;
    int MAX_MUL = (int) sqrt(MAX_NUM) + 1;
    num1 = rand() % MAX_MUL;
    num2 = rand() % MAX_MUL;
    eq->num1 = num1;
    eq->num2 = num2;
    eq->result = eq->num1 * eq->num2;
}

int isPrime(int num) {
    for (int i = 0; i < PRIME_NUM; i++) {
        if (num == prime[i]) {
            return 1;
        }
    }
    return 0;
}

void div_generator(Eq *eq) {
    int num1, num2;
    num1 = rand() % MAX_NUM;
    while (isPrime(num1)) {
        num1 = rand() % MAX_NUM;
    }
    int divider[MAX_NUM];
    int divider_count = 0;
    for (int i = 2; i < num1; ++i) {
        if (num1 % i == 0) {
            divider[divider_count++] = i;
        }
    }
    int index = rand() % divider_count;
    num2 = divider[index];

    eq->num1 = num1;
    eq->num2 = num2;
    eq->result = eq->num1 / eq->num2;
}

Eq *eq_generator() {
    Eq *eq = (Eq *) malloc(sizeof(Eq));
    eq->op = op_generator(&d1);
    switch (eq->op) {
        case ADD:
            add_generator(eq);
            break;
        case SUB:
            sub_generator(eq);
            break;
        case MUL:
            mul_generator(eq);
            break;
        case DIV:
            div_generator(eq);
            break;
        default:
            break;
    }
    return eq;
}

int gen_and_send() {
    Eq *eq = eq_generator();

    send_eq(eq);

    int res = eq->result;
    free(eq);

    return res;
}
