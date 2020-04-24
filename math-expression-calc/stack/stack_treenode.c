#include <stdlib.h>
#include "stack.h"

void push_node(tree_node_stack_t* stack, tree_node_t* node) {
    push_node_flag(stack, node, 0);
}

tree_node_t* pop_node(tree_node_stack_t *stack) {
    return pop_node_flag(stack, 0);
}

tree_node_t* top_node(tree_node_stack_t stack) {
    return stack ? stack->info : NULL;
}

void push_node_flag(tree_node_stack_t* stack, tree_node_t* node, char flag) {
    tree_node_stack_node_t *t = malloc(sizeof(tree_node_stack_node_t));
    t->info = node;
    t->flag = flag;
    t->next = *stack ? *stack : NULL;
    *stack = t;
}

tree_node_t* pop_node_flag(tree_node_stack_t *stack, char *flag) {
    if (!*stack) return NULL;

    tree_node_stack_node_t *t = *stack;
    *stack = (*stack)->next;
    tree_node_t *n = t->info;
    if (flag) *flag = t->flag;
    free(t);
    return n;
}