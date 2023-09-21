#ifndef NOTATION_H
#define NOTATION_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EPS 1e-8

struct st {
    double elem;
    struct st *next;
};

struct st *initialize();                         //инициализация
void up_to_head(struct st **head, double elem);  //засунуть
double del_node(struct st **);                   //снести 1
int stack_size(struct st *head);
void remove_all(struct st **);  // разрушить
void unary_operation(struct st *op, char input_char, int *flag);
void binary_operation(struct st **op, char input_char, int *flag);
double while_float(char *input_str, int *i, int *cnt);
int notation(char *input_str, double *result, double x);
int obxod(struct st *op, char *input_str, double *result, double x);
int is_digit(char z);
int is_binary(char s);
int is_unary(char s);

#endif
