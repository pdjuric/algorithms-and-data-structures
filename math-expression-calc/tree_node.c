#include <stdlib.h>
#include "stack/stack.h"
#include "tree.h"
#include "symbols.h"


extern void (*cleanupFunctions[]) (tree_node_t **treeNode);

/**
 * @return zero-init'd tree_node_t
 */
tree_node_t* get_empty_tree_node(){
    tree_node_t* t = malloc(sizeof(tree_node_t));
    t->left = t->right = NULL;
    t->h = 0;
    t->entryIdx = INVALID_INDEX;
    return t;
}

/**
 * Adjusts the height of a tree_node_t as the max height of its subtrees + 1.
 * Does not traverse the subtrees recursively, just reads info from children nodes.
 * In order for height to be correct, all subtrees must have up-to-date heights.
 */
void set_height(tree_node_t *t) {
    t->h = 0;
    if (t->left) t->h = t->left->h + 1;
    if (t->right && t->right->h >= t->h) t->h = t->right->h + 1;
}

/**
 * Creates new tree_node_t with given token and children; simplifies the subtree based on the node token and adjusts the
 * height of the tree_node_t.
 */
tree_node_t* new_tree_node(token_t token, tree_node_t* leftChild, tree_node_t* rightChild) {
    tree_node_t *treeNode = get_empty_tree_node();
    treeNode->token = token;
    treeNode->left = leftChild;
    treeNode->right = rightChild;
    index_t idx = to_index(token);
    // todo index out of bounds?
    void (*cleanupFunction) (tree_node_t **treeNode) = cleanupFunctions[idx];
    if (cleanupFunction != NULL) cleanupFunction(&treeNode);
    set_height(treeNode);
    return treeNode;
}
/**
 * Changes token and children of the given node (frees old children if needed) and adjusts the height of the node
 */
void set_tree_node(tree_node_t* treeNode, token_t token, tree_node_t* leftChild, tree_node_t* rightChild) {
    if (treeNode->left != leftChild && treeNode->left != rightChild)
        free(treeNode->left);
    if (treeNode->right != leftChild && treeNode->right != rightChild)
        free(treeNode->right);

    treeNode->token = token;
    treeNode->left = leftChild;
    treeNode->right = rightChild;
    set_height(treeNode);
}

/**
 * Creates new tree_node_t with given token; populates the children pointers with children from the top of the provided
 * stack; simplifies the subtree based on the node token; adjusts the height of the tree_node_t.
 */
tree_node_t* new_tree_node_from_stack(token_t token, tree_node_stack_t *stack) {
    tree_node_t *l = NULL, *r = NULL;
    switch (token) {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case POW:
        case LOG:
            r = pop_node(stack);
        case EXP:
        case ABS:
        case UN_SUB:
            l = pop_node(stack);
        case VAL_1:
        case VAL_NULL:
        case OP:
            return new_tree_node(token, l, r);
        default:
            return NULL;
    }
}

/**
 * Creates new tree_node_t with OP (operand) token; populates the entryIdx with provided index of entry in symbol table
 */
tree_node_t* new_operand_tree_node(index_t idx) {
    tree_node_t *t = get_empty_tree_node();
    t->token = OP;
    t->entryIdx = idx;
    return t;
}



/**
 * Checks whether the provided node is of provided type
 */
int check_node_type(tree_node_t* treeNode, token_t token) {
    return treeNode != NULL && treeNode->token == token;
}

/// inspects whether the provided node is Ln
int check_ln_type(tree_node_t* treeNode){
    if (check_node_type(treeNode, LOG) && check_node_type(treeNode->left, EXP) &&
        check_node_type(treeNode->left->left, VAL_1)) return 1;
    else return 0;
}

///
tree_node_t *newLnTreeNode(tree_node_t* rightChild) {
    // 1
    tree_node_t *one = new_tree_node(VAL_1, NULL, NULL);

    // e^1
    tree_node_t *expOne = new_tree_node(EXP, one, NULL);

    // ln => log_(e^1) (rightChild)
    return new_tree_node(LOG, expOne, rightChild);
}

