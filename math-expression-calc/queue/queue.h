#ifndef ASP1_QUEUE_TREENODE_H
#define ASP1_QUEUE_TREENODE_H

#include "../struct.h"


tree_node_queue_t new_tree_node_queue();
tree_node_t* delete_node(tree_node_queue_t* queue, unsigned char *level);
void insert_node(tree_node_queue_t* queue, tree_node_t *node, unsigned char level);
int is_empty(tree_node_queue_t queue);
void copy(tree_node_queue_t dst, tree_node_queue_t src);

#endif //ASP1_QUEUE_TREENODE_H
