#include "draw.h"

#include <math.h>
#include <stdio.h>

void output(const double *data) {
    for (int i = 0; i < ROWS; i++) {
        double y = (double)(i - 12) / (ROWS - 1) * 2;
        for (int j = 0; j < COLUMNS; j++) {
            if (data[j] <= 1 && data[j] >= -1 && data[j] - 0.04 <= y && data[j] + 0.04 >= y)
                printf("*");
            else
                printf(".");
        }
        printf("\n");
    }
}