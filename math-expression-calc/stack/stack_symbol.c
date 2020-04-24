#include <stdlib.h>
#include "stack.h"

void push_symbol(SymbolStack* stack, symbol_t symbol) {
    symbol_stack_node_t *n = malloc(sizeof(symbol_stack_node_t));
    n->info = symbol;
    n->next = *stack ? *stack : NULL;
    *stack = n;
}

symbol_t pop_symbol(SymbolStack* stack) {
    if (!*stack) return 0;
    symbol_stack_node_t *t = *stack;
    *stack = (*stack)->next;
    char c = t->info;
    free(t);
    return c;
}

symbol_t top_symbol(SymbolStack stack) {
    return stack ? stack->info : 0;
}
