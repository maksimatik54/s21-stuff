#include "input.h"

#include <stdio.h>

void input(char *str) {
    int c;
    while ((c = getchar()) != '\n') {
        *str++ = c;
    }
    *str = '\0';
}