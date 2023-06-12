#include "symbol-list.h"


void init_list(symbol_list * list){
    list->first = NULL;
    list->size = 0;
}

bool contains_symbol(symbol_list * list, char * node_name, bool is_array){
    symbol_node * node = list->first;
    while(node != NULL){
        if( strcmp(node->name, node_name) == 0){
            if(node->is_array == is_array || node->is_pointer)
                return true;
            return false;
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

char * generateNewIndex(symbol_list * list){
    char* variable_name = (char*)malloc(sizeof(char) * 3);
    strcpy(variable_name, "a");

    while (contains_symbol(list, variable_name, true) || contains_symbol(list, variable_name, false)) {
        int len = strlen(variable_name);

        // If we've reached the end of the alphabet, append two letters
        if (variable_name[len - 1] == 'z') {
            variable_name = (char*)realloc(variable_name, (len + 2) * sizeof(char));
            variable_name[len] = 'a';
            variable_name[len + 1] = 'a';
            variable_name[len + 2] = '\0';
        }
        // Increment the last character by one
        else {
            variable_name[len - 1]++;
        }
    }

    symbol_node * node = malloc(sizeof(symbol_node));
    node->is_array = false;
    node->is_function = false;
    node->is_pointer = false;
    node->name = variable_name;
    node->next = NULL;
    node->type = Int;
    add_symbol(list, node);
    return variable_name;
}