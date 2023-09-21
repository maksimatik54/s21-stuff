#include "s21_string.h"

void s21_strncpy(char *buffer, char *target, int n) {
    for (int i = 0; i < n; i++) {
        *buffer++ = *target++;
    }
    *buffer = '\0';
}