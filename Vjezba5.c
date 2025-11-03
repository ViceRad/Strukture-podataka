#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define SUCCESS 0
#define FAILURE 1

typedef struct _num* position;
typedef struct _num {
    double number;
    position next;
} num;

int push(position head, double n);
int pop(position head, double* value);
int isEmpty(position head);
int evaluatePostfix(char* expression, double* result);
int freeStack(position head);

int main() {
    const char* filePath = "postfix.txt";
    FILE* file = fopen(filePath, "r");
    if (!file) {
        printf("Greska pri otvaranju datoteke '%s'!\n", filePath);
        return FAILURE;
    }

    char buffer[MAX_LINE];
    while (fgets(buffer, sizeof(buffer), file)) {
        
        buffer[strcspn(buffer, "\n")] = 0;

        
        if (buffer[0] == '\0') continue;

        double result;
        if (evaluatePostfix(buffer, &result) == SUCCESS) {
            printf("%lf\n", result);
        }
        else {
            printf("Greska!\n");
        }
    }

    fclose(file);
    return SUCCESS;
}

int push(position head, double n) {
    position newNum = (position)malloc(sizeof(num));
    if (!newNum) return FAILURE;

    newNum->number = n;
    newNum->next = head->next;
    head->next = newNum;
    return SUCCESS;
}

int pop(position head, double* value) {
    if (isEmpty(head)) return FAILURE;

    position temp = head->next;
    *value = temp->number;
    head->next = temp->next;
    free(temp);
    return SUCCESS;
}

int isEmpty(position head) {
    return head->next == NULL;
}

int evaluatePostfix(char* expression, double* result) {
    num head;
    head.next = NULL;

    char* token = strtok(expression, " \t");
    double k, op1, op2;

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            if (push(&head, atof(token)) != SUCCESS) {
                freeStack(&head);
                return FAILURE;
            }
        }
        else {
            if (pop(&head, &op2) != SUCCESS || pop(&head, &op1) != SUCCESS) {
                freeStack(&head);
                return FAILURE;
            }

            if (strcmp(token, "+") == 0) k = op1 + op2;
            else if (strcmp(token, "-") == 0) k = op1 - op2;
            else if (strcmp(token, "*") == 0) k = op1 * op2;
            else if (strcmp(token, "/") == 0) {
                if (op2 == 0) {
                    freeStack(&head);
                    return FAILURE;
                }
                k = op1 / op2;
            }
            else {
                freeStack(&head);
                return FAILURE;
            }

            if (push(&head, k) != SUCCESS) {
                freeStack(&head);
                return FAILURE;
            }
        }
        token = strtok(NULL, " \t");
    }

    if (pop(&head, result) != SUCCESS || !isEmpty(&head)) {
        freeStack(&head);
        return FAILURE;
    }

    return SUCCESS;
}

int freeStack(position head) {
    double temp;
    while (!isEmpty(head)) {
        if (pop(head, &temp) != SUCCESS) {
            return FAILURE;
        }
    }
    return SUCCESS;
}