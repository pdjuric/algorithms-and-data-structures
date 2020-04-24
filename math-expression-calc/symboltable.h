#ifndef ASP1_SYMBOLTABLE_H
#define ASP1_SYMBOLTABLE_H

#include "struct.h"


void init_symbol_table(tree_t *tree, symbol_table_t oldTable);
void shrink_symbol_table(symbol_table_t* symbolTable, symbol_table_size_t size);


index_t add_symbol(symbol_table_t* symbolTable, symbol_t symbol);
symbol_t get_symbol(symbol_table_t* symbolTable, index_t idx);

double get_value(symbol_table_t* symbolTable, index_t idx);
void set_value(symbol_table_t* symbolTable, index_t idx, double value);


#endif //ASP1_SYMBOLTABLE_H




