#include <stdlib.h>
#include "symboltable.h"
#include "stack/stack.h"
#include "symbols.h"

/**
 * Function that inits new symbol table w/o allocating it.
 */
void init_symbol_table(tree_t* tree, symbol_table_t oldTable) {
    symbol_table_t *newTable = &tree->symbolTable;
    tree_node_stack_t stack = NULL;
    newTable->entries = calloc(oldTable.size , sizeof(symbol_table_entry_t));       // zero init
    newTable->size = 0;
    index_t idx;
    tree_node_t *t;
    push_node(&stack, tree->root);
    while (top_node(stack)) {
        t = pop_node(&stack);
        while (t) {
            if (is_variable(idx = t->token) && !newTable->entries[idx].symbol) {
                ++newTable->size;
                newTable->entries[idx].symbol = oldTable.entries[idx].symbol;
                newTable->entries[idx].value = 0;
            }
            if (t->right) push_node(&stack, t->right);
            t = t->left;
        }
    }
}

/**
 * Adds new symbol to the symbol table; \n no duplicate symbols are allowed; \n multiple TRANSIENT_SYMBOLs are allowed
 * @param symbolTable - table in which to add the symbol
 * @param symbol - symbol to insert in the table
 * @returns index - if symbol is added or it already exists in the table
 * @returns invalid index - if new table entry could not be allocated
 */
index_t add_symbol(symbol_table_t* symbolTable, symbol_t symbol) {
    if (symbol != TRANSIENT_SYMBOL)
        for (index_t idx = 0; idx < symbolTable->size; idx++)
            if (symbolTable->entries[idx].symbol == symbol)
                return idx;

    if (symbolTable->allocated == symbolTable->size) {
        if (symbolTable->allocated ==  MAX_INDEX) return INVALID_INDEX;

        if (symbolTable->allocated + 10 > MAX_INDEX) symbolTable->allocated = MAX_INDEX;
        else symbolTable->allocated += 10;
        symbolTable->entries = realloc(symbolTable->entries, symbolTable->allocated * sizeof(symbol_table_entry_t));
    }
    symbolTable->entries[symbolTable->size].symbol = symbol;
    symbolTable->entries[symbolTable->size].value = 0;
    return symbolTable->size++;
}

symbol_t get_symbol(symbol_table_t* symbolTable, index_t idx) {
    return symbolTable->entries[idx].symbol;
}

double get_value(symbol_table_t* symbolTable, index_t idx){
    return symbolTable->entries[idx].value;
}

void set_value(symbol_table_t* symbolTable, index_t idx, double value){
    symbolTable->entries[idx].value = value;
}


/**
 * Deletes all symbol table entries that exceed provided size
 */
void shrink_symbol_table(symbol_table_t* symbolTable, symbol_table_size_t size) {
    symbolTable->entries = realloc(symbolTable->entries, size * sizeof(symbol_table_entry_t));
    symbolTable->allocated = size;
    symbolTable->size = size;
}