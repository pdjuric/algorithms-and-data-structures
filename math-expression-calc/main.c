#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "symboltable.h"
#include <stdlib.h>


extern error_t parse_input(tree_t *pTree);
extern void print_tree(tree_t* t, int toFile);
extern void print_prefix(tree_t *tree);
extern error_t calc_tree(tree_t tree, double* res);
extern tree_node_t *dTree(tree_t *tree, index_t dVariableEntryIdx);



void print_menu() {
    printf("1. Input new expression\n"
           "2. Print tree\n"
           "3. Print expression (prefix notation)\n"
           "4. Calculate expression\n"
           "5. Differentiate expression\n"
           "0. Exit\n\n"
           "Choose an option: ");
}

error_t choose_tree(char treeCount, tree_t *trees, char* idx) {
    if (treeCount == 0) return NO_EXPRESSION;

    if (treeCount == 1) {
        *idx = 0;
        return OK;
    }

    printf("Choose expression:\n"
           "O - starting expression\n");

    for (char i = 1; i < treeCount; i++)
        printf("%d - %d. derivative with respect to variable %c\n", i, trees[i].dCount, trees[i].dVarIdx);

    *idx = -1;
    scanf("%hhd", idx);

    if (*idx >= treeCount) return UNKNOWN_TREE;
    getchar();
    return OK;
}






int main() {
    tree_t *trees = NULL, *tree;
    char treeCnt = 0, treeIdx, c;
    symbol_table_t *symbolTable;
    error_t status;
    double value;
    while (1) {
        print_menu();
        scanf("%hhd", &c);
        switch (c) {
            case 1:
                delete_all_trees(&trees, &treeCnt);
                alloc_tree(&trees, &treeCnt);
                getchar();
                printf("Enter the expression ( supported functions - log(X,Y) abs(X) exp(X) ): ");
                status = parse_input(&trees[treeCnt - 1]);
                if (status != OK)
                    switch (status) {
                    case UNKNOWN_FUNC:
                        printf("An unknown function has been used in the expression.");
                        break;
                    case UNKNOWN_SYMBOL:
                        printf("A forbidden symbol has been used in the expression.");
                        break;
                    default:
                        printf("Invalid option.");
                        break;
                    }
                putchar('\n');
                break;
            case 2:
                if (choose_tree(treeCnt, trees, &treeIdx) == OK) {
                    print_tree(&trees[treeIdx], 0);
                }
                break;
            case 3:
                if (choose_tree(treeCnt, trees, &treeIdx) == OK)
                    print_prefix(&trees[treeIdx]);
                break;
            case 4:
                if (choose_tree(treeCnt, trees, &treeIdx) == OK) {
                    symbolTable = &trees[treeIdx].symbolTable;
                    for (index_t idx = 0; idx < symbolTable->size; idx++) {
                        printf("%c = ", get_symbol(symbolTable, idx));
                        scanf("%lf", &value);
                        set_value(symbolTable, idx, value);
                    }
                    status = calc_tree(trees[treeIdx], &value);
                    switch (status) {
                        case OK:
                            printf("Expression value is %lf\n\n", value);
                            break;
                        default:
                            printf("An error occurred.\n\n");

                    }
                    break;
                    case 5:
                        getchar();
                    if (choose_tree(treeCnt, trees, &treeIdx) == OK) {
                        printf("With respect to which variable should the expression be differentiated? ");
                        scanf("%c", &c);
                        if (c < 'A' || c > 'Z') {
                            printf("The entered variable is not allowed.\n\n");
                            break;
                        }
                        trees = realloc(trees, ++treeCnt * sizeof(tree_t));
//                        tree_t* tree = &trees[treeCnt - 1];
                        tree = &trees[treeCnt - 1];

//                        alloc_tree(&trees, &treeCnt);
//                        tree = &trees[treeCnt - 1];
                        symbolTable = &trees[0].symbolTable;
                        index_t idx = 0;
                        while (idx < symbolTable->size && get_symbol(symbolTable, idx) != c) idx++;
                        if (idx == trees[treeIdx].symbolTable.size) idx = INVALID_INDEX;
                        tree->dVarIdx = idx;
//                    if (i == trees[0].symbolTable.size) {
//                        printf("Slucaj sa promenjlivom koje nema u izrazu.\n\n");
//                        i = '!';
//                    }
                        tree->dVarIdx = c;
                        tree->dCount = trees[treeIdx].dCount + 1;
                        tree->root = dTree(&trees[treeIdx], idx);
                        tree->height = tree->root->h;
                        init_symbol_table(tree, trees[treeIdx].symbolTable);
                        putchar('\n');
                    }
                    break;
                    case 0:
                        delete_all_trees(&trees, &treeCnt);
                    return 0;
                    default:
                        printf("Invalid option.\n\n");
                }
        }
        putchar('\n');
        putchar('\n');
    }
}
/*TEST PRIMERI
 * exp(abs(C*X^C-C/(X+A)))
 * A*log(E,exp(C+D*abs(A)))-(-C)
 * log(A^X,X/B)^(X+C)
 * log(exp(-C),abs(A+B+(-C)))
 * A+(B*(-C)/A)/A           diferencirano po A ne moze da stane u konzolu !!
 * A^C*exp(A)
 * (X^A*B)*log(A^B,A*X+B)
 * log(A^X+B/exp(C),abs(A*X^(A/X)+C/A))
 * A*B+C/A
 * (-(A+C^X-X/A))
 *
 * FORMAT ISPISA STABLA
 *(X^A*B)*log(A^B,A*X+B)
 *
 * log(exp(-C),abs(A+B+(-C)))
 *


                                  _____________________________ E _____________________________
                                /                                                               \
                    ___________log_____________                                     ___________abs_____________
                  /                             \                                 /
           _____ - _____                   _____exp_____                   _____log_____
         /               \               /                               /               \
       _ + _           _ * _           _ B _                           _ R _           _log_
     /       \       /       \                                                       /       \
     *       *       ~       A                                                      exp     exp
   /   \   /   \   /                                                               /       /
   A   X   C   ^   C                                                               A       B
              / \
              X C


*/