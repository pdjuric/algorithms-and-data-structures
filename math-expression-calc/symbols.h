#ifndef ASP1DZ2_SYMBOLS_H
#define ASP1DZ2_SYMBOLS_H

#include "struct.h"


const char* to_string(token_t token);
token_t to_token(symbol_t symbol);
char to_symbol(token_t token);
int is_variable(token_t token);
index_t to_index(token_t token);

char input_priority(symbol_t symbol);
char stack_priority(symbol_t symbol);

int is_symbolic_operator(symbol_t symbol);
int is_function(const char* functionString);

#define is_operand(symbol) ((symbol) >= 'A' && (symbol) <= 'Z')
#define is_function_start(symbol) ((symbol) >= 'a' && (symbol) <= 'z')


#endif //ASP1DZ2_SYMBOLS_H
