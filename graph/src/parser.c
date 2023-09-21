#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

#define DEFAULT 0
#define NUMBER 1
#define OPERATOR 2
#define FUNCTION 3
#define PARENTHESIS 4

int is_function(char *input) {
    int i = 0;
    while (input[i] != '\0') i++;

    for (int j = 2; j < 5; j++) {
        if (i < j) return 0;
        char check[6];
        s21_strncpy(check, input, j);
        if (!(strcmp(check, "sin") && strcmp(check, "cos") && strcmp(check, "tan") && strcmp(check, "ctg") &&
              strcmp(check, "sqrt") && strcmp(check, "ln"))) {
            return strlen(check);
        }
    }
    return 0;
}

int is_number(char input) { return input >= 48 && input <= 57; }

int is_operator(char input) {
    return input == '+' || input == '-' || input == '*' || input == '/' || input == 'l' || input == 's' ||
           input == 'c' || input == 't';
}

int compare(char *first, char second) { return priority_from_string(first) <= priority_from_stack(second); }

int priority(char op) {
    int priority = 0;
    switch (op) {
        case '(':
            priority = 0;
            break;
        case '+':
        case '-':
            priority = 1;
            break;
        case '*':
        case '/':
            priority = 2;
            break;
        case 'l':
        case 's':
        case 'c':
        case 't':
            priority = 3;
            break;
    }
    return priority;
}

int priority_from_string(char *op) {
    int p = priority(*op);
    return (p == 3) ? p * (is_function(op) && 1) : p;
}

int priority_from_stack(char op) {
    switch (op) {
        case '~':
            return 1;
        case 'i':
        case 'o':
        case 'a':
        case 't':
        case 'n':
        case 'q':
            return 3;

        default:
            return priority(op);
    }
}

char *parse_operator(char *input, const char *first_char, char **result, int *current_state,
                     struct node **stack) {
    char o = *input;
    if (*input == '-' && (*result == first_char || seek(*stack) == '(')) o = '~';
    *current_state = (is_function(input)) ? FUNCTION : OPERATOR;
    if (seek(*stack) != '\0' && compare(input, seek(*stack))) {
        while (compare(input, seek(*stack)) && seek(*stack) != '(' && seek(*stack) != '\0') {
            *(*result)++ = ' ';
            *(*result)++ = pop(stack);
        }
        char op = (is_function(input)) ? *(input + 1) : o;
        push(stack, op);
    } else {
        char op = (is_function(input)) ? *(input + 1) : o;
        push(stack, op);
    }
    int op_size;
    if ((op_size = is_function(input))) input += op_size - 1;
    return input;
}

char *parse_parenthesis(char *input, char **result, int *current_state, struct node **stack) {
    if (*input == ')') {
        *current_state = PARENTHESIS;
        while (seek(*stack) != '(' && *stack != NULL) {
            *(*result)++ = ' ';
            *(*result)++ = pop(stack);
        }
        if (*stack != NULL) {
            pop(stack);
        } else {
            return NULL;
        }

    } else if (*input == '(') {
        *current_state = PARENTHESIS;
        push(stack, '(');
    }
    return input;
}

int parse(char *result, char *input) {
    int state = DEFAULT;
    struct node *stack = init('\0');
    char *first_char = result;
    char *first_inp = input;
    while (*input != '\0') {
        if (*input == ' ') {
            input++;
            continue;
        }
        int current_state = DEFAULT;
        if (is_number(*input) || *input == 'x' || *input == '.') {
            current_state = NUMBER;
            if (first_char != result && state != NUMBER && current_state == NUMBER) *result++ = ' ';
            *result++ = *input;
        } else if (is_operator(*input)) {
            if (state == OPERATOR && !is_function(input)) {
                destroy(&stack);
                return input - first_inp;
            }
            input = parse_operator(input, first_char, &result, &current_state, &stack);
        } else if (*input == ')' || *input == '(') {
            input = parse_parenthesis(input, &result, &current_state, &stack);
            if (input == NULL) return -1;
        } else {
            destroy(&stack);
            return input - first_inp;
        }
        state = current_state;
        input++;
    }
    while (stack != NULL) {
        if (seek(stack) == '(') {
            destroy(&stack);
            return -1;
        }
        *result++ = ' ';
        *result++ = pop(&stack);
    }
    return 0;
}