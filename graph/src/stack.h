#ifndef STACK_H
#define STACK_H

struct node *init(char);
void push(struct node **, char);
char seek(struct node *head);
int size(struct node *head);
char pop(struct node **);
void destroy(struct node **);

struct node {
    char elem;
    struct node *next;
};

#endif
