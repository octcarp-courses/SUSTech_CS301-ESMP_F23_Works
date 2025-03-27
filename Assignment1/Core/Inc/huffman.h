#ifndef __HUFFMAN
#define __HUFFMAN

#include "data.h"

extern const char *huffman_code[26];

void HUFFMAN_Init();

char *de_one_inRes(MyData *d);

int decodeH(char *str_in, char *de_str);

void encodeH(char *str_in, char *en_str);

#endif
