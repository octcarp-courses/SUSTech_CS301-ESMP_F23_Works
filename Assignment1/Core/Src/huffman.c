#include <ctype.h>
#include <malloc.h>
#include <string.h>


#include "huffman.h"
#include "utils.h"

const char *huffman_code[26] = {
        "000", // A
        "101001", // B
        "00101", // C
        "11011", // D
        "011", // E
        "01000", // F
        "001001", // G
        "0011", // H
        "1100", // I
        "00100011", // J
        "0010000", // K
        "11010", // L
        "01001", // M
        "1011", // N
        "1110", // O
        "101011", // P
        "0010001001", // Q
        "0101", // R
        "1000", // S
        "1111", // T
        "10011", // U
        "101000", // V
        "101010", // W
        "001000101", // X
        "10010", // Y
        "0010001000" // Z
};

char *alphabet[26] = {
        "A", // A
        "B", // B
        "C", // C
        "D", // D
        "E", // E
        "F", // F
        "G", // G
        "H", // H
        "I", // I
        "J", // J
        "K", // K
        "L", // L
        "M", // M
        "N", // N
        "O", // O
        "P", // P
        "Q", // Q
        "R", // R
        "S", // S
        "T", // T
        "U", // U
        "V", // V
        "W", // W
        "X", // X
        "Y", // Y
        "Z" // Z
};

typedef struct Node {
    char letter;
    struct Node *l;
    struct Node *r;
} Node;

Node *root;

Node *createNode(char data) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->letter = data;
    node->l = NULL;
    node->r = NULL;
    return node;
}

void HUFFMAN_Init() {
    root = createNode(' ');
    for (int i = 0; i < 26; i++) {
        Node *cur = root;
        for (int j = 0; j < strlen(huffman_code[i]); ++j) {
            char ch = huffman_code[i][j];
            if (ch == '0') {
                if (cur->l == NULL) {
                    cur->l = createNode(' ');
                }
                cur = cur->l;
            } else if (ch == '1') {
                if (cur->r == NULL) {
                    cur->r = createNode(' ');
                }
                cur = cur->r;
            }
        }
        cur->letter = (char) ('A' + i);
    }
}

char *de_one_inRes(MyData *d) {
    Node *cur = root;

    int st = d->in_p;

    while (d->in_p < strlen(d->inRes)) {
        if (d->inRes[d->in_p] == '0') {
            cur = cur->l;
        } else if (d->inRes[d->in_p] == '1') {
            cur = cur->r;
        }
        if ((cur->l == NULL && cur->r == NULL) || d->in_p == strlen(d->inRes) - 1) {
            if (cur->letter == ' ') {
                return substring(d->inRes, st, d->in_p);
            }
            return alphabet[(int) cur->letter - 'A'];
        }
        ++(d->in_p);
    }
}

int decodeH(char *str_in, char *de_str) {
    Node *cur = root;
    int len = strlen(str_in);

    de_str[0] = '\0';
    int index = 0;
    for (int i = 0; i < len; ++i) {
        if (str_in[i] == '0') {
            cur = cur->l;
        } else if (str_in[i] == '1') {
            cur = cur->r;
        }
        if ((cur->l == NULL && cur->r == NULL) || i == len - 1) {
            if (cur->letter == ' ') {
                de_str[0] = '\0';
                return 1;
            }
            de_str[index++] = cur->letter;
            cur = root;
        }
    }
    de_str[index] = '\0';

    return 0;
}


void encodeH(char *str_in, char *en_str) {
    int len = strlen(str_in);

    en_str[0] = '\0';

    for (int i = 0; i < len; ++i) {
        char ch = str_in[i];
        if (ch == ' ') {
            continue;
        }

        int index = -1;
        if (isalpha(ch)) {
            ch = tolower(ch);
            index = ch - 'A';
        }

        if (index != -1) {
            strcat(en_str, huffman_code[index]);
        }
    }
}
