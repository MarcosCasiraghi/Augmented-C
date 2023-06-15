#include "symbol-list.h"


void init_list(symbol_list * list){
    list->first = NULL;
    list->size = 0;
}

bool contains_symbol(symbol_list * list, char * node_name, bool is_array, bool forGenerate){
    symbol_node * node = list->first;
    while(node != NULL){
        if( strcmp(node->name, node_name) == 0 && ( forGenerate || is_in_current_scope(state.stack, node->scope))){
            if(node->is_array == is_array || node->is_pointer){
                return true;
            }  
        }
        node = node->next;
    }
    return false;
}

int add_symbol(symbol_list * list, symbol_node * node){
    symbol_node * list_node = list->first;
    if( list_node == NULL ){
        list->first = node;
        list->size++;
        return 1;
    }

    while( list_node->next != NULL ){
        list_node = list_node->next;
    }
    list_node->next = node;
    list->size++;
    return 1;
}

char * generateNewIndex(symbol_list * list){
    char* variable_name = (char*)malloc(sizeof(char) * 3);
    strcpy(variable_name, "a");

    while (contains_symbol(list, variable_name, true, true) || contains_symbol(list, variable_name, false, true)) {
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


void freeSymbolList(symbol_list * list){
    if( list->first != NULL)
        freeSymbolNode(list->first);
}

void freeSymbolNode(symbol_node * node){
    if( node->next != NULL)
        freeSymbolNode(node->next);
    //no es necesario liberar el nombre pues es el mismo de la variable del nodo que ya se libera
    // free(node->name);
    free(node);
}