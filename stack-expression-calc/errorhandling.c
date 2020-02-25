#include "errorhandling.h"
#include "stdio.h"

int validExpression;

void setError(ErrorCode errorCode) {
    char* msg;
    validExpression = 0;
    switch(errorCode) {
        case STACK_UNDERFLOW: msg = "Stack underflow."; break;
        case INVALID_EXPRESSION: msg = "Invalid expression."; break;
        case INVALID_SYMBOL: msg = "Invalid symbol entered.";
    }
    printf("ERROR: %s Enter new expression.", msg);
}

int errorOccurred() {
    return !validExpression;
}

void resetErrors() {
    validExpression = 1;
}