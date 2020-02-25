#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#include "errorhandling.h"

#define MAXL 50
#define VARCNT 26

typedef enum {
    VAR_NOT_USED = 0,
    VAR_USED
} VarFlag;


char* expression = NULL;
Node* stack = NULL;
VarFlag* varFlags = NULL;
double* values = NULL;

void printMenu() {
    printf("Options:\n"
           "\t1 - load postfix expression\n"
           "\t2 - load variable values\n"
           "\t3 - calculate\n"
           "\t0 - exit\n\n"
           "Choice: ");
}

int isVar(char c) {
    return c >= 'A' && c <= 'Z';
}

int isOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '~';
}



void loadExpression() {
    if (varFlags != NULL) free(varFlags);
    if (values != NULL) free(values);
    if (expression != NULL) free(expression);
    expression = calloc(MAXL + 1, sizeof(char));
    varFlags = calloc(VARCNT, sizeof(VarFlag));
    resetErrors();

    printf("Enter postfix expression (A..Z + - * / ~) (max len %d): ", MAXL);
    scanf("%s", expression);

    for (char* currSymbol = expression; *currSymbol && !errorOccurred(); currSymbol++)
        if (isVar(*currSymbol))
            varFlags[*currSymbol - 'A'] = VAR_USED;
        else if (!isOp(*currSymbol))
            setError(INVALID_SYMBOL);
}

void setVarValues() {
    if (expression == NULL) {
        printf("Expression not entered..\n");
        return;
    }

    if (errorOccurred()) {
        printf("Error occurred. Enter new expression.\n");
        return;
    }
    values = calloc(26, sizeof(double));
    for (int idx = 0; idx < VARCNT; idx++)
        if (varFlags[idx] == VAR_USED)
            do {
                printf("%c = ", idx + 'A');
            } while (scanf("%lf", &values[idx]) == 0);
}



void calc() {
    if (expression == NULL) {
        printf("Expression not entered..\n");
        return;
    }

    if (values == NULL) {
        printf("Enter values of variables first!\n");
        return;
    }

    if (errorOccurred()) {
        printf("Error occurred. Enter new expression.\n");
        return;
    }

    for (char* currSymbol = expression; *currSymbol != '\0' && !errorOccurred(); currSymbol++) {
        double val;
        if (isVar(*currSymbol))
            val = values[*currSymbol - 'A'];
        else {
            double a, b;
            b = pop(&stack);
            if (*currSymbol != '~') a = pop(&stack);

            switch(*currSymbol) {
                case '+': val = a + b; break;
                case '-': val = a - b; break;
                case '*': val = a * b; break;
                case '/': val = a / b; break;
                case '~': val = - b; break;
            }
        }

        push(&stack, val);
    }

    if (!errorOccurred()) {
        if (isEmpty(stack)) setError(INVALID_EXPRESSION);
        else {
            double result = pop(&stack);
            if (!isEmpty(stack)) setError(INVALID_EXPRESSION);
            else printf("\nRESULT: %.2f\n", result);
        }
    }

    empty(&stack);
}


int main() {
    int c;
    while (1) {
        printMenu();
        scanf("%d", &c);

        switch(c) {
            case 1: loadExpression(); break;
            case 2: setVarValues(); break;
            case 3: calc(); break;
            case 0: return 0;
        }
    }
}