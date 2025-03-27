#include "utils.h"

char *substring(char *source, int start, int end) {
    int length = end - start + 1;
    char *result = malloc(length + 1);
    strncpy(result, source + start, length);
    result[length] = '\0';
    return result;
}
