#ifndef ASP1_STACK_H
#define ASP1_STACK_H

#include "../struct.h"


void push_symbol(SymbolStack* stack, symbol_t symbol);
symbol_t pop_symbol(SymbolStack* stack);
symbol_t top_symbol(SymbolStack stack);

void push_node(tree_node_stack_t* stack, tree_node_t* node);
tree_node_t* pop_node(tree_node_stack_t *stack);
tree_node_t* top_node(tree_node_stack_t stack);

void push_node_flag(tree_node_stack_t* stack, tree_node_t* newNode, char flag);
tree_node_t* pop_node_flag(tree_node_stack_t *stack, char *flag);


#endif //ASP1_STACK_H
