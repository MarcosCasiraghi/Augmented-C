#include "scope-stack.h"

void init_stack(StackList * stack){
    stack->size = 0;
    stack->index = 0;

    //se agrega scope base
	StackNode * node = malloc(sizeof(StackNode));
	node->scope = stack->index++;
	node->next = NULL;

    stack->first = node;
    stack->size++;
}

int pop(StackList * stack){
    if( is_stack_empty(stack)){
        return 0;
    }
    StackNode * temp = stack->first;
    stack->first = stack->first->next;
    free(temp);
    stack->size--;
    return 1;
}

void push(StackList * stack){
    StackNode * node = malloc(sizeof(StackNode));
    node->next = stack->first;
    node->scope = stack->index++;
    stack->first = node;
    stack->size++;
}

bool is_stack_empty(StackList * stack){
    return stack->size <= 0;
}

int peek(StackList * stack){
    if( is_stack_empty(stack))
        return -1;
    return stack->first->scope;
}

bool is_in_current_scope(StackList * stack, int scope){
    if( is_stack_empty(stack))
        return false;
    StackNode * node = stack->first;
    while(node != NULL){
        if(node->scope == scope)
            return true;
        node = node->next;
    }
    return false;
}

void freeScopeStack(StackList * stack){
    if( stack->first != NULL)
        freeScopeNode(stack->first);
}

void freeScopeNode(StackNode * node){
    if(node->next != NULL)
        freeScopeNode(node->next);
    free(node);
}

