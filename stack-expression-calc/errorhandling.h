//
// Created by Pavle Djuric on 25/02/2020.
//

#ifndef errorhandling
#define errorhandling

typedef enum {
    STACK_UNDERFLOW,
    INVALID_EXPRESSION,
    INVALID_SYMBOL
} ErrorCode;

void setError(ErrorCode errorCode);
int errorOccurred();
void resetErrors();

#endif
