#ifndef __DATA_H
#define __DATA_H

#include "uart_info.h"

typedef enum MyMode { IDLE, START, END } MyMode;

typedef struct MyData {
    MyMode mode;
    int has_op;
    int good_answer;
    int count_down;
    int left_prob;
} MyData;

extern MyData d1;
extern MyMode m1;

void restart(MyData *d1);

void start_calc(MyData *d1);

void end_calc(MyData *d1);

void handle_result(MyData *d1, int ans);

#endif
