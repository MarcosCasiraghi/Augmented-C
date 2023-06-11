#include "../support/logger.h"
#include "generator.h"

static void GenExpressionType(ExpressionNodeType type);
static void GenDataType(DataType dataType);

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
		printf("%d", node->numConstantIntNode);
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

void GenSingleInitializeNode(SingleInitializeNode * node, int isFor){
	if( node->type == AssignSingle) {
		printf(" = ");
		GenExpressionNode(node->expressionNode, isFor);
	}
	if(!isFor && node->expressionNode == NULL) {
		printf(";\n");
		return;
	}
	if(!isFor && node->expressionNode->functionCallNode == NULL){
		printf(";\n");
	}
}

void GenSingleDeclarationNode(SingleDeclarationNode * node, int isFor){
	GenDataType(node->dataType);
	if( node->type == SingleWithPointer){
		GenPointerNode(node->pointer);
	}
	printf("%s", node->variable);
	GenSingleInitializeNode(node->singleInitializeNode, isFor);
}

static void GenDataType(DataType dataType){
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

void GenDeclarationNode(DeclarationNode * node, int isFor) {
	switch(node->type) {
		case SingleDeclaration:
			GenSingleDeclarationNode(node->singleDeclarationNode, isFor);
			break;
		case ArrayDeclaration:
			GenArrayDeclarationNode(node->arrayDeclarationNode);
		default:
			break;
			//impossible
	}
}

void GenAssignmentType(AssignmentType type) {
	switch(type) {
		case ASSIGN_TYPE:
			printf(" = ");
			break;
		case SUM_ASSIGN_TYPE:
			printf(" += ");
			break;
		case SUB_ASSIGN_TYPE:
			printf(" -= ");
			break;
		case MULT_ASSIGN_TYPE:
			printf(" *= ");
			break;
		case DIV_ASSIGN_TYPE:
			printf(" /= ");
			break;
		case MOD_ASSIGN_TYPE:
			printf(" %%= ");
			break;
		default:
			break; // impossible
	}
}

void GenAssignmentNode(AssignmentNode * node, int isForArg) {
	if(node->withType == withVar) {
		printf("%s", node->variable);
	} else if(node->withType == withArrayDeref) {
		GenArrayDerefNode(node->arrayDefinitionNode);
	} else {
		return; // error
	}
	GenAssignmentType(node->assignmentType);
	GenExpressionNode(node->expressionNode, 0);
}

void GenFunctionCallNode(FunctionCallNode * node, int isFor) {
	printf("%s(", node->Variable);
	if(node->type == WithArgs) {
		GenFunctionCallArgNode(node->functionCallArgNode, isFor);
	}
	if(!isFor) printf(");\n");
	else printf(")");
}

void GenFunctionCallArgNode(FunctionCallArgNode * node, int isFor) {
	GenExpressionNode(node->expressionNode, isFor);
	if(node->type == FunctionCallWithArgs) {
		printf(", ");
		GenFunctionCallArgNode(node->functionCallArgNode, isFor);
	}
}

static void GenExpressionType(ExpressionNodeType type) {
	switch(type) {
		case AddOp:
			printf(" + ");
			break;
		case SubOp:
			printf(" - ");
			break;
		case MultOp:
			printf(" * ");
			break;
		case DivOp:
			printf(" / ");
			break;
		case ModOp:
			printf(" %% ");
			break;
		case IncOp:
			printf("++");
			break;
		case DecOp:
			printf("--");
			break;
		case BitNotOp:
			printf("~");
			break;
		case BitRightOp:
			printf(">>");
			break;
		case BitLeftOp:
			printf("<<");
			break;
		case BitXorOp:
			printf("^");
			break;
		case BitOrOp:
			printf("|");
			break;
		case BitAndOp:
			printf("&");
			break;
		case AndOp:
			printf(" && ");
			break;
		case OrOp:
			printf(" || ");
			break;
		case NotOp:
			printf("!(");
			break;
		case EqOp:
			printf(" == ");
			break;
		case GrOp:
			printf(" > ");
			break;
		case GeOp:
			printf(" >= ");
			break;
		case LtOp:
			printf(" < ");
			break;
		case LeOp:
			printf(" <= ");
			break;
		case NeOp:
			printf(" != ");
			break;
		case WithParenthesis:
			printf("(");
			break;
		default:
			break;
	}
}

void GenExpressionNode(ExpressionNode * node, int isFor) {
	if(node->leftExpressionNode != NULL) {
		GenExpressionNode(node->leftExpressionNode, isFor);
	}
	GenExpressionType(node->op);
	if(node->rightExpressionNode != NULL) {
		GenExpressionNode(node->rightExpressionNode, isFor);
	}
	if(node->op == NotOp || node->op == WithParenthesis) {
		printf(")");
	}
	if(node->rightExpressionNode == NULL && node->leftExpressionNode == NULL) {
		switch(node->op) {
			case VariableType:
				printf("%s", node->Variable);
				break;
			case NumConstantFloat:
				printf("%s", node->numConstantFloatNode);
				break;
			case NumConstantInt:
				printf("%d", node->numConstantIntNode);
				break;
			case specialVariable:
				printf("%s", node->specialVariable);
				break;
			case functionCall:
				GenFunctionCallNode(node->functionCallNode, isFor);
				break;
			case ArrayDeref:
				GenArrayDerefNode(node->arrayDerefNode);
				break;
			case String:
				printf("%s", node->StringNode);
				break;
			default:
				break; //impossible
		}
	}
	
}

void GenReturnStatementNode(ReturnStatementNode * node) {
	printf("return ");
	GenExpressionNode(node->expressionNode, 0);
	printf(";\n");
}

void GenIfStatementNode(IfStatementNode * node) {
	printf("if (");
	GenExpressionNode(node->expressionNode, 0);
	printf(") {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("}\n");
}

void GenElseStatementNode(ElseStatementNode * node) {
	printf("else {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("}\n");
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
	GenExpressionNode(node->expressionNode, 0);
	printf(") {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("}\n");
}

void GenForStatementNode(ForStatementNode * node) {
	printf("for (");
	GenDeclarationNode(node->declarationNode, 1);
	printf("; ");
	GenExpressionNode(node->firstExpressionNode, 1);
	printf("; ");
	if(node->type == withAssignment) {
		GenAssignmentNode(node->AssignmentNode, 1);
	}
	else {
		GenExpressionNode(node->expressionNode, 1);
	}
	printf(") {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("}\n");
}

void GenSwitchStatementNode(SwitchStatementNode * node) {
	printf("switch (");
	GenExpressionNode(node->expressionNode, 0);
	printf(") {\n");
	GenCodeBlockNode(node->codeBlockNode);
	printf("\n}\n");
}


void GenCodeBlockNode(CodeBlockNode * node) {
	switch( node->type ) {
		case DeclarationStatement:
			GenDeclarationNode(node->declarationNode, 0);
			break;
		case SpecialStatement:
			GenSpecialStatementNode(node->specialStatement);
			break;
		case ExpressionStatement:
			if( node->expression && node->expressionNode)
				printf("\n\n\nlos 2 al mismo tiempo\n\n\n");
			if( node->expression){
				GenExpressionNode(node->expression, 0);
			}else if(node->expressionNode )
				GenExpressionNode(node->expressionNode, 0);
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
			GenAssignmentNode(node->assingment, 0);
			printf(";\n");
			break;
		case CaseStatement:
			printf("case ");
			GenExpressionNode(node->expression, 0);
			printf(":\n");
			break;
		case ContinueStatement:
			printf("continue;\n");
			break;
		case DefaultCaseStatement:
			printf("default:\n");
			break;
		case BreakStatement:
			printf("break;\n");
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
	if(node->type == pointer) {
		GenPointerNode(node->pointer);
	}
	printf("%s", node->variable);
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
		GenDataType(node->functionType);
		printf("%s() {\n", node->variable);
	} else if(node->type == FunctionDeclarationWithArgs) {
		GenDataType(node->functionType);
		printf("%s(", node->variable);
		GenFunctionArgsNode(node->functionArgs);
		printf(") {\n");
	} else if(node->type == FunctionDeclarationNoArgsVoid) {
		printf("void %s() {\n", node->variable);
	} else {
		printf("void %s(", node->variable);
		GenFunctionArgsNode(node->functionArgs);
		printf(") {\n");
	}
	GenCodeBlockNode(node->codeBlock);
	printf("}\n");
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
			GenDeclarationNode(node->declarationNode, 0);
			break;
		default:
			break;
			//impossible
	} 
	if(node->child == HasChild){
		GenStatementNode(node->statement);
	}
}

void GenProgramNode(ProgramNode * node){
	GenStatementNode(node->statementNode);
}

void GenSpecialStatementNode(SpecialStatementNode * node) {

}
