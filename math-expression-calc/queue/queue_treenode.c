#include <stdlib.h>
#include "queue.h"

tree_node_queue_t new_tree_node_queue() {
    tree_node_queue_t queue = {NULL, NULL};
    return queue;
}

tree_node_t* delete_node(tree_node_queue_t* queue, unsigned char *level) {
    if (queue->b) {
        tree_node_queue_node_t *t = queue->b;
        queue->b = queue->b->next;
        if (!queue->b) queue->f = NULL;
        tree_node_t *r = t->info;
        *level = t->level;
        free(t);
        return r;
    } else return NULL;
}

void insert_node(tree_node_queue_t* queue, tree_node_t *node, unsigned char level) {
    tree_node_queue_node_t *t = malloc(sizeof(tree_node_queue_node_t));
    t->info = node;
    t->level = level;
    if (queue->f) queue->f->next = t;
    else queue->b = t;
    t->next = NULL;
    queue->f = t;
}

int is_empty(tree_node_queue_t queue) {
    if (queue.f || queue.b) return 0;
    else return 1;
}

void copy(tree_node_queue_t dst, tree_node_queue_t src) {
    dst.b = src.b;
    dst.f = src.f;
}
