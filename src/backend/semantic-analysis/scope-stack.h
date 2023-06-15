#ifndef SCOPE_STACK_H
#define SCOPE_STACK_H

#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>

typedef struct StackNode StackNode;

struct StackNode{
    int scope;
    StackNode * next;
};

typedef struct StackList{
    StackNode * first;
    size_t size;
    size_t index;
}StackList;

void init_stack(StackList * stack);
int pop(StackList * stack);
void push(StackList * stack);
bool is_stack_empty(StackList * stack);
int peek(StackList * stack);
bool is_in_current_scope(StackList * stack, int scope);

void freeScopeStack(StackList * stack);
void freeScopeNode(StackNode * node);


#endif