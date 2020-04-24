#include <stdio.h>
#include "stdlib.h"
#include "tree.h"
#include "stack/stack.h"
#include "symbols.h"


//region SIMPLIFICATION FUNCTIONS

/**
 * simplifies the subtree with root '+'
 */
void cleanup_addition(tree_node_t **treeNode) {
    tree_node_t *t = *treeNode;
    int leftIsNull = check_node_type(t->left, VAL_NULL);
    int rightIsNull = check_node_type(t->right, VAL_NULL);

    if (leftIsNull && rightIsNull) {
        set_tree_node(t, VAL_NULL, NULL, NULL);
    } else if (leftIsNull || rightIsNull) {
        *treeNode = leftIsNull ? t->right : t->left;
        free(leftIsNull ? t->left : t->right);
        free(t);
    }
}

/**
 * Simplifies the subtree with root '*'
 */
void cleanup_multiplication(tree_node_t **treeNode) {
    tree_node_t *t = *treeNode;
    int leftIsNull = check_node_type(t->left, VAL_NULL);
    int rightIsNull = check_node_type(t->right, VAL_NULL);
    int leftIsOne = check_node_type(t->left, VAL_1);
    int rightIsOne = check_node_type(t->right, VAL_1);

    if (leftIsNull || rightIsNull) {
        set_tree_node(t, VAL_NULL, NULL, NULL);
    } else if (leftIsOne || rightIsOne) {
        *treeNode = leftIsOne ? t->right : t->left;
        free(leftIsOne ? t->left : t->right);
        free(t);
    }
}

/**
 * Simplifies the subtree with root '-'
 */
void cleanup_bubtraction(tree_node_t **treeNode) {
    tree_node_t *t = *treeNode;
    int leftIsNull = check_node_type(t->left, VAL_NULL);
    int rightIsNull = check_node_type(t->right, VAL_NULL);
    int leftIsOne = check_node_type(t->left, VAL_1);
    int rightIsOne = check_node_type(t->right, VAL_1);

    if (rightIsNull) {
        *treeNode = t->left;
        free(t->right);
        free(t);
    } else if (leftIsNull) {
        set_tree_node(t, UN_SUB, t->right, NULL);
    } else if (leftIsOne && rightIsOne) {
        set_tree_node(t, VAL_NULL, NULL, NULL);
    }
}

/**
 * Simplifies the subtree with root '/'
 */
void cleanup_division(tree_node_t **treeNode) {
    tree_node_t *t = *treeNode;
    int nullLeft = check_node_type(t->left, VAL_NULL);

    if (nullLeft) {
        set_tree_node(t, VAL_NULL, NULL, NULL);
    }
}

/**
 * Simplifies the subtree with root '~'
 */
void cleanup_unary_subtraction(tree_node_t **treeNode) {
    tree_node_t *t = *treeNode;
    int nullLeft = check_node_type(t->left, VAL_NULL);

    if (nullLeft) {
        set_tree_node(t, VAL_NULL, NULL, NULL);
    }
}

//endregion

//region DERIVATION FUNCTIONS

/**
 *  (f(x) + g(x)) d/dx => f'(x) + g'(x)
 */
tree_node_t *dAddition(tree_node_stack_t *dStack, tree_node_t *root) {
    tree_node_t *right = pop_node(dStack), *left = pop_node(dStack);
    return new_tree_node(ADD, left, right);
}

/**
 * (f(x) - g(x)) d/dx => f'(x) - g'(x)
 */
tree_node_t *dSubtraction(tree_node_stack_t *dStack, tree_node_t *root) {
    tree_node_t* right = pop_node(dStack), *left = pop_node(dStack);
    return new_tree_node(SUB, left, right);
}

/**
 * ~f(x) d/dx => ~f'(x)
 */
tree_node_t *dUnSubtraction(tree_node_stack_t *dStack, tree_node_t *root) {
    tree_node_t *left = pop_node(dStack);
    return new_tree_node(UN_SUB, left, NULL);
}

/**
 * f(x) * g(x) d/dx => f'(x) * g(x) + g'(x) * f(x)
 */
tree_node_t *dMultiplication(tree_node_stack_t *dStack, tree_node_t *root) {
    // g'(x) * f(x)
    tree_node_t *right = new_tree_node(MUL, pop_node(dStack), copy_subtree(root->left));

    // f'(x) * g(x)
    tree_node_t *left = new_tree_node(MUL, pop_node(dStack), copy_subtree(root->right));

    // f'(x) * g(x) + g'(x) * f(x)
    return new_tree_node(ADD, left, right);
}

/**
 * f(x) / g(x) d/dx => (f'(x) * g(x) - g'(x) * f(x)) / g(x)^2
 */
tree_node_t *dDivision(tree_node_stack_t *dStack, tree_node_t *root) {

    // g'(x)*f(x)
    tree_node_t *leftLeft = new_tree_node(MUL, pop_node(dStack), copy_subtree(root->right));;

    // f'(x)*g(x)
    tree_node_t *leftRight = new_tree_node(MUL, copy_subtree(root->left), pop_node(dStack));

    // f'(x)*g(x) - g'(x)f(x)
    tree_node_t *left = new_tree_node(SUB, leftLeft, leftRight);

    // g(x)^2
    tree_node_t *right = new_tree_node(MUL, copy_subtree(root->right), copy_subtree(root->right));

    // (f'(x)*g(x) - g'(x)f(x)) / g(x)^2
    return new_tree_node(DIV, left, right);
}

/**
 * ln(f(x)) d/dx = f'(x) / f(x)
 */
tree_node_t *dNatLogarithm(tree_node_stack_t *dStack, tree_node_t *root) {
    tree_node_t *dt = pop_node(dStack);

    if (check_node_type(root->right, EXP)) {
        // ln(exp(f(x))) => f'(x)
        // f'(x) is already on the stack
        return dt;
    } else if (dt->token == VAL_NULL) {
        // ln(f(n)) d/dx = 1/f(n) * f'(n) = 1/f(n) * 0 = 0
        return dt;
    } else {
        // ln(f(x)) d/dx = 1/f(x) * f'(x) = f'(x) / f(x)
        return new_tree_node(DIV, dt, copy_subtree(root->right));
    }
}

tree_node_t *dExponentiation(tree_node_stack_t *dStack, tree_node_t *root) {
    tree_node_t *dRoot = malloc(sizeof(tree_node_t));
    tree_node_t *dRight = pop_node(dStack);
    tree_node_t *dLeft = pop_node(dStack);
    int nullLeft = check_node_type(dLeft, VAL_NULL);
    int nullRight = check_node_type(dRight, VAL_NULL);

    if (!nullLeft && nullRight) {
        // f(x)^g(n) - base is a function of x, and exponent is not
        // f(x)^g(n) d/dx => (g(n) * (f(x)^g(n) / f(x))) * f'(x) => g(n) * f(x)^(g(n)-1) * f'(x)

        // f(x)^g(n) / f(x)
        tree_node_t *leftRight = new_tree_node(DIV, copy_subtree(root), copy_subtree(root->left));

        // g(n) * (f(x)^g(n) / f(x))
        tree_node_t* left = new_tree_node(MUL, copy_subtree(root->right), leftRight);

        // (g(n) * (f(x)^g(n) / f(x))) * f'(x)
        return new_tree_node(MUL, left, dLeft);

    } else if (!nullRight) {
        //f(x)^g(x) d/dx => f(x)^g(x) * (ln(f(x)) * g'(x) + f'(x) / f(x) * g(x))

        // ln(f(x))
        tree_node_t *tLeft = newLnTreeNode(copy_subtree(dLeft));

        // ln(f(x)) * g(x)
        tree_node_t *t = new_tree_node(MUL, tLeft, copy_subtree(root->right));

        // ln(f(x)) d/dx  => f'(x) / f(x)
        push_node(dStack, dLeft);
        dLeft = dNatLogarithm(dStack, tLeft);

        // (ln(f(x)) * g(x)) d/dx => ln(f(x)) * g'(x) + f'(x) / f(x) * g(x)
        push_node(dStack, dLeft);
        push_node(dStack, dRight);
        tree_node_t* right = dMultiplication(dStack, t);

        free_subtree(t);



        // f(x)^g(x) * (ln(f(x)) * g'(x) + f'(x) / f(x) * g(x))
        return new_tree_node(MUL, copy_subtree(root), right);

    } else {
        // if neither base nor exponent is a function of x, the derivative is zero
        set_tree_node(dRoot, VAL_NULL, NULL, NULL);
        return dRoot;
    }
}

/**
 * log_a b d/dx => (ln b / ln a) d/dx ...
 */
tree_node_t *dLogarithm(tree_node_stack_t *dStack, tree_node_t *root) {
    tree_node_t* t = new_tree_node(DIV,
                                   newLnTreeNode(copy_subtree(root->right)),
                                   newLnTreeNode(copy_subtree(root->left)));


    tree_node_t* dLeft = dNatLogarithm(dStack, t->left);
    tree_node_t* dRight = dNatLogarithm(dStack, t->right);
    push_node(dStack, dLeft);
    push_node(dStack, dRight);

    tree_node_t *dRoot = dDivision(dStack, t);
    free_subtree(t);
    return dRoot;
}

/**
 * e^x => x * e^x
 */
tree_node_t *dExponential(tree_node_stack_t *dStack, tree_node_t *root) {
    return new_tree_node(MUL, copy_subtree(root), pop_node(dStack));
}

/**
 * |x| d/dx =>  (f'(x)*f(x)) / |f(x)|
 */
tree_node_t *dAbsolute(tree_node_stack_t *dStack, tree_node_t *root) {
    // f'(x)*f(x)
    tree_node_t* left = new_tree_node(MUL, pop_node(dStack), copy_subtree(root->left));

    // (f'(x)*f(x)) / |f(x)|
    return new_tree_node(DIV, left, copy_subtree(root));
}


tree_node_t *dToken(tree_node_t *treeNode, index_t dTokenIndex) {
    return new_tree_node(treeNode->entryIdx == dTokenIndex ? VAL_1 : VAL_NULL, NULL, NULL);
}

//endregion

tree_node_t* (*derivationFunctions[]) (tree_node_stack_t*, tree_node_t*) = {dLogarithm, dExponential, dAbsolute, dUnSubtraction,
                                                                            dAddition, dSubtraction, dMultiplication, dDivision,
                                                                            dExponentiation, NULL, NULL};

void (*cleanupFunctions[]) (tree_node_t **treeNode) = {NULL, NULL, NULL, cleanup_unary_subtraction, cleanup_addition,
                                                       cleanup_bubtraction, cleanup_multiplication, cleanup_division,
                                                       NULL, NULL, NULL, NULL};


extern void print_tree_to_stdout(tree_t *tree);

/**
 * Derives the whole expression (postorder traversal)
 */
tree_node_t *dTree(tree_t *tree, index_t dVariableEntryIdx) {
    tree_node_stack_t stack = NULL, dStack = NULL;
    tree_node_t *next = tree->root, *t = NULL;
    tree_node_t* (*derivationFunction) (tree_node_stack_t*, tree_node_t*);
    char visitFlag;
    while (next) {
        push_node_flag(&stack, next, VISITED_ONCE);
        next = next->left;
    }
    while (top_node(stack)) {
        next = pop_node_flag(&stack, &visitFlag);
        if (visitFlag == VISITED_ONCE) {
            push_node_flag(&stack, next, VISITED_TWICE);
            next = next->right;
            while (next) {
                push_node_flag(&stack, next, VISITED_ONCE);
                next = next->left;
            }
        } else {

            if (check_node_type(next, OP)) {
                t = dToken(next, dVariableEntryIdx);

            } else {

                if (check_ln_type(next)) {
                    // this node has NULL left child, so we need to pop it out of the stack
                    t = pop_node(&dStack);
                    free_subtree(pop_node(&dStack));
                    push_node(&dStack, t);

                    derivationFunction = dNatLogarithm;
                } else {
                    index_t idx = to_index(next->token);
                    if (idx == INVALID_INDEX) {
                        printf("Invalid node token\n\n");
                        return NULL;
                    }
                    derivationFunction = derivationFunctions[idx];
                }
                t = derivationFunction(&dStack, next);
            }

            push_node(&dStack, t);
        }
    }
    return pop_node(&dStack);
}

