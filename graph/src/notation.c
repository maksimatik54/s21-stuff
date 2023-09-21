#include "notation.h"
#include <math.h>

int notation(char* input_str, double* result, double x) {
    struct st* op = initialize();
    return obxod(op, input_str, result, x);
}

int obxod(struct st* op, char* input_str, double* result, double x) {
    int flag = 1;
    double celoe = 0;
    for (int i = 0; flag && i < (int)strlen(input_str); ++i) {
        if (input_str[i] == 'x') {
            up_to_head(&op, x);
        }  // мы нашли число, оно первое в строке и перед ним ничего нет
        else if (is_digit(input_str[i]) && (i == 0 || !is_digit(input_str[i - 1]))) {
            celoe += input_str[i] - '0';
        }  // мы нашли цифру и перед ней стоит тоже цифра
        else if (is_digit(input_str[i]) && is_digit(input_str[i - 1])) {
            celoe *= 10;
            celoe += input_str[i] - '0';
        } else if (input_str[i] == '.') {  // если мы встретили точку - перед нами дробное число
            int cnt = 0;
            up_to_head(&op, celoe + ((while_float(input_str, &i, &cnt)) / pow(10, cnt)));
            celoe = 0;
            i++;
        } else if (!is_digit(input_str[i]) && is_digit(input_str[i - 1])) {  // если дошли до пробела
            up_to_head(&op, celoe);
            celoe = 0;
        } else if (is_binary(input_str[i])) {
            if (stack_size(op) < 2) flag = 0;
            binary_operation(&op, input_str[i], &flag);  // мы нашли бинарный оператор в строке
        }

        else if (is_unary(input_str[i])) {
            if (stack_size(op) < 1) flag = 0;
            unary_operation(op, input_str[i], &flag);  // мы нашли унарный оператор в строке
        }
    }
    if (flag) *result = del_node(&op);
    remove_all(&op);
    return flag;
}

double while_float(char* input_str, int* i, int* cnt) {
    double drob = 0;
    while (is_digit(input_str[*i + 1])) {
        drob += input_str[*i + 1] - '0';
        if (is_digit(input_str[*i + 2])) drob *= 10;
        (*i)++;
        (*cnt)++;
    }
    return drob;
}

void binary_operation(struct st** op, char input_char, int* flag) {
    double operand1 = del_node(op);
    double operand2 = del_node(op);
    if (input_char == '+')
        up_to_head(op, operand1 + operand2);
    else if (input_char == '*') {
        up_to_head(op, operand1 * operand2);
    } else if (input_char == '/') {
        if (fabs(operand1) <= EPS)
            *flag = 0;
        else
            up_to_head(op, operand2 / operand1);
    } else if (input_char == '-') {
        up_to_head(op, operand2 - operand1);
    }
}

void unary_operation(struct st* op, char input_char, int* flag) {
    double operand = del_node(&op);
    if (input_char == 'i') up_to_head(&op, sin(operand));
    if (input_char == 'o') up_to_head(&op, cos(operand));
    if (input_char == 'a') {
        if ((int)(operand / M_PI_2) % 2 != 0)
            *flag = 0;
        else
            up_to_head(&op, tan(operand));
    }
    if (input_char == 't') {
        if ((int)(operand / M_PI_2) % 2 == 0)
            *flag = 0;
        else
            up_to_head(&op, cos(operand) / sin(operand));
    }
    if (input_char == 'q') {
        if (operand < 0) {
            *flag = 0;
        } else
            up_to_head(&op, sqrt(operand));
    }
    if (input_char == 'n') {
        if (operand <= 0)
            *flag = 0;
        else
            up_to_head(&op, log(operand));
    }
    if (input_char == '~') {
        up_to_head(&op, operand * -1);
    }
}

int is_binary(char s) { return (s == '+' || s == '*' || s == '/' || s == '-'); }

int is_unary(char s) {
    return (s == 'i' || s == 'o' || s == 'a' || s == 't' || s == 'q' || s == 'n' || s == '~');
}

int is_digit(char z) { return '0' <= z && z <= '9'; }

struct st* initialize() {
    struct st* stack;
    stack = (struct st*)malloc(sizeof(struct st));
    stack->elem = 0;
    stack->next = NULL;
    return stack;
}

void up_to_head(struct st** head, double elem) {
    struct st* tmp = (struct st*)malloc(sizeof(struct st));
    if (tmp == NULL) {
        return;
    }
    tmp->next = *head;
    tmp->elem = elem;
    *head = tmp;
}

int stack_size(struct st* head) {
    int t = 1;
    if (head->next != NULL) t += stack_size(head->next);
    return t;
}

double del_node(struct st** head) {
    struct st* out;
    double elem;
    if (*head == NULL) {
        return 0;
    }
    out = *head;
    *head = (*head)->next;
    elem = out->elem;
    free(out);
    return elem;
}

void remove_all(struct st** head) {
    struct st* tmp = *head;
    while (*head != NULL) {
        *head = (*head)->next;
        free(tmp);
        tmp = *head;
    }
}
