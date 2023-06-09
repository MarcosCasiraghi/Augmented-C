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
		printf("%s\n", node->variable);
	}else{
		printf("int %d", node->numConstantIntNode);
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
			break;
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

// las de mg

void GenDeclarationNode(DeclarationNode * node) {

}

void GenAssignmentNode(AssignmentNode * node) {

}

void GenFunctionCallNode(FunctionCallNode * node) {

}

void GenFunctionCallArgNode(FunctionCallArgNode * node) {

}

void GenExpressionNode(ExpressionNode * node) {

}

void GenReturnStatementNode(ReturnStatementNode * node) {

}

void GenIfStatementNode(IfStatementNode * node) {

}

void GenElseStatementNode(ElseStatementNode * node) {

}

//

void GenIfElseStatementNode(IfElseStatementNode * node) {
	if(node->type == withElse) {
		GenIfStatementNode(node->ifStatementNode);
		GenElseStatementNode(node->elseStatementNode);
	}
	else {
		GenIfStatementNode(node->ifStatementNode);
	}
}

void GenWhileStatementNode(WhileStatementNode * node) {
	printf("while(");
	GenExpressionNode(node->expressionNode);
	printf(") {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("}\n");
}

void GenForStatementNode(ForStatementNode * node) {
	printf("for (");
	GenDeclarationNode(node->declarationNode);
	printf(", ");
	GenExpressionNode(node->firstExpressionNode);
	printf(", ");
	if(node->type == withAssignment) {
		GenAssignmentNode(node->AssignmentNode);
	}
	else {
		GenExpressionNode(node->expressionNode);
	}
	printf(") {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("}\n");
}

void GenSwitchStatementNode(SwitchStatementNode * node) {
	printf("switch (");
	GenExpressionNode(node->expressionNode);
	printf(") {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("}\n");
}


void GenCodeBlockNode(CodeBlockNode * node) {
	switch( node->type ) {
		case DeclarationStatement:
			GenDeclarationNode(node->declarationNode);
			break;
		case SpecialStatement:
			GenSpecialStatementNode(node->specialStatement);
			break;
		case ExpressionStatement:
			GenExpressionNode(node->expressionNode);
			printf(";\n");
			break;
		case ReturnStatement:
			GenReturnStatementNode(node->returnStatement);
			break;
		case IfElseStatement:
			GenIfElseStatementNode(node->ifElse);
			break;
		case ForStatement:
			GenForStatementNode(node->forStatement);
			break;
		case WhileStatement:
			GenWhileStatementNode(node->whileStatement);
			break;
		case SwitchStatement:
			GenSwitchStatementNode(node->switchStatement);
			break;
		case AssignmentStatement:
			GenAssignmentNode(node->assingment);
			printf(";\n");
			break;
		case CaseStatement:
			if(node->child == HasChild) {
				printf("case ");
				GenExpressionNode(node->expression);
				printf(":\n");
				GenCodeBlockNode(node->codeBlock);
				printf("break;\n");
			}
			break;
		case ContinueStatement:
			printf("continue;\n");
			break;
		case DefaultCaseStatement:
			printf("default:\n");
			break;
		case BreakStatement:
			printf("break;");
			break;
		default:
			break;
			//impossible
	}
	if(node->child == HasChild) {
		GenCodeBlockNode(node->codeBlock);
	}
}

void GenFunctionArgNode(FunctionArgNode * node) {
	GenDataType(node->dataType);
	if(node->pointer == pointer) {
		GenPointerNode(node->pointer);
	}
	printf("%s\n", node->variable);
}

void GenFunctionArgsNode(FunctionArgsNode * node) {
	GenFunctionArgNode(node->functionArgNode);
	if(node->type == multiple) {
		printf(", ");
		GenFunctionArgsNode(node->functionArgsNode);
	}
}

void GenFunctionDeclarationNode(FunctionDeclarationNode * node) {
	if(node->type == FunctionDeclarationNoArgs) {
		GenDataType(node->type);
		printf("%s ( ) {\n", node->variable);
	} else if(node->type == FunctionDeclarationWithArgs) {
		GenDataType(node->type);
		printf("%s (", node->variable);
		GenFunctionArgsNode(node->functionArgs);
		printf(") {\n");
	} else if(node->type == FunctionDeclarationNoArgsVoid) {
		printf("void %s () {\n", node->variable);
	} else {
		printf("void %s (", node->variable);
		GenFunctionArgsNode(node->functionArgs);
		printf(") {\n");
	}
	GenCodeBlockNode(node->codeBlock);
	printf("}");
}

void GenStatementNode(StatementNode * node) {
	switch(node->type) {
		case MetaCommand:
			GenMetaCommandNode(node->metacommand);
			break;
		case FunctionDeclaration:
			GenFunctionDeclarationNode(node->functionDeclarationNode);
			break;
		case Declaration:
			GenDeclarationNode(node->declarationNode);
			break;
		default:
			break;
			//impossible
	} 
	if(node->child == HasChild){
		GenStatementNode(node->statement);
	}
}

void GenSpecialStatementNode(SpecialStatementNode * node) {
	
}
