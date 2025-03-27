#include "display.h"
#include <string.h>
#include "data.h"
#include "lcd.h"
#include "pic.h"
#include "utils.h"


#define max(a, b) ((a) >= (b) ? (a) : (b));
#define min(a, b) ((a) <= (b) ? (a) : (b));

const int XF = 240;
const int YF = 320;

const int CI_L = 30;
const int CI_U = 10;
const int PL_L = CI_L;
const int PL_U = CI_U + (YF / 5) * 3;

const int CI_CODE_L = CI_L;
const int CI_CODE_U = CI_U + 30;

const int PL_CODE_L = PL_L;
const int PL_CODE_U = PL_U + 30;

const int CODE_SZ = 12;

const int ONE_LINE = 30;
const int CI_MAX_LINE = 6;
const int PL_MAX_LINE = 3;

const int CI_ERR_L = CI_L;
const int CI_ERR_U = CI_CODE_L + CODE_SZ * CI_MAX_LINE + 5;

const int PL_ERR_L = PL_L;
const int PL_ERR_U = PL_CODE_L + CODE_SZ * PL_MAX_LINE + 5;

const int CI_PIC_L = CI_L;
const int CI_PIC_U = CI_ERR_U + 30;

const int PL_PIC_L = PL_L;
const int PL_PIC_U = PL_ERR_U + 20;

const int PIC_P_WID = 94;
const int PIC_P_HEI = 40;

const int PIC_F_WID = 98;
const int PIC_F_HEI = 40;

void Draw_Mode(MyData *d) {
    if (d->mode == DE) {
        LCD_DrawRectangle(CI_L - 5, CI_U - 5, CI_L + 160 + 5, CI_U + 16 + 5);
    } else if (d->mode == EN) {
        LCD_DrawRectangle(PL_L - 5, PL_U - 5, PL_L + 160 + 5, PL_U + 16 + 5);
    }
}

void Draw_Frame(MyData *d) {
    LCD_Clear(WHITE);
    Draw_Mode(d);
    LCD_ShowString(CI_L, CI_U, 160, 16, 16, (uint8_t *) "Cipher Code");
    LCD_ShowString(PL_L, PL_U, 160, 16, 16, (uint8_t *) "Plain Code");
}

void Show_Code(char *str_in, int pos) {
    int max_line, up_pos, left_pos;
    if (pos == 2) {
        max_line = PL_MAX_LINE;
        up_pos = PL_CODE_U;
        left_pos = PL_CODE_L;
    } else {
        max_line = CI_MAX_LINE;
        up_pos = CI_CODE_U;
        left_pos = CI_CODE_L;
    }

    int len = strlen(str_in);
    len = min(len, max_line * ONE_LINE);

    int line_n = (len - 1) / ONE_LINE;
    line_n = max(line_n, 0);

    for (int i = 0; i <= line_n; ++i) {
        char *sub = substring(str_in, i * ONE_LINE, i < line_n ? ((i + 1) * ONE_LINE - 1) : len - 1);
        LCD_ShowString(left_pos, up_pos + i * (CODE_SZ + 4), CODE_SZ / 2 * ONE_LINE + 10, CODE_SZ, CODE_SZ,
                       (uint8_t *) sub);
        free(sub);
    }
}

void Show_Code_In(MyData *d) {
    if (d->mode == DE) {
        Show_Code(d->inBuff, 1);
    } else if (d->mode == EN) {
        Show_Code(d->inBuff, 2);
    }
}

void Show_Res_Out(MyData *d) {
    if (d->mode == DE) {
        Show_Code(d->outRes, 2);
    } else if (d->mode == EN) {
        Show_Code(d->outRes, 1);
    }
}

void Show_Pic_Pass(MyData *d) {
    if (d->mode == DE) {
        LCD_ShowPic(CI_PIC_L, CI_PIC_U, PIC_P_WID, PIC_P_HEI, (uint8_t *) pic_pass);
    } else if (d->mode == EN) {
        LCD_ShowPic(CI_PIC_L, CI_PIC_U, PIC_P_WID, PIC_P_HEI, (uint8_t *) pic_pass);
    }
}

void Show_Pic_Fail(MyData *d) {
    if (d->mode == DE) {
        LCD_ShowPic(CI_PIC_L, CI_PIC_U, PIC_F_WID, PIC_F_HEI, (uint8_t *) pic_fail);
    } else if (d->mode == EN) {
        LCD_ShowPic(CI_PIC_L, CI_PIC_U, PIC_F_WID, PIC_F_HEI, (uint8_t *) pic_fail);
    }
}

void Show_Err(MyData *d) {
    if (d->mode == DE) {
        char *err_str = "Decode Err: ";
        strcat(err_str, d->in_err);
        Show_Code(err_str, 2);
    } else if (d->mode == EN) {
        char *err_str = "Encode Err: ";
        strcat(err_str, d->in_err);
        Show_Code(err_str, 2);
    }
}
