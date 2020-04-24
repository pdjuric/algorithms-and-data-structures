#include <math.h>
#include <string.h>
#include "stack/stack.h"
#include "symboltable.h"
#include "tree.h"

/**
 * Calculates the operation when given concrete operands
 * @param tree - tree that contains symbol table in which to find symbols' values
 * @param node - node to calculate the value for
 * @param result - location in which to save the result
 * @returns error that occurred - DIVISION_BY_ZERO, LOG_UNDEFINED, OP_UNKNOWN or OK
 */
error_t calculate(tree_t tree, tree_node_t *node, double* result) {
    double  op1, op2;

    if (node->left->token == VAL_1) op1 = 1;
    else op1 = get_value(&tree.symbolTable, node->left->entryIdx);

    // if a node has a right child, it is the second operand
    if (!node->right) op2 = 0;
    else if (node->right->token == VAL_1) op2 = 1;
    else op2 = get_value(&tree.symbolTable, node->right->entryIdx);

    switch (node->token) {
        case ADD: *result = op1 + op2; break;
        case SUB: *result = op1 - op2; break;
        case MUL: *result = op1 * op2; break;
        case UN_SUB: *result = -op1; break;
        case POW: *result = pow(op1, op2); break;
        case ABS: *result = fabs(op1); break;
        case EXP: *result = exp(op1); break;
        case DIV:
            if (op2 == 0) return DIVISION_BY_ZERO;
            *result = op1/op2; break;
        case LOG:
            if (!(op1 > 1 && op2 > 1)) return LOG_UNDEFINED;
            *result = log(op2) / log(op1); break;
        default:
            return OP_UNKNOWN;
    }
    return OK;
}

/**
 * Calculates the infix expression encoded in the tree (postorder traversal)
 */
error_t calc_tree(tree_t tree, double* res) {
    symbol_table_t* symbolTable = &tree.symbolTable;
    tree_node_t *next = tree.root;
    tree_node_stack_t stack = NULL;
    error_t err = OK;
    symbol_table_size_t symbolTableSize = symbolTable->size;
    char visitFlag;
    double result = 0;

    // fixme prompt for values of all the symbols in the symbol_t table


    while (next) {
        // the node is traversed the first time, so it's being pushed to the stack
        push_node_flag(&stack, next, VISITED_ONCE);
        next = next->left;
    }
    while (top_node(stack)) {
        next = pop_node_flag(&stack, &visitFlag);
        // if the node is already visited
        if (visitFlag == VISITED_ONCE) {
            push_node_flag(&stack, next, VISITED_TWICE);
            next = next->right;
            while (next) {
                // the node is traversed the first time, so it's being pushed to the stack
                push_node_flag(&stack, next, VISITED_ONCE);
                next = next->left;
            }
        }
            // visit the node only if it is not a leaf, as leaf does not contain an operation
        else if (next->left || next->right) {
            err = calculate(tree, next, &result);
            if (err != OK) break;

            // if the left operand is a transient value, and not a symbol from input, overwrite it
            if (!check_node_type(next->left, OP)) {
                next->entryIdx = next->left->entryIdx;
            }
                // if the left operand is a symbol, but the right operand is a transient value, overwrite the right one
            else if (!check_node_type(next->right, OP)) {
                next->entryIdx = next->right->entryIdx;
            }
                // if neither of operands are transient values, make new entry in symbol table
            else  {
                next->entryIdx = add_symbol(symbolTable, TRANSIENT_SYMBOL);
            }

            set_value(symbolTable, next->entryIdx, result);

        }
    }
    *res = get_value(symbolTable, tree.root->entryIdx);

    // delete all the entries that were allocated for transient values
    shrink_symbol_table(symbolTable, symbolTableSize);
    return err;
}

