#include <stdlib.h>
#include "stack/stack.h"
#include "tree.h"

/**
 * Frees all the memory used for the subtree; postorder tree traversal
 */
void free_subtree(tree_node_t *root) {
    tree_node_stack_t stack = NULL;
    tree_node_t *next = root;
    char visitFlag;
    while (next) {
        push_node_flag(&stack, next, VISITED_ONCE);
        next = next->left;
    }
    while (top_node(stack)) {
        next = pop_node_flag(&stack, &visitFlag);
        if (visitFlag == VISITED_ONCE) {
            push_node_flag(&stack, next,VISITED_TWICE);
            next = next->right;
            while (next) {
                push_node_flag(&stack, next, VISITED_ONCE);
                next = next->left;
            }
        } else {
            free(next);
        }
    }
}

/**
 * Frees all the memory used for the tree (root subtree + symbol table)
 */
void free_tree(tree_t *tree) {
    free_subtree(tree->root);
    free(tree->symbolTable.entries);
}

/**
 * Returns a deep copy of provided subtree.
 * Used when an expression is being differentiated
 */
tree_node_t *copy_subtree(tree_node_t *root) {
    tree_node_stack_t stack = NULL, treeStack = NULL;
    tree_node_t *next = root, *t = NULL;
    char visitFlag;
    while (next) {
        push_node_flag(&stack, next, VISITED_ONCE);
        next = next->left;
    }
    while (top_node(stack) != NULL) {
        next = pop_node_flag(&stack, &visitFlag);
        if (visitFlag == VISITED_ONCE) {
            push_node_flag(&stack, next, VISITED_TWICE);
            next = next->right;
            while (next) {
                push_node_flag(&stack, next, VISITED_ONCE);
                next = next->left;
            }
        } else {
            t = new_tree_node_from_stack(next->token, &treeStack);
            if (next->token == OP) t->entryIdx = next->entryIdx;
            push_node(&treeStack, t);
        }
    }
    t = pop_node(&treeStack);
    return t;
}


void delete_all_trees(tree_t** ppTree, char* pTreeCnt) {
    for (int i = 0; i < *pTreeCnt; i++) free_tree(ppTree[i]);
    free(*ppTree);
    *ppTree = NULL;
    *pTreeCnt = 0;
}

void alloc_tree(tree_t** trees, char* treeCnt) {
    *trees = realloc(*trees, ++(*treeCnt) * sizeof(tree_t));
    tree_t* tree = &*trees[*treeCnt - 1];
    tree->root = NULL;
    tree->symbolTable.size = 0;
    tree->symbolTable.entries = NULL;
    tree->symbolTable.allocated = 0;
    tree->dCount = 0;
    tree->dVarIdx = 0;
    tree->height = 0;
}
