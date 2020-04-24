#include "symbols.h"
#include "struct.h"
#include <string.h>

token_t tokens[] = {LOG, EXP, ABS, UN_SUB, ADD, SUB, MUL, DIV, POW, VAL_NULL, VAL_1};
const char* strings[] = {"log", "exp", "abs"};
symbol_t symbols[] = {'l', 'e', 'a', '~', '+', '-', '*', '/' , '^', 'x', '1' };
char tempStr[2];

const char* to_string(token_t token) {
    switch (token) {
        case LOG: case EXP: case ABS:
            return strings[to_index(token)];
        default:
            tempStr[0] = to_symbol(token);
            return tempStr;
    }
}

index_t to_index(token_t token) {
    for (index_t i = 0; i < 11; i++)
        if (tokens[i] == token) return i;
    return INVALID_INDEX;
}

symbol_t to_symbol(token_t token) {
    index_t idx = to_index(token);
    if (idx == INVALID_INDEX) return '?';
    else return symbols[idx];
}

token_t to_token(symbol_t symbol){
    for (int i = 0; i < 11; i++)
        if (symbols[i] == symbol) return tokens[i];
    return symbol;
}

int is_variable(token_t token) {
    for (int i = 0; i < 9; i++)
        if (token == tokens[i]) return 0;
    return 1;
}

int is_function(const char* functionString) {
    token_t functions[] = {ABS, EXP, LOG};

    for (int idx = 0; idx < 3; idx++)
        if (strcmp(functionString, to_string(functions[idx])))
            return 1;

    return 0;
}

int is_symbolic_operator(symbol_t symbol) {
    for (int i = 3; i < 9; i++)
        if (symbol == symbols[i])
            return 1;
    if (symbol == '(' || symbol == ')' || symbol == ',') return 1;
    return 0;
}

char input_priority(symbol_t symbol){
    switch(symbol) {
        case '(':
            return 6;
        case '^':
            return 5;
        case '*': case '/':
            return 3;
        case 'a': case 'e': case 'l': case '+': case '-': case '~':
            return 2;
        case ',': case ')':
            return 1;
        default:
            return -1;
    }
}

char stack_priority(symbol_t symbol){
    switch(symbol) {
        case '^':
            return 4;
        case '*': case '/':
            return 3;
        case 'a': case 'e': case 'l': case '+': case '-': case '~':
            return 2;
        case ',': case '(':
            return 0;
        default:
            return -1;
    }
}
