#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

struct node* init(char elem) {
    struct node* stack;
    stack = (struct node*)malloc(sizeof(struct node));
    stack->elem = elem;
    stack->next = NULL;
    return stack;
}

void push(struct node** head, char elem) {
    if (elem == 0) return;
    struct node* tmp = (struct node*)malloc(sizeof(struct node));
    if (tmp == NULL) {
        return;
    }
    tmp->next = *head;
    tmp->elem = elem;
    *head = tmp;
}

char seek(struct node* head) {
    if (head != NULL) return head->elem;
    return 0;
}

int size(struct node* head) {
    int t = 1;
    if (head->next != NULL) t += size(head->next);
    return t;
}

char pop(struct node** head) {
    struct node* out;
    char elem;
    if (*head == NULL) {
        return 0;
    }
    out = *head;
    *head = (*head)->next;
    elem = out->elem;
    free(out);
    return elem;
}

void destroy(struct node** head) {
    struct node* tmp = *head;
    while (*head != NULL) {
        *head = (*head)->next;
        free(tmp);
        tmp = *head;
    }
}
