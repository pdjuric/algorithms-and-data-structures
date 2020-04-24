
#ifndef ASP1_TREE_H
#define ASP1_TREE_H

#include "struct.h"

error_t parse_input(tree_t* pTree);

tree_node_t* new_tree_node(token_t token, tree_node_t* leftChild, tree_node_t* rightChild);

tree_node_t* new_tree_node_from_stack(token_t token, tree_node_stack_t *stack);
tree_node_t* new_operand_tree_node(index_t idx);
tree_node_t *newLnTreeNode(tree_node_t* rightChild);
void set_tree_node(tree_node_t* treeNode, token_t token, tree_node_t* leftChild, tree_node_t* rightChild);
int check_node_type(tree_node_t* treeNode, token_t token);
int check_ln_type(tree_node_t* treeNode);


void delete_all_trees(tree_t** ppTree, char* pTreeCnt);
void alloc_tree(tree_t** trees, char* treeCnt);
void free_tree(tree_t *tree);

tree_node_t *copy_subtree(tree_node_t *root);
void free_subtree(tree_node_t *root);

#endif //ASP1_TREE_H









