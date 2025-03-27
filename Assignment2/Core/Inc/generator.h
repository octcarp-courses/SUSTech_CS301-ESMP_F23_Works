#ifndef __GENERATOR_H
#define __GENERATOR_H

typedef enum Op { ADD = 0, SUB = 1, MUL = 2, DIV = 3 } Op;

typedef struct Eq {
    int num1;
    int num2;
    Op op;
    int result;
} Eq;


int gen_and_send();

#endif
