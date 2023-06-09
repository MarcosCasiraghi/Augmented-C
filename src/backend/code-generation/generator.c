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

void GenSizeNode(SizeNode * node){
	if (node->type == VariableSize ){
		printf("%s", node->variable);
	}else{
		GenNumConstantIntNode(node->numConstantIntNode);
	}
}

void GenArrayDerefNode(ArrayDerefNode * node){
	printf("%s[", node->variable);
	GenSizeNode(node->sizeNode);
	printf("]");
}

void GenPointerNode(PointerNode * node){
	printf("*");
	if( node->child == HasChild){
		GenPointerNode(node->pointerNode);
	}
}

void GenSingleInitializeNode(SingleInitializeNode * node){
	if( node->type == AssignSingle) {
		printf(" = ");
		GenExpressionNode(node->expressionNode);
	}
	printf(";\n");
}

void GenSingleDeclarationNode(SingleDeclarationNode * node){
	GenDataType(node->dataType);
	if( node->type == SingleWithPointer){
		GenPointerNode(node->pointer);
	}
	printf(" %s", node->variable);
	GenSingleInitializeNode(node->singleInitializeNode);
}

void GenDataType(DataType dataType){
	switch( dataType ){
		case Int:
			printf("int ");
			break;
		case Float:
			printf("float ");
			break;
		case Double:
			printf("double ");
			break;
		case Long:
			printf("long ");
			break;
		case Short:
			printf("short ");
			break;
		case Char:
			printf("char ");
			break;
		case VoidPointer:
			printf("void * ");
			break;
		case Void:
			printf("void ");
			break;
		default:
			//no deberia entrar aca nunca
	}
}

void GenArrayInitializeNode(ArrayInitializeNode * node){
	if( node->type == WithList){
		printf(" = {");
		GenArrayListNode(node->arrayListNode);
		printf("}");
	}
	printf(";\n");
}

void GenArrayListNode(ArrayListNode * node){
	printf("%d", node->integer);
	if( node->child == HasChild ){
		printf(", ");
		GenArrayListNode(node->arrayListNode);
	}
}

void GenArraySizeNode(ArraySizeNode * node){
	printf("[");
	if(node->child == HasChild && node->type == NotSizedSingle){
		printf("]");
		GenArraySizeNode(node->arraySizeNode);
	}else if(node->child == HasChild && node->type == Sized){
		printf("%d]", node->numberConstant);
		GenArraySizeNode(node->arraySizeNode);
	}else if( node->child == NoChild && node->type == Sized){
		printf("%d]", node->numberConstant);
	}else{
		printf("]");
	}
}

void GenArrayDeclarationNode(ArrayDeclarationNode * node){
	GenDataType(node->dataType);
	printf("%s ", node->variable);
	GenArraySizeNode(node->arraySizeNode);
	GenArrayInitializeNode(node->arrayInitializeNode);
}