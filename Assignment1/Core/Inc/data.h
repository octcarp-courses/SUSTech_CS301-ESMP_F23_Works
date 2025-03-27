#ifndef __DATA_H
#define __DATA_H


typedef enum Mode { IDLE = 0, DE = 1, EN = 2 } Mode;

typedef enum LedSig { BL_IDLE, BL_SEQ, BL_ERR } LedSig;

typedef struct MyData {
    int empty;
    int mode;
    int in_p;
    int out_p;
    char inBuff[1024];
    char inRes[1024];
    char outBuff[1024];
    char outRes[1024];
    char in_err[1024];
} MyData;

extern MyData d1;
extern LedSig ls1;

void Data_Buff_Clear(MyData *d);

void Data_Init(MyData *d);

void Key_In(MyData *d, char input);

void Key_Confirm(MyData *d);

void UART_In(MyData *d, char *input);

#endif
