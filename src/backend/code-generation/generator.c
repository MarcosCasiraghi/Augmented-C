#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

void Generator(int result) {
	LogInfo("El resultado de la expresion computada es: '%d'.", result);
}

void GenMetaCommandNode(MetaCommandNode * node){
	printf("#include ");
	if( node->type == MetaCommandString){
		printf("\"%s\"\n", node->string);
	}else {
		printf("<%s>\n", node->string);
	}	
}
