//
// Created by Pavle Djuric on 25/02/2020.
//

#ifndef linkedlist
#define linkedlist

typedef struct listNode {
    double value;
    struct listNode *next;
} Node;


void push(Node** list, double val);
double pop(Node** head);
int isEmpty(Node *head);
void empty(Node** list);

#endif
