#include "data.h"
#include "generator.h"

const int MAX_TIME = 60;
const int MAX_PROB = 5;

MyData d1;
MyMode m1;

void restart(MyData *d) {
    d->mode = IDLE;
    d->count_down = MAX_TIME;
}

void start_calc(MyData *d) {
    d->mode = START;
    d->left_prob = MAX_PROB;
    d->count_down = MAX_TIME;
    d->has_op = 0;
    d->good_answer = gen_and_send();
}

void end_calc(MyData *d) {
    d->mode = END;
    d->count_down = MAX_TIME;
}

void handle_result(MyData *d, int ans) {
    if (d->good_answer != -1 && ans == d->good_answer) {
        info_calc_res(1);
        d->good_answer = -1;
        if (--d->left_prob == 0) {
            info_pass();
            end_calc(d);
            HAL_NVIC_SystemReset();
            return;
        }
        hint_left_prob(d->left_prob);
        d->good_answer = gen_and_send();
    } else {
        info_calc_res(0);
        ++d->left_prob;
        hint_left_prob(d->left_prob);
    }
}
