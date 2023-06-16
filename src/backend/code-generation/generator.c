#include "../support/logger.h"
#include "generator.h"

static void GenExpressionType(ExpressionNodeType type);
static void GenDataType(DataType dataType);

/**
 * Implementación de "generator.h".
 */

void GenMetaCommandNode(MetaCommandNode * node){
	fprintf(state.fd,"#include ");
	if( node->type == MetaCommandString){
		fprintf(state.fd,"\"%s\"\n", node->string);
		
	}else {
		fprintf(state.fd,"<%s>\n", node->string);
		
	}	
}

void GenSizeNode(SizeNode * node, bool shouldNewLine){
	if (node->type == VariableSize ){
		fprintf(state.fd,"%s", node->variable);
		if(shouldNewLine){
			fprintf(state.fd,"");
			
		}
	}else{
		fprintf(state.fd,"%d", node->numConstantIntNode);
	}
}

void GenArrayDerefNode(ArrayDerefNode * node){
	fprintf(state.fd,"%s[", node->variable);
	GenSizeNode(node->sizeNode, 0);
	fprintf(state.fd,"]");
}

void GenPointerNode(PointerNode * node){
	fprintf(state.fd,"*");
	if( node->child == HasChild){
		GenPointerNode(node->pointerNode);
	}
}

void GenSingleInitializeNode(SingleInitializeNode * node, int isFor){
	if( node->type == AssignSingle) {
		fprintf(state.fd," = ");
		GenExpressionNode(node->expressionNode, isFor, NULL, NULL);
	}
	if(!isFor && (node->expressionNode == NULL || node->expressionNode->functionCallNode == NULL)) {
		fprintf(state.fd,";");
		
		return;
	}
}

void GenSingleDeclarationNode(SingleDeclarationNode * node, int isFor){
	GenDataType(node->dataType);
	if( node->type == SingleWithPointer){
		GenPointerNode(node->pointer);
	}
	fprintf(state.fd,"%s", node->variable);
	GenSingleInitializeNode(node->singleInitializeNode, isFor);
}

static void GenDataType(DataType dataType){
	switch( dataType ){
		case Int:
			fprintf(state.fd,"int ");
			break;
		case Float:
			fprintf(state.fd,"float ");
			break;
		case Double:
			fprintf(state.fd,"double ");
			break;
		case Long:
			fprintf(state.fd,"long ");
			break;
		case Short:
			fprintf(state.fd,"short ");
			break;
		case Char:
			fprintf(state.fd,"char ");
			break;
		case VoidPointer:
			fprintf(state.fd,"void * ");
			break;
		case Void:
			fprintf(state.fd,"void ");
			break;
		default:
			break;
			//no deberia entrar aca nunca
	}
}

void GenArrayInitializeNode(ArrayInitializeNode * node){
	if( node->type == WithList){
		fprintf(state.fd," = {");
		GenArrayListNode(node->arrayListNode);
		fprintf(state.fd,"}");
	}
	fprintf(state.fd,";");
	
}

void GenArrayListNode(ArrayListNode * node){
	fprintf(state.fd,"%d", node->integer);
	if( node->child == HasChild ){
		fprintf(state.fd,", ");
		GenArrayListNode(node->arrayListNode);
	}
}

void GenArraySizeNode(ArraySizeNode * node){
	fprintf(state.fd,"[");
	if(node->child == HasChild && node->type == NotSizedSingle){
		fprintf(state.fd,"]");
		GenArraySizeNode(node->arraySizeNode);
	}else if(node->child == HasChild && node->type == Sized){
		fprintf(state.fd,"%d]", node->numberConstant);
		GenArraySizeNode(node->arraySizeNode);
	}else if( node->child == NoChild && node->type == Sized){
		fprintf(state.fd,"%d]", node->numberConstant);
	}else{
		fprintf(state.fd,"]");
	}
}

void GenArrayDeclarationNode(ArrayDeclarationNode * node){
	GenDataType(node->dataType);
	fprintf(state.fd,"%s ", node->variable);
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
			fprintf(state.fd," = ");
			break;
		case SUM_ASSIGN_TYPE:
			fprintf(state.fd," += ");
			break;
		case SUB_ASSIGN_TYPE:
			fprintf(state.fd," -= ");
			break;
		case MULT_ASSIGN_TYPE:
			fprintf(state.fd," *= ");
			break;
		case DIV_ASSIGN_TYPE:
			fprintf(state.fd," /= ");
			break;
		case MOD_ASSIGN_TYPE:
			fprintf(state.fd," %%= ");
			break;
		default:
			break; // impossible
	}
}

void GenAssignmentNode(AssignmentNode * node, int isFor) {
	if(node->withType == withVar) {
		fprintf(state.fd,"%s", node->variable);
	} else if(node->withType == withArrayDeref) {
		GenArrayDerefNode(node->arrayDefinitionNode);
	} else {
		return; // error
	}
	GenAssignmentType(node->assignmentType);
	GenExpressionNode(node->expressionNode, isFor, NULL, NULL);
	//fprintf(state.fd,");");
}

void GenFunctionCallNode(FunctionCallNode * node, int isFor, char * arrayName, char * index) {
	fprintf(state.fd,"%s(", node->Variable);
	if(node->type == WithArgs) {
		GenFunctionCallArgNode(node->functionCallArgNode, 1, arrayName, index);
	}
	if(!isFor){
		fprintf(state.fd,");");
		
	} 
	else fprintf(state.fd,")");
}

void GenFunctionCallArgNode(FunctionCallArgNode * node, int isFor, char * arrayName, char * index) {
	GenExpressionNode(node->expressionNode, isFor, arrayName, index);
	if(node->type == FunctionCallWithArgs) {
		fprintf(state.fd,", ");
		GenFunctionCallArgNode(node->functionCallArgNode, isFor, arrayName, index);
	}
}

static void GenExpressionType(ExpressionNodeType type) {
	switch(type) {
		case AddOp:
			fprintf(state.fd," + ");
			break;
		case SubOp:
			fprintf(state.fd," - ");
			break;
		case MultOp:
			fprintf(state.fd," * ");
			break;
		case DivOp:
			fprintf(state.fd," / ");
			break;
		case ModOp:
			fprintf(state.fd," %% ");
			break;
		case IncOp:
			fprintf(state.fd,"++");
			break;
		case DecOp:
			fprintf(state.fd,"--");
			break;
		case BitNotOp:
			fprintf(state.fd,"~");
			break;
		case BitRightOp:
			fprintf(state.fd,">>");
			break;
		case BitLeftOp:
			fprintf(state.fd,"<<");
			break;
		case BitXorOp:
			fprintf(state.fd,"^");
			break;
		case BitOrOp:
			fprintf(state.fd,"|");
			break;
		case BitAndOp:
			fprintf(state.fd,"&");
			break;
		case AndOp:
			fprintf(state.fd," && ");
			break;
		case OrOp:
			fprintf(state.fd," || ");
			break;
		case NotOp:
			fprintf(state.fd,"!(");
			break;
		case EqOp:
			fprintf(state.fd," == ");
			break;
		case GrOp:
			fprintf(state.fd," > ");
			break;
		case GeOp:
			fprintf(state.fd," >= ");
			break;
		case LtOp:
			fprintf(state.fd," < ");
			break;
		case LeOp:
			fprintf(state.fd," <= ");
			break;
		case NeOp:
			fprintf(state.fd," != ");
			break;
		case WithParenthesis:
			fprintf(state.fd,"(");
			break;
		default:
			break;
	}
}

void GenExpressionNode(ExpressionNode * node, int isFor, char * variableName, char * index) {
	if(node->leftExpressionNode != NULL) {
		GenExpressionNode(node->leftExpressionNode, isFor, variableName, index);
	}
	GenExpressionType(node->op);
	if(node->rightExpressionNode != NULL) {
		GenExpressionNode(node->rightExpressionNode, isFor, variableName, index);
	}
	if(node->op == NotOp || node->op == WithParenthesis) {
		fprintf(state.fd,")");
	}
	if(node->rightExpressionNode == NULL && node->leftExpressionNode == NULL) {
		switch(node->op) {
			case VariableType:
				fprintf(state.fd,"%s", node->Variable);
				break;
			case NumConstantFloat:
				fprintf(state.fd,"%s", node->numConstantFloatNode);
				break;
			case NumConstantInt:
				fprintf(state.fd,"%d", node->numConstantIntNode);
				break;
			case specialVariable:
				fprintf(state.fd,"%s[%s]", variableName, index);
				break;
			case functionCall:
				GenFunctionCallNode(node->functionCallNode, isFor, variableName, index);
				break;
			case ArrayDeref:
				GenArrayDerefNode(node->arrayDerefNode);
				break;
			case String:
				fprintf(state.fd,"%s", node->StringNode);
				break;
			default:
				break; //impossible
		}
	}
	
}

void GenReturnStatementNode(ReturnStatementNode * node) {
	fprintf(state.fd,"return ");
	GenExpressionNode(node->expressionNode, 0, NULL, NULL);
	fprintf(state.fd,";");
	
}

void GenIfStatementNode(IfStatementNode * node) {
	fprintf(state.fd,"if (");
	GenExpressionNode(node->expressionNode, 0, NULL, NULL);
	fprintf(state.fd,") {");
	
	GenCodeBlockNode(node->codeBlockNode);
	fprintf(state.fd,"}");
	
}

void GenElseStatementNode(ElseStatementNode * node) {
	fprintf(state.fd,"else {");
	
	GenCodeBlockNode(node->codeBlockNode);
	fprintf(state.fd,"}");
	
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
	fprintf(state.fd,"while(");
	GenExpressionNode(node->expressionNode, 0, NULL, NULL);
	fprintf(state.fd,") {");
	
	GenCodeBlockNode(node->codeBlockNode);
	fprintf(state.fd,"}");
	
}

void GenForStatementNode(ForStatementNode * node) {
	fprintf(state.fd,"for (");
	GenDeclarationNode(node->declarationNode, 1);
	fprintf(state.fd,"; ");
	GenExpressionNode(node->firstExpressionNode, 1, NULL, NULL);
	fprintf(state.fd,"; ");
	if(node->type == withAssignment) {
		GenAssignmentNode(node->AssignmentNode, 1);
	}
	else {
		GenExpressionNode(node->expressionNode, 1, NULL, NULL);
	}
	fprintf(state.fd,") {");
	
	GenCodeBlockNode(node->codeBlockNode);
	fprintf(state.fd,"}");
	
}

void GenSwitchStatementNode(SwitchStatementNode * node) {
	fprintf(state.fd,"switch (");
	GenExpressionNode(node->expressionNode, 0, NULL, NULL);
	fprintf(state.fd,") {");
	
	GenCodeBlockNode(node->codeBlockNode);
	fprintf(state.fd,"");
	
	fprintf(state.fd,"}");
	
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
			if( node->expression){
				GenExpressionNode(node->expression, 0, NULL, NULL);
			}else if(node->expressionNode ){
				GenExpressionNode(node->expressionNode, 0, NULL, NULL);
			}
			if((node->expression && node->expression->op != functionCall) || (node->expressionNode && node->expressionNode->op != functionCall)){
				fprintf(state.fd,";");
				
			}
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
			fprintf(state.fd,";");
			
			break;
		case CaseStatement:
			fprintf(state.fd,"case ");
			GenExpressionNode(node->expression, 0, NULL, NULL);
			fprintf(state.fd,":");
			
			break;
		case ContinueStatement:
			fprintf(state.fd,"continue;");
			
			break;
		case DefaultCaseStatement:
			fprintf(state.fd,"default:");
			
			break;
		case BreakStatement:
			fprintf(state.fd,"break;");
			
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
	fprintf(state.fd,"%s", node->variable);
}

void GenFunctionArgsNode(FunctionArgsNode * node) {
	GenFunctionArgNode(node->functionArgNode);
	if(node->type == multiple) {
		fprintf(state.fd,", ");
		GenFunctionArgsNode(node->functionArgsNode);
	}
}

void GenFunctionDeclarationNode(FunctionDeclarationNode * node) {
	if(node->type == FunctionDeclarationNoArgs) {
		GenDataType(node->functionType);
		fprintf(state.fd,"%s() {", node->variable);
		
	} else if(node->type == FunctionDeclarationWithArgs) {
		GenDataType(node->functionType);
		fprintf(state.fd,"%s(", node->variable);
		GenFunctionArgsNode(node->functionArgs);
		fprintf(state.fd,") {");
		
	} else if(node->type == FunctionDeclarationNoArgsVoid) {
		fprintf(state.fd,"void %s() {", node->variable);
		
	} else {
		fprintf(state.fd,"void %s(", node->variable);
		GenFunctionArgsNode(node->functionArgs);
		fprintf(state.fd,") {");
		
	}
	GenCodeBlockNode(node->codeBlock);
	fprintf(state.fd,"}");
	
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

void GenProgramNode(ProgramNode * node) {
	GenStatementNode(node->statementNode);
}

void GenSpecialStatementNode(SpecialStatementNode * node) {
	GenSelectorNode(node->selectorNode);
}

void GenSelectorNode(SelectorNode * node) {
	switch(node->selectorNodeType) {
		case ReduceStatement:
			GenReduceStatementNode(node->reduceStatement);
			break;
		case FilterStatement:
			GenFilterStatementNode(node->filterStatement);
			break;
		case ForeachStatement:
			GenForeachStatementNode(node->foreachStatement);
			break;
		case MapStatement:
			GenMapStatementNode(node->mapStatement);
			break;
		case CreateStatement:
			GenCreateStatementNode(node->createStatement);
			break;
		case ReduceRangeStatement:
			GenReduceRangeStatementNode(node->reduceRangeStatement);
			break;
		case FilterRangeStatement:
			GenFilterRangeStatementNode(node->filterRangeStatement);
			break;
		case ForeachRangeStatement:
			GenForeachRangeStatementNode(node->foreachRangeStatement);
			break;
		case MapRangeStatement:
			GenMapRangeStatementNode(node->mapRangeStatement);
			break;
		default:
			break; //impossible
	}
}

void GenReduceStatementNode(ReduceStatementNode * node) {
	char * index = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = 0; %s < ", index, index);
	GenSizeNode(node->unboundedParametersNode->SizeNode, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	fprintf(state.fd,"%s = ", node->unboundedParametersNode->variable2);
	GenExpressionNode(node->lambda->expressionNode, 0, node->unboundedParametersNode->variable1, index);
	fprintf(state.fd,";");
	
	fprintf(state.fd,"}");
	
}
void GenReduceRangeStatementNode(ReduceRangeStatementNode * node) {
	char * index = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = ", index);
	GenSizeNode(node->boundedParametersNode->rangeNode->sizeNode1, 0);
	fprintf(state.fd,"; %s < ", index);
	GenSizeNode(node->boundedParametersNode->rangeNode->sizeNode2, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	fprintf(state.fd,"%s = ", node->boundedParametersNode->variable2);
	GenExpressionNode(node->lambda->expressionNode, 0, node->boundedParametersNode->variable1, index);
	fprintf(state.fd,";");
	
	fprintf(state.fd,"}");
	
}
void GenFilterStatementNode(FilterStatementNode * node) {
	char * index = generateNewIndex(state.list);
	char * index2 = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = 0, %s = 0; %s < ", index, index2, index);
	GenSizeNode(node->unboundedParametersNode->SizeNode, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	fprintf(state.fd,"if(");
	GenExpressionNode(node->lambda->expressionNode, 0, node->unboundedParametersNode->variable1, index);
	fprintf(state.fd,") {");
	
	fprintf(state.fd,"%s[%s++] = %s[%s];", node->unboundedParametersNode->variable2, index2, node->unboundedParametersNode->variable1, index);
	
	fprintf(state.fd,"}}");
	
}
void GenFilterRangeStatementNode(FilterRangeStatementNode * node) {
	char * index = generateNewIndex(state.list);
	char * index2 = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = ", index);
	GenSizeNode(node->boundedParametersNode->rangeNode->sizeNode1, 0);
	fprintf(state.fd,", int %s = 0; %s < ",index2, index);
	GenSizeNode(node->boundedParametersNode->rangeNode->sizeNode2, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	fprintf(state.fd,"if(");
	GenExpressionNode(node->lambda->expressionNode, 0, node->boundedParametersNode->variable1, index);
	fprintf(state.fd,") {");
	
	fprintf(state.fd,"%s[%s++] = %s[%s];", node->boundedParametersNode->variable2, index2, node->boundedParametersNode->variable1, index);
	
	fprintf(state.fd,"}}");
	
}
void GenForeachStatementNode(ForeachStatementNode * node) {
	char * index = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = 0 ; %s < ", index, index);
	GenSizeNode(node->sizeNode, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	GenFunctionCallNode(node->consumerFunctionNode->functionCallNode, 0, node->variable, index);
	fprintf(state.fd,"}");
	
}
void GenForeachRangeStatementNode(ForeachRangeStatementNode * node) {
	char * index = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = ", index);
	GenSizeNode(node->rangeNode->sizeNode1, 0);
	fprintf(state.fd," ; %s < ", index);
	GenSizeNode(node->rangeNode->sizeNode2, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	GenFunctionCallNode(node->consumerFunctionNode->functionCallNode, 0, node->variable, index);
	fprintf(state.fd,"}");
	
}
void GenMapStatementNode(MapStatementNode * node) {
	char * index = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = 0; %s < ", index, index);
	GenSizeNode(node->unboundedParametersNode->SizeNode, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	fprintf(state.fd,"%s[%s] = ", node->unboundedParametersNode->variable2, index);
	GenExpressionNode(node->lambda->expressionNode, 0, node->unboundedParametersNode->variable1, index);
	fprintf(state.fd,";");
	
	fprintf(state.fd,"}");
	
}
void GenMapRangeStatementNode(MapRangeStatementNode * node) {
	char * index = generateNewIndex(state.list);
	fprintf(state.fd,"for(int %s = ", index);
	GenSizeNode(node->boundedParametersNode->rangeNode->sizeNode1, 0);
	fprintf(state.fd,"; %s < ", index);
	GenSizeNode(node->boundedParametersNode->rangeNode->sizeNode2, 0);
	fprintf(state.fd," ; %s++) {", index);
	
	fprintf(state.fd,"%s[%s] = ", node->boundedParametersNode->variable2, index);
	GenExpressionNode(node->lambda->expressionNode, 0, node->boundedParametersNode->variable1, index);
	fprintf(state.fd,";");
	
	fprintf(state.fd,"}");
	
}
void GenCreateStatementNode(CreateStatementNode * node) {
	GenDataType(node->dataType);
	char * index = generateNewIndex(state.list);
	if( node->createLambda->isLower){
		fprintf(state.fd," %s[%d-%d+1];", node->variable1, node->createLambda->constant2, node->createLambda->constant1);
		fprintf(state.fd,"for(int %s = %d; %s <= %d ; %s++) {", index, node->createLambda->constant1, index, node->createLambda->constant2, index);
		fprintf(state.fd,"%s[%s-%d] = %s", node->variable1, index, node->createLambda->constant1, index);
	}else{
		fprintf(state.fd," %s[%d-%d+1];", node->variable1, node->createLambda->constant1, node->createLambda->constant2);
		fprintf(state.fd,"for(int %s = %d; %s >= %d ; %s--) {", index, node->createLambda->constant1, index, node->createLambda->constant2, index);
		fprintf(state.fd,"%s[%d-%s] = %s", node->variable1, node->createLambda->constant1, index, index);
	}
	
	fprintf(state.fd,";");
	
	fprintf(state.fd,"}");
	

}

