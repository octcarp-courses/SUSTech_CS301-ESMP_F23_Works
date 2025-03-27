#ifndef __UART_INFO_H
#define __UART_INFO_H

#include "generator.h"
#include "usart.h"


void send_eq(Eq *eq);

void warning_left(int time);

void info_time_out();

void info_calc_res(int good);

void info_pass();

void hint_left_prob(int left);

#endif
