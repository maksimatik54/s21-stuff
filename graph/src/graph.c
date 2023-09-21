#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 10000

#include "draw.h"
#include "input.h"
#include "notation.h"
#include "parser.h"

int main() {
    char *str = (char *)malloc(NMAX * sizeof(char));
    input(str);
    char result[strlen(str)];

    int status = parse(result, str);
    if (!status) {
        double value = 0;
        double y[80];
        int j = 0;
        for (double i = 0; i < 4 * M_PI; i += 4 * M_PI / 81, j++) {
            notation(result, &value, i);
            y[j] = value;
        }
        output(y);
    } else if (status == -1) {
        printf("Parenthesis error\n");
    } else {
        for (int i = 0; str[i] != '\0'; i++) {
            if (i == status)
                printf("^");
            else
                printf("~");
        }
        printf("\n");
    }
    free(str);
    return 0;
}
