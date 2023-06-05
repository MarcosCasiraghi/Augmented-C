#include "symbol-list.h"


void init_list(symbol_list * list){
    list->first = NULL;
    list->size = 0;
}

bool contains_symbol(symbol_list * list, char * node_name){
    symbol_node * node = list->first;
    while(node != NULL){
        if( strcmp(node->name, node_name) == 0){
            return true;
        }
        node = node->next;
    }
    return false;
}

int add_symbol(symbol_list * list, symbol_node * node){
    symbol_node * list_node = list->first;
    if( list_node == NULL ){
        list->first = node;
        return 1;
    }

    while( list_node->next != NULL ){
        if( strcmp(list_node->name, node->name) == 0){
            //la variable ya existe
            //TODO - manejo de errores
            return 0;
        }
        list_node = list_node->next;
    }
    list_node->next = node;
    return 1;
}