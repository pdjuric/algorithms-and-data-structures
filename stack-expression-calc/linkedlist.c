#include "linkedlist.h"
#include "errorhandling.h"
#include <stdlib.h>

void push(Node** list, double val) {
    Node* t = (Node*) malloc(sizeof(Node));
    t->value = val;
    t->next = *list;
    *list = t;
}

double pop(Node** head) {
    if (*head == NULL) {
        setError(STACK_UNDERFLOW);
        return 0;
    }

    double val = (*head)->value;
    Node* t = *head;
    (*head) = (*head)->next;
    free(t);
    return val;
}

int isEmpty(Node *head) {
    return head == NULL;
}

void empty(Node** list) {
    Node* t;
    while (*list != NULL) {
        t = *list;
        *list = (*list)->next;
        free(t);
    }
}