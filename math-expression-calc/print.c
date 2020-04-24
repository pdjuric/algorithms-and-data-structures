#include <stdio.h>
#include <math.h>
#include <string.h>
#include "queue/queue.h"
#include "stack/stack.h"
#include "symbols.h"
#include "symboltable.h"
#include "tree.h"

#define underscoreCnt(i) ((i > 2) ? (((unsigned int) pow(2,(double)i-1))-2) : 0)
#define blankCnt(i) ((i > 2) ? ((unsigned int) pow(2,(double) i-1)) : ((i == 2) ? 2 : 0))


FILE* currentFile = NULL;

void put_char(int character) {
    currentFile ? fputc(character, currentFile) : putchar(character);
}

void put_chars(int character, int times) {
    for (int i = 0; i < times; i++)
        put_char(character);
}

int print(const char* f, int i) {
    return currentFile ? fprintf(currentFile, f, i) : printf(f, i);
}


void print_slashes(tree_node_queue_t queue, tree_node_t **current, unsigned char count) {
    tree_node_queue_t queue2 = new_tree_node_queue();
    tree_node_t *nextNode = *current;
    unsigned char level = count, slashDirection = 1;
    while (nextNode || !is_empty(queue)) {

        if (count > 0) for (int j = 0; j < blankCnt(count) + underscoreCnt(count) + 1; j++) put_char(' ');

        if (nextNode == NULL) put_char(' ');
        else put_char(slashDirection ? '/' : '\\');

        slashDirection = 1 - slashDirection;

        put_chars(' ', underscoreCnt(count) + blankCnt(count) + ((count > 0) ? 1 : 0));

        insert_node(&queue2, nextNode, level);
        if (!is_empty(queue)) nextNode = delete_node(&queue, &level);
        else nextNode = NULL;
        put_char(' ');
    }
    insert_node(&queue2, nextNode, level);
    *current = delete_node(&queue2, &level);
    copy(queue, queue2);
}


void get_node_repr(char* s, tree_node_t *t, symbol_table_t* symbolTable, int preferredSpace) {
    if (check_node_type(t, ABS), check_node_type(t, LOG), check_node_type(t, EXP))
        strcpy(s, to_string(t->token));
    else {

        symbol_t c;
        if (check_node_type(t, OP)) c = get_symbol(symbolTable, t->entryIdx);
        else c = to_symbol(t->token);

        if (preferredSpace == 3) {
            s[0] = s[2] = ' ';
            s[1] = c;
            s[3] = 0;
        } else {
            s[0] = c;
            s[1] = 0;
        }
    }
}

/**
 * Traverses the tree in level-order and prints the nodes.
 */
void print_tree_to_stdout(tree_t *tree) {
    tree_node_queue_t queue = new_tree_node_queue();
    tree_node_t *nextNode = tree->root;
    char c, s[4];
    unsigned char currLevel = tree->height, nextLevel = 0;

    insert_node(&queue, nextNode, currLevel);
    while (!is_empty(queue)) {
        nextNode = delete_node(&queue, (unsigned char *) &nextLevel);

        if (nextLevel == currLevel) {
            if (currLevel != tree->height)
                put_char(' ');
        } else {
            put_char('\n');
            print_slashes(queue, &nextNode, nextLevel);
            --currLevel;
            put_char('\n');
        }

        if (nextNode) {
            c = (nextLevel > 1) ? '_' : ' ';

            put_chars(' ', blankCnt(nextLevel));
            put_chars(c, underscoreCnt(nextLevel));

            get_node_repr(s, nextNode, &tree->symbolTable, nextLevel > 0 ? 3 : 1);
            print(s, 0);

            put_chars(c, underscoreCnt(nextLevel));
            put_chars(' ', blankCnt(nextLevel));

            // insert children or empty space
            if (nextNode && nextNode->left) insert_node(&queue, nextNode->left, nextLevel - 1);
            else if (nextLevel > 0) insert_node(&queue, NULL, nextLevel - 1);

            // insert children or empty space
            if (nextNode && nextNode->right) insert_node(&queue, nextNode->right, nextLevel - 1);
            else if (nextLevel > 0) insert_node(&queue, NULL, nextLevel - 1);
        } else {
            put_chars(' ', 2 * (int) pow(2, currLevel) - 1);
            if (nextLevel > 0) {
                insert_node(&queue, NULL, nextLevel - 1);
                insert_node(&queue, NULL, nextLevel - 1);
            }
        }
    }
}


void print_tree(tree_t* t, int toFile) {
    if (toFile) {
        currentFile = fopen("output.txt", "w");
        if (!currentFile) {
            printf("ERROR: Creating output file failed..");
            return;
        }
    } else {
        currentFile = NULL;
    }

    print_tree_to_stdout(t);

    if (toFile) {
        fclose(currentFile);
        currentFile = NULL;
    }

    printf("tree_t is to wide, so it will be printed to output.txt");

}


/**
 * Prints the expression (encoded in the tree) in prefix notation; traverses the tree in preorder
 */
void print_prefix(tree_t *tree) {
    tree_node_stack_t stack = NULL;
    tree_node_t *t;

    push_node(&stack, tree->root);
    while (top_node(stack)) {
        t = pop_node(&stack);
        while (t) {
            if (check_node_type(t, OP))
                printf("%c ", get_symbol(&tree->symbolTable, t->entryIdx));
            else printf("%s ", to_string(t->token));

            if (t->right) push_node(&stack, t->right);
            t = t->left;
        }
    }
}