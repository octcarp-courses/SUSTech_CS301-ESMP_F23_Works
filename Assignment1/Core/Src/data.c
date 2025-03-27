#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#include "data.h"
#include "display.h"
#include "huffman.h"
#include "led_ctrl.h"


MyData d1;
LedSig ls1;

void Data_Buff_Clear(MyData *d) {
    d->empty = 1;
    d->in_p = 0;
    d->out_p = 0;
    memset(d->inBuff, 0, sizeof(d->inBuff));
    memset(d->outBuff, 0, sizeof(d->outBuff));
    memset(d->inRes, 0, sizeof(d->inRes));
    memset(d->outRes, 0, sizeof(d->outRes));
    memset(d->in_err, 0, sizeof(d->in_err));
}


void Data_Init(MyData *d) {
    d->mode = DE;
    Data_Buff_Clear(d);
}


void Key_In(MyData *d, char input) {
    if (d->mode == EN) {
        return;
    }

    d->empty = 0;
    Draw_Frame(d);

    switch (input) {
        case '0': {
            strcat(d->inBuff, "0");
            break;
        }
        case '1': {
            strcat(d->inBuff, "1");
            break;
        }
        default: {
            strcpy(d->in_err, "Unexpected key input.");
            break;
        }
    }

    Show_Code_In(d);
}

void UART_In(MyData *d, char *input) {
    d->empty = 0;
    Draw_Frame(d);

    strcpy(d->inBuff, input);

    Show_Code_In(d);
}

void Switch_Mode(MyData *d) {
    if (d->mode == DE) {
        d->mode = EN;
    } else if (d->mode == EN) {
        d->mode = DE;
    } else if (d->mode == IDLE) {
        d->mode = DE;
    }
}

void Key_Confirm(MyData *d) {
    if (d->empty) {
        Switch_Mode(d);
        Data_Buff_Clear(d);
        Draw_Frame(d);
    } else {
        d->empty = 1;
        strcpy(d->inRes, d->inBuff);
        if (d->mode == DE) {
            d->in_p = 0;
            while (d->in_p < strlen(d->inRes)) {
                char *ch = de_one_inRes(d);
                ++(d->in_p);
                if (strlen(ch) == 1 && isalpha((int) ch[0])) {
                    strcat(d->outRes, ch);
                    Show_Res_Out(d);
                    blink_one_seq(huffman_code[(int) ch[0] - 'A']);
                    blink_basic(2, 2);
                } else {
                    strcat(d->outRes, " De Err: ");
                    strcat(d->outRes, ch);
                    strcat(d->outRes, " ");
                    free(ch);
                    Show_Res_Out(d);
                    Show_Pic_Fail(d);
                    blink_err();

                    d->outRes[0] = '\0';
                    return;
                }
            }
        } else if (d->mode == EN) {
            int len = strlen(d->inRes);
            d->outRes[0] = '\0';
            for (int i = 0; i < len; ++i) {
                char ch = d->inRes[i];
                if (ch == ' ' || ch == '\n') {
                    continue;
                }

                int index = 27;
                if (isalpha(ch)) {
                    ch = toupper(ch);
                    index = ch - 'A';
                }

                if (index != 27) {
                    strcat(d->outRes, huffman_code[index]);
                    Show_Res_Out(d);
                    blink_one_seq(huffman_code[index]);
                    blink_basic(2, 2);
                } else {
                    strcat(d->outRes, " En Err:");
                    char st[1] = {ch};
                    strcat(d->outRes, st);
                    strcat(d->outRes, " ");
                    Show_Res_Out(d);
                    blink_err();
                }
            }
        }
        Show_Pic_Pass(d);
        Show_Res_Out(d);
        d->outRes[0] = '\0';
    }
}
