#ifndef PARSER_H
#define PARSER_H

#include "stack.h"

char *parse_operator(char *input, const char *first_char, char **result, int *current_state,
                     struct node **stack);
char *parse_parenthesis(char *input, char **result, int *current_state, struct node **stack);
int parse(char *result, char *input);

int is_function(char *input);
int is_number(char input);
int is_operator(char input);
int compare(char *first, char second);
int priority(char op);
int priority_from_string(char *op);
int priority_from_stack(char op);

#endif