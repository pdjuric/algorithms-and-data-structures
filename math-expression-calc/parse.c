#include <printf.h>
#include "stack/stack.h"
#include "tree.h"
#include "symboltable.h"
#include "symbols.h"
#include "struct.h"


/**
 * Creates a tree from infix expression.
 */
error_t parse_input(tree_t *pTree) {
    char lastCharWasOpenParenthesis = 0;
    symbol_t symbol;
    token_t token;
    tree_node_t *t = NULL;

    tree_node_stack_t treeNodeStack = NULL;
    SymbolStack symbolStack = NULL;

    while ((symbol = (char) getchar()) != '\n') {

        // set flag when open parentheses come
        if (symbol == '(') lastCharWasOpenParenthesis = 1;
        else if (lastCharWasOpenParenthesis) {
            // when the input is "(-", - is converted to unary minus (~)
            if (symbol == to_symbol(SUB)) symbol = to_symbol(UN_SUB);
            lastCharWasOpenParenthesis = 0;
        }

        if (is_operand(symbol)) {
            // add it to the symbol table, or get its current entry index
            // add the token to the tree
            index_t idx = add_symbol(&pTree->symbolTable, symbol);
            t = new_operand_tree_node(idx);
            push_node(&treeNodeStack, t);

        } else if (is_function_start(symbol)) {
            // load whole function string
            char functionString[] = {symbol, 0, 0, 0};
            scanf("%2c", functionString + 1);

            // check whether the function name is valid
            if (!is_function(functionString)) {
                while (getchar() != '\n');
                return UNKNOWN_FUNC;
            }

            // add function to the symbol stack
            push_symbol(&symbolStack, symbol);
        } else if (is_symbolic_operator(symbol)) {

            // pop all the operations with greater stack priority than current operation's input priority
            while (top_symbol(symbolStack) && input_priority(symbol) <= stack_priority(top_symbol(symbolStack))) {
                // and add a new node with the popped operation to the tree
                token = to_token(pop_symbol(&symbolStack));
                t = new_tree_node_from_stack(token, &treeNodeStack);
                push_node(&treeNodeStack, t);
            }

            // , popped all the operands except (
            // , isn't pushed to the stack
            if (symbol == ',') continue;
            else if (symbol == ')') {
                // pop the open parentheses '(' from the stack
                pop_symbol(&symbolStack);

                // if the function is on top of the stack, it is added to the tree
                symbol = top_symbol(symbolStack);
                if (symbol == to_symbol(ABS) || symbol == to_symbol(LOG) || symbol == to_symbol(EXP)) {
                    token = to_token(pop_symbol(&symbolStack));
                    t = new_tree_node_from_stack(token, &treeNodeStack);
                    push_node(&treeNodeStack, t);
                }
            } else {
                // in case of any other symbol, push it to the stack
                push_symbol(&symbolStack, symbol);
            }
        } else {
            while (getchar() != '\n');
            return UNKNOWN_SYMBOL;
        }
    }

    while (top_symbol(symbolStack)) {
        token = to_token(pop_symbol(&symbolStack));
        t = new_tree_node_from_stack(token, &treeNodeStack);
        push_node(&treeNodeStack, t);
    }

    pTree->root = pop_node(&treeNodeStack);
    pTree->height = pTree->root->h;
    return OK;
}
