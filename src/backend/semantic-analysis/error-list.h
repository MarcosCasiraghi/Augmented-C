#ifndef ERROR_LIST_H
#define ERROR_LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 100

typedef struct error_node error_node;

struct error_node{
    char message[SIZE];
    error_node * next;
};

typedef struct error_list{
    error_node * first;
    size_t size;
}error_list;

void init_error_list( error_list * error_list);
int add_symbol_to_error_list(error_list * error_list, error_node * node);
void printErrors(error_list * error_list);

#endif