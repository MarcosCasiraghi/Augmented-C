#include "error-list.h"

void init_error_list( error_list * error_list){
    error_list->first = NULL;
    error_list->size = 0;
}


int add_symbol_to_error_list(error_list * error_list, error_node * node){
    error_node * current_node = error_list->first;
    error_node * prev_node = NULL;
    if( current_node == NULL ){
        error_list->first = node;
        error_list->size++;
        return 1;
    }
    while( current_node != NULL && current_node->lineNumber < node->lineNumber){
        prev_node = current_node;
        current_node = current_node->next;
    }
    if( prev_node == NULL){
        node->next = error_list->first;
        error_list->first = node;
    }else{
        prev_node->next = node;
        node->next = current_node;
    }
    error_list->size++;
    return 1;
}

void printErrors(error_list * error_list){
    error_node * list_node = error_list->first;
    if( list_node == NULL )
        return;
    printf("\nLos siguientes errores mencionados no van a ser de compilacion de C\nSolamente de errores de nuestra nueva declaración\n");
    if( error_list->size == 1 ){
        printf("1 error encontrado\n\n");
    }else
        printf("%d errores encontrados:\n\n", error_list->size);
    while(list_node->next != NULL){
        printf("%s\n", list_node->message);
        list_node = list_node->next;
    }
    printf("%s\n", list_node->message);
}

void freeErrorList(error_list * list){
    if( list->first != NULL)
        freeErrorNode(list->first);
}

void freeErrorNode( error_node * node){
    if( node->next != NULL)
        freeErrorNode(node->next);
    free(node);
}
