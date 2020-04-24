#ifndef ASP1_STRUCT_H
#define ASP1_STRUCT_H

typedef char symbol_t;
typedef unsigned char symbol_table_size_t;
#define TRANSIENT_SYMBOL 1

typedef enum {
    LOG,
    EXP,
    UN_SUB,
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    ABS,
    VAL_1,
    VAL_NULL,
    OP
} token_t;

typedef unsigned char index_t;
#define INVALID_INDEX ((index_t) ~0)
#define MAX_INDEX (INVALID_INDEX - 1)

#define VISITED_ONCE 1
#define VISITED_TWICE 2


typedef struct symbol_stack_node {
    symbol_t info;
    struct symbol_stack_node *next;
} symbol_stack_node_t;

typedef symbol_stack_node_t* SymbolStack;


typedef struct {
    char symbol;
    double value;
} symbol_table_entry_t;


typedef struct {
    symbol_table_entry_t* entries;
    symbol_table_size_t size;
    symbol_table_size_t allocated;
} symbol_table_t;


typedef struct tree_node {
    token_t token;                              // token
    unsigned char h;                            // height of the tree or temp field
    index_t entryIdx;                           // index of the symbol in the symbol table
    struct tree_node *left, *right;             // children
} tree_node_t;


typedef struct {
    tree_node_t* root;                          // ptr to the table root
    symbol_table_t symbolTable;                 // symbol table
    unsigned char height;                       // tree height
    unsigned char dVarIdx;                      // todo
    unsigned char dCount;                       // todo
} tree_t;




typedef struct tree_node_queue_node {
    tree_node_t *info;
    unsigned char level;
    struct tree_node_queue_node *next;
} tree_node_queue_node_t;

typedef struct {
    tree_node_queue_node_t *f, *b;
}  tree_node_queue_t;



typedef struct tree_node_stack_node {
    tree_node_t *info;
    char flag;
    struct tree_node_stack_node *next;
} tree_node_stack_node_t;

typedef tree_node_stack_node_t*  tree_node_stack_t;






typedef enum  {
    OK, DIVISION_BY_ZERO,
    LOG_UNDEFINED,                  // Log _ for base _ not defined.
    OP_UNKNOWN, UNKNOWN_FUNC,
    NO_EXPRESSION,                  // Expression not loaded
    UNKNOWN_TREE,                   // Tree does not exist.
    UNKNOWN_SYMBOL
} error_t;


#endif //ASP1_STRUCT_H
