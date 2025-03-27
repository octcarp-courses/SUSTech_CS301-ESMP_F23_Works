
#include "scence.h"
#include <stdlib.h>
#include <string.h>
#include "delay.h"
#include "main.h"
#include "sys.h"

// #include "usart.h"
#include "key.h"
#include "lcd.h"
#include "led.h"

// #include "usmart.h"
#include "24cxx.h"
#include "24l01.h" //通信驱动 基于spi进行通信
#include "image.h"
#include "touch.h"


int sc_count = 0;
const SC_MAX = 130;
/// /////////////////////////////////////////////////////////////////////////////
int pa_x = 0, pa_y = 0;
int tar_x = 0, tar_y = 0;
int saw_w = 60, saw_h = 45;
int big_w = 180, big_h = 120;
int big_x = 60, big_y = 10;

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

int tp_in(int x1, int y1, int x2, int y2) {
    return (x1 < tp_dev.x[0] && tp_dev.x[0] < x2 && y1 < tp_dev.y[0] && tp_dev.y[0] < y2) ? 1 : 0;
}

int confirm_btn_x = 20, confirm_btn_y = 200;
int refresh_btn_x = 20, refresh_btn_y = 160;
int btn_w = 60, btn_h = 30;

void draw_confirm_btn() {
    LCD_DrawRectangle(confirm_btn_x, confirm_btn_y, confirm_btn_x + btn_w, confirm_btn_y + btn_h);
    LCD_ShowString(confirm_btn_x + 5, confirm_btn_y + 5, 100, 12, 12, "Confirm");
}

void draw_refresh_btn() {
    LCD_DrawRectangle(refresh_btn_x, refresh_btn_y, refresh_btn_x + btn_w, refresh_btn_y + btn_h);
    LCD_ShowString(refresh_btn_x + 5, refresh_btn_y + 5, 100, 12, 12, "Refresh");
}


void rand_miss() {
    srand(SysTick->VAL);
    pa_x = rand() % (big_w - saw_w - 1);
    pa_y = rand() % (big_h - saw_h - 1);
    tar_x = pa_x + big_x;
    tar_y = pa_y + big_y;
}

void show_big_miss() {
    LCD_ShowBigMiss(big_x, big_y, big_w, big_h, pa_x, pa_y, saw_w, saw_h, (unsigned short *) big_pic);
}

void show_big_part(int x, int y) {
    LCD_ShowPic_Part(big_w, big_h, x, y, pa_x, pa_y, saw_w, saw_h, (unsigned short *) big_pic);
}

int pic_it() {
    LCD_Clear(WHITE);
    rand_miss();
    show_big_miss();
    u8 key;
    u8 i = 0;
    int saw_x = 10, saw_y = tar_y;
    show_big_part(saw_x, tar_y);
    draw_confirm_btn();
    sc_count = 70;
    while (1) {
        if (sc_count <= 0) {
            LCD_ShowString(10, 180, 100, 12, 12, "Time up!");
            return 2;
        }
        key = KEY_Scan(0);
        tp_dev.scan(0);

        if (tp_dev.sta & TP_PRES_DOWN) // 触摸屏被按下
        {
            if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height) {
                if (tp_in(saw_x, saw_y, saw_x + 45, saw_y + 30)) {
                    if (saw_x < big_x) {
                        LCD_Fill(saw_x, saw_y, min(saw_x + saw_w, big_x), saw_y + saw_h, WHITE);
                    } else if (saw_x + saw_w > big_x + big_w) {
                        LCD_Fill(max(saw_x, big_x + big_w), saw_y, saw_x + saw_w, saw_y + saw_h, WHITE);
                    }
                    show_big_miss();
                    saw_x = tp_dev.x[0] - (saw_w >> 1);
                    saw_x = max(saw_x, 0);
                    saw_x = min(saw_x, lcddev.width - saw_w);
                    show_big_part(saw_x, saw_y);
                } else if (tp_in(confirm_btn_x, confirm_btn_y, confirm_btn_x + btn_w, confirm_btn_y + btn_h)) {
                    int diff = abs(tar_x - saw_x);
                    if (diff < big_w / 10 && diff > 2) {
                        if (sc_count > SC_MAX - 50) {
                            return 3;
                        } else {
                            screen_norm_print();
                            LCD_ShowPicture(big_x, big_y, big_w, big_h, (unsigned short *) big_pic);
                            LCD_ShowString(10, 180, 100, 12, 12, "success!");
                            return 1;
                        }
                    } else if (diff <= 2) {
                        return 2;
                    } else {
                        return 0;
                    }
                }
            }
        } else
            delay_ms(10); // 没有按键按下的时�?
        delay_ms(100);
        --sc_count;
        if (key == KEY0_PRES) // KEY0按下,则执行校准程�?
        {
            LCD_Clear(WHITE); // 清屏
            TP_Adjust(); // 屏幕校准
            TP_Save_Adjdata();
            Load_Drow_Dialog();
        }
    }
}

unsigned short *choice_pic(char ch, int id) {
    switch (ch) {
        case 'a':
            return a_pics[id];
        case 'b':
            return b_pics[id];
        case 'c':
        case 'C':
            return c_pics[id];
        case 'A':
            return A_pics[id];
        case 'B':
            return B_pics[id];
        case '0':
            return zero_pics[id];
        case '1':
            return one_pics[id];
        case '2':
            return two_pics[id];
        case '3':
            return three_pics[id];
        default:
            break;
    }
}

char codes[4];
int order[4];
int clicked[4];
int click_cnt;

void gerate_verify_code() {
    for (size_t i = 0; i < 4; i++) {
        clicked[i] = 0;
    }
    click_cnt = 0;

    srand(SysTick->VAL);
    char tempCodeArr[4];

    int is_big = rand() % 2;
    tempCodeArr[0] = rand() % 3 + (is_big ? 'a' : 'A');
    tempCodeArr[1] = rand() % 4 + '0';
    tempCodeArr[2] = rand() % 23 + (is_big ? 'D' : 'd');
    tempCodeArr[3] = rand() % 6 + '4';

    for (int i = 0; i < 4; ++i) {
        int randomIndex = rand() % 4;
        char temp = tempCodeArr[i];
        tempCodeArr[i] = tempCodeArr[randomIndex];
        tempCodeArr[randomIndex] = temp;
    }

    for (int i = 0; i < 4; ++i) {
        codes[i] = tempCodeArr[i];
    }
    int tempOrderArr[4] = {0, 1, 2, 3};
    // 打乱数组顺序
    for (int i = 3; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = tempOrderArr[i];
        tempOrderArr[i] = tempOrderArr[j];
        tempOrderArr[j] = temp;
    }
    for (int i = 0; i < 4; ++i) {
        order[i] = tempOrderArr[i];
    }

    char fin_order[4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (order[j] == i) {
                fin_order[i] = codes[j];
            }
        }
    }
    for (int i = 0; i < 4; ++i) {
        char codes_show[200];
        sprintf(codes_show, "Click %c,%c,%c and %c in turn.", fin_order[0], fin_order[1], fin_order[2], fin_order[3]);
        LCD_ShowString(100, 140, 200, 12, 12, codes_show);
    }
    // LCD_ShowPicture(20, 20, 100, 100, choice_pic(ch1, rand() % 2));
    // LCD_ShowPicture(20, 120, 100, 100, choice_pic(ch2, rand() % 2));
    // LCD_ShowPicture(20, 220, 100, 100, choice_pic(ch3, rand() % 2));
}

int veri_x = 10, veri_y = 10;
int veri_w = 280, veri_h = 100;
int code_x[4], code_y[4];
int region_sz = 25;


void draw_codes() {
    LCD_DrawRectangle(veri_x, veri_y, veri_x + veri_w, veri_y + veri_h);
    for (int i = 0; i < 4; ++i) {
        code_x[i] = veri_x + 5 + i * 70;
        code_y[i] = veri_y + 5 + rand() % 40;
        char ch = codes[i];
        if (isalpha(ch)) {
            char lower = tolower(ch);
            if (lower == 'a' || lower == 'b' || lower == 'c') {
                LCD_ShowPicture(code_x[i], code_y[i], region_sz, region_sz, choice_pic(ch, rand() % 2));
            } else {
                LCD_ShowChar(code_x[i] + (region_sz >> 1), code_y[i], ch, 24, 0);
            }
        } else if (ch == '0' || ch == '1' || ch == '2' || ch == '3') {
            LCD_ShowPicture(code_x[i], code_y[i], region_sz, region_sz, choice_pic(ch, rand() % 2));
        } else {
            LCD_ShowChar(code_x[i] + (region_sz >> 1), code_y[i], ch, 24, 0);
        }
    }
}

void draw_clicked() {
    char temp_click[4 + 1];
    for (size_t i = 1; i <= click_cnt; i++) {
        for (size_t j = 0; j < 4; j++) {
            if (clicked[j] == i) {
                temp_click[i] = codes[j];
            }
        }
    }

    char str[200] = "You have licked: ";
    for (size_t i = 1; i <= click_cnt; i++) {
        char temp_str[10];
        sprintf(temp_str, "%c ", temp_click[i]);
        strcat(str, temp_str);
    }
    LCD_Fill(100, 160, 320, 180, WHITE);
    LCD_ShowString(100, 160, 200, 12, 12, str);
}
typedef struct points {
    int x;
    int y;
} points;

#define POINT_MAX (60)
#define LINE_MAX (4 << 1)
points mask_points[POINT_MAX];
points line_points[LINE_MAX];
points polyline_points[LINE_MAX];

void generate_draw_mask() {
    int temp_color = POINT_COLOR;
    srand(SysTick->VAL);
    for (int i = 0; i < POINT_MAX; i++) {
        mask_points[i].x = (rand() % veri_w) + veri_x;
        mask_points[i].y = (rand() % veri_h) + veri_y;
        TP_Draw_Big_Point(mask_points[i].x, mask_points[i].y, BLACK);
    }
    for (int i = 0; i < (LINE_MAX); i += 2) {
        line_points[i].x = (rand() % veri_w) + veri_x;
        line_points[i].y = (rand() % veri_h) + veri_y;
        line_points[i + 1].x = (rand() % veri_w) + veri_x;
        line_points[i + 1].y = (rand() % veri_h) + veri_y;
        LCD_DrawLine(line_points[i].x, line_points[i].y, line_points[i + 1].x, line_points[i + 1].y);
    }
    int poly_y1 = rand() % veri_h + veri_y;
    int poly_y2 = rand() % veri_h + veri_y;
    if (poly_y1 > poly_y2) {
        int temp = poly_y1;
        poly_y1 = poly_y2;
        poly_y2 = temp;
    }
    int x_start = rand() % 20 + veri_x;
    int x_end = x_start + (LINE_MAX - 1) * 20;
    for (int i = 0; i < (LINE_MAX); ++i) {
        polyline_points[i].x = x_start + ((x_end - x_start) / LINE_MAX) * i;
        polyline_points[i].y = (i & 1) ? poly_y1 : poly_y2;
        if (i > 0) {
            LCD_DrawLine(polyline_points[i - 1].x, polyline_points[i - 1].y, polyline_points[i].x,
                         polyline_points[i].y);
        }
    }
    POINT_COLOR = temp_color;
}

void reset_draw_code() {
    LCD_Clear(WHITE);
    gerate_verify_code();
    draw_codes();
    generate_draw_mask();
    draw_refresh_btn();
    draw_confirm_btn();
    draw_clicked();
    LCD_ShowString(200, 280, 100, 12, 12, "SC2");
}

int word_it() {

    u8 key;
    u8 i = 0;
    reset_draw_code();
    while (1) {
        key = KEY_Scan(0);
        tp_dev.scan(0);

        if (tp_dev.sta & TP_PRES_DOWN) { // 触摸屏被按下
            if (tp_in(refresh_btn_x, refresh_btn_y, refresh_btn_x + btn_w, refresh_btn_y + btn_h)) {
                reset_draw_code();
            } else if (tp_in(confirm_btn_x, confirm_btn_y, confirm_btn_x + btn_w, confirm_btn_y + btn_h)) {
                int pass = 1;
                if (click_cnt >= 4) {
                    for (size_t i = 0; i < click_cnt; ++i) {
                        if (clicked[i] != order[i] + 1) {
                            LCD_ShowString(100, 180, 200, 12, 12, "Wrong Order!");
                            pass = 0;
                            break;
                        }
                    }
                    if (pass) {
                        LCD_ShowString(100, 180, 200, 12, 12, "Succeses!");
                        return 1;
                    }
                } else {
                    LCD_ShowString(100, 180, 200, 12, 12, "Click all the codes!");
                }
            } else {
                for (int i = 0; i < 4; ++i) {
                    if (tp_in(code_x[i], code_y[i], code_x[i] + region_sz, code_y[i] + region_sz)) {
                        if (clicked[i] == 0) {
                            clicked[i] = ++click_cnt;
                        } else {
                            if (clicked[i] == click_cnt) {
                                clicked[i] = 0;
                                click_cnt--;
                            }
                        }
                        draw_clicked();
                    }
                }

                delay_ms(100);
            }
        } else
            delay_ms(10); // 没有按键按下的时�?
        delay_ms(100);
    }
}

typedef enum { IDLE, SC1, SC2, FIN } mode_t;


// void main_logic(){
// 	u8 key;
// 	u8 i=0;
// 	mode_t mode = IDLE;
// 	while(mode!=FIN){
// 		LCD_Clear(WHITE);
// 		LCD_ShowString(10, 180, 200, 12, 12, "Press key0 to start");
// 		while (mode == IDLE) {
// 			key=KEY_Scan(0);
// 			delay_ms(10);
// 			if(key==KEY0_PRES)	{
// 				mode = SC1;
// 			}
// 		}

// 		int res1 = pic_it();
// 		if(res1 != 1){
// 			mode = IDLE;
// 			continue;
// 		} else if(res1==1){
// 			mode = SC2;
// 		}
//		if (mode == SC2) {
//			int res2 = word_it();
//			if (res2==1){
//				mode=FIN;
//			}
//		}

//		LCD_ShowString(100, 200, 200, 12, 12, "You pass all test!");
// }

void main_logic() {
    u8 key;
    u8 i = 0;
    mode_t mode = IDLE;
    while (mode != FIN) {
        LCD_Clear(WHITE);
        LCD_ShowString(10, 180, 200, 12, 12, "Press key0 to start");
        while (mode == IDLE) {
            key = KEY_Scan(0);
            delay_ms(10);
            if (key == KEY0_PRES) {
                mode = SC1;
            }
        }

        int res1 = pic_it();
        if (res1 != 1) {
            mode = IDLE;
            continue;
        } else {
            mode = SC2;
        }

        if (mode == SC2) {
            int res2 = word_it();
            if (res2 == 1) {
                mode = FIN;
            }
        }
    }

    LCD_ShowString(100, 200, 200, 12, 12, "You pass all test!");
}
