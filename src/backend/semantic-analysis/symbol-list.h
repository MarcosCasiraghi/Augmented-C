#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "abstract-syntax-tree.h"
#include <stdlib.h>

typedef struct symbol_node symbol_node;

struct symbol_node{
    char * name;
    DataType type;
    bool is_pointer;
    bool is_array;
    bool is_function;
    //TODO - agregar scope
    symbol_node * next;
};

typedef struct symbol_list{
    symbol_node * first;
    size_t size;
}symbol_list;

void init_list( symbol_list * list);

bool contains_symbol(symbol_list * list, char * node_name);

int add_symbol(symbol_list * list, symbol_node * node);

char * generateNewIndex(symbol_list * list);

#endif