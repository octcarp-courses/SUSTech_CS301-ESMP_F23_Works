#ifndef __LED_CTRL
#define __LED_CTRL
#include <string.h>

#include "data.h"
#include "main.h"

#include "huffman.h"
#include "led_ctrl.h"


void blink_basic(int id, int times);

void blink_one_seq(const char *seq);

void blink_seq(MyData *d);

void blink_err();
#endif
