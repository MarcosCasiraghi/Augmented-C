#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include "abstract-syntax-tree.h"
#include <stdio.h>
#include <string.h>

/**
 * Implementación de "bison-grammar.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
	LogError("En ASCII es:");
	LogErrorRaw("\t");
	const int length = strlen(yytext);
	for (int i = 0; i < length; ++i) {
		LogErrorRaw("[%d]", yytext[i]);
	}
	LogErrorRaw("\n\n");
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
// int ProgramGrammarAction(const int value) {
// 	LogDebug("\tProgramGrammarAction(%d)", value);
// 	/*
// 	* "state" es una variable global que almacena el estado del compilador,
// 	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
// 	* es utilizada en la función "main".
// 	*/
// 	state.succeed = true;
// 	/*
// 	* Por otro lado, "result" contiene el resultado de aplicar el análisis
// 	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
// 	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
// 	* la expresión se computa on-the-fly, y es la razón por la cual esta
// 	* variable es un simple entero, en lugar de un nodo.
// 	*/
// 	state.result = value;
// 	return value;
// }

void ProgramGrammarAction() {
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
}

// int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tAdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Add(leftValue, rightValue);
// }

// int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tSubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Subtract(leftValue, rightValue);
// }

// int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tMultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Multiply(leftValue, rightValue);
// }

// int DivisionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tDivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Divide(leftValue, rightValue);
// }

// int FactorExpressionGrammarAction(const int value) {
// 	LogDebug("\tFactorExpressionGrammarAction(%d)", value);
// 	return value;
// }

// int ExpressionFactorGrammarAction(const int value) {
// 	LogDebug("\tExpressionFactorGrammarAction(%d)", value);
// 	return value;
// }

// int ConstantFactorGrammarAction(const int value) {
// 	LogDebug("\tConstantFactorGrammarAction(%d)", value);
// 	return value;
// }

// int IntegerConstantGrammarAction(const int value) {
// 	LogDebug("\tIntegerConstantGrammarAction(%d)", value);
// 	return value;
// }


// - - - - - - Function Declaration - - - - - -

FunctionDeclarationNode* FunctionDeclarationNoArgsAction(DataType functionType, Variable variable, CodeBlockNode* codeBlock) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = NoArgs;
    node->functionType = functionType;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = NULL;
    return node;
}

FunctionDeclarationNode* FunctionDeclarationWithArgs(DataType functionType, Variable variable, CodeBlockNode* codeBlock, FunctionCallArgNode* functionArgs) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = WithArgs;
    node->functionType = functionType;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = functionArgs;
    return node;
}

FunctionDeclarationNode* VoidFunctionDeclaration(Variable variable, CodeBlockNode* codeBlock) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = NoArgsVoid;
    node->functionType = NoArgsVoid;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = NULL;
    return node;
}

FunctionDeclarationNode* VoidFunctionDeclarationWithArgs(Variable variable, CodeBlockNode* codeBlock, FunctionCallArgNode* functionArgs) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = WithArgsVoid;
    node->functionType = WithArgsVoid;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = functionArgs;
    return node;
}


// - - - - - - Code Block - - - - - -


CodeBlockNode * DelcalartionCodeBlockAction(DeclarationNode * declarationNode){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = DeclarationStatement;
    node->declarationNode = declarationNode;
    return node;
}
CodeBlockNode * SpecialStatementCodeBlockAction(SpecialStatementNode * specialStatement){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = SpecialStatement;
    node->specialStatement = specialStatement;
    return node;
}
CodeBlockNode * ExpressionCodeBlockAction(ExpressionNode * expression){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = ExpressionStatement;
    node->expression = expression;
    return node;
}

CodeBlockNode * ReturnCodeBlockAction(ReturnStatementNode * returnStatementNode){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = ReturnStatement;
    node->returnStatement = returnStatementNode;
    return node;
}

CodeBlockNode * IfElseCodeBlockAction(IfElseStatementNode * ifElse){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = IfElseStatement;
    node->ifElse = ifElse;
    return node;
}
CodeBlockNode * ForCodeBlockAction(ForStatementNode * forStatement){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = ForStatement;
    node->ifElse = forStatement;
    return node;
}
CodeBlockNode * WhileCodeBlockAction(WhileStatementNode * whileStatement){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = WhileStatement;
    node->whileStatement = whileStatement;
    return node;
}
CodeBlockNode * SwitchCodeBlockAction(SwitchStatementNode * switchStatement){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = SwitchStatement;
    node->switchStatement = switchStatement;
    return node;
}
CodeBlockNode * AssingmentCodeBlockAction(AssigmentNode * assingment){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = AssignmentStatement;
    node->assingment = assingment;
    return node;
}
CodeBlockNode * CaseCodeBlockAction(ExpressionNode * expression, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = AssignmentStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * DefaultCaseCodeBlockAction(ExpressionNode * expression, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = DefaultCaseStatement;
    node->codeBlock = codeBlock;
    return node;
}

CodeBlockNode * DelcalartionCodeBlockActionWithChild(DeclarationNode * declarationNode, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = DeclarationStatement;
    node->declarationNode = declarationNode;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * SpecialStatementCodeBlockActionWithChild(SpecialStatementNode * specialStatement, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = SpecialStatement;
    node->specialStatement = specialStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * ExpressionCodeBlockActionWithChild(ExpressionNode * expression, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = ExpressionStatement;
    node->expression = expression;
    node->codeBlock = codeBlock;
    return node;
}

CodeBlockNode * ReturnCodeBlockActionWithChild(ReturnStatementNode * returnStatementNode, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = ReturnStatement;
    node->returnStatement = returnStatementNode;
    node->codeBlock = codeBlock;
    return node;
}

CodeBlockNode * IfElseCodeBlockActionWithChild(IfElseStatementNode * ifElse, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = IfElseStatement;
    node->ifElse = ifElse;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * ForCodeBlockActionWithChild(ForStatementNode * forStatement, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = ForStatement;
    node->ifElse = forStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * WhileCodeBlockActionWithChild(WhileStatementNode * whileStatement, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = WhileStatement;
    node->whileStatement = whileStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * SwitchCodeBlockActionWithChild(SwitchStatementNode * switchStatement, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = SwitchStatement;
    node->switchStatement = switchStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * AssingmentCodeBlockActionWithChild(AssigmentNode * assingment, CodeBlockNode * codeBlock){
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = AssignmentStatement;
    node->assingment = assingment;
    node->codeBlock = codeBlock;
    return node;
}

// - - - - - - Pointer - - - - - -

PointerNode * PointerAction(){
    PointerNode * node = malloc(sizeof(PointerNode));
    node->child = NoChild;

    return node;
}

PointerNode * PointerActionWithChild(PointerNode * pointerNode){
    PointerNode * node = malloc(sizeof(PointerNode));
    node->child = NoChild;
    node->pointerNode = pointerNode;
    return node;
}

// - - - - - - Declaration - - - - - -

DeclarationNode * DelcarationOfSingleAction(SingleDeclarationNode * singleDeclarationNode){
    DeclarationNode * node = malloc(sizeof(DeclarationNode));
    node->type = SingleDeclaration;
    node->singleDeclarationNode = singleDeclarationNode;
    return node;
}

DeclarationNode * DelcarationOfArrayAction(ArrayDeclarationNode * arrayDeclarationNode){
    DeclarationNode * node = malloc(sizeof(DeclarationNode));
    node->type = ArrayDeclaration;
    node->arrayDeclarationNode = arrayDeclarationNode;
    return node;
}


// - - - - - - Single Declaration - - - - - -

SingleDeclarationNode * SingleWithPointerDelcarationAction(PointerNode * pointer, DataType dataType, Variable * variable, SingleInitializeNode * singleInitializeNode){
    SingleDeclarationNode * node = malloc(sizeof(SingleDeclarationNode));
    node->type = SinleWithPointer;
    node->pointer = pointer;
    node->dataType = dataType;
    node->variable = variable;
    node->singleInitializeNode = singleInitializeNode;
    return node;
}


SingleDeclarationNode * SingleWithoutPointerDelcarationAction(DataType dataType, Variable * variable, SingleInitializeNode * singleInitializeNode){
    SingleDeclarationNode * node = malloc(sizeof(SingleDeclarationNode));
    node->type = SinleWithoutPointer;
    node->dataType = dataType;
    node->variable = variable;
    node->singleInitializeNode = singleInitializeNode;
    return node;
}

// - - - - - - Single Initialization - - - - - -

SingleInitializeNode * SingleInitializationWithoutAssignAction(){
    SingleInitializeNode * node = malloc(sizeof(SingleInitializeNode));
    node->type = NoAssign;
    return node;
}
SingleInitializeNode * SingleInitializationWithAssignAction(ExpressionNode * expressionNode){
    SingleInitializeNode * node = malloc(sizeof(SingleInitializeNode));
    node->type = AssignSingle;
    node->expressionNode = expressionNode;
    return node;
}


// - - - - - - Array Declaration - - - - - -

ArrayDeclarationNode * ArrayDeclarationAction(DataType dataType, Variable variable, ArraySizeNode * arraySizeNode, ArrayInitializeNode * arrayInitializeNode){
    ArrayDeclarationNode * node = malloc(sizeof(ArrayDeclarationNode));
    node->dataType = dataType;
    node->variable = variable;
    node->arraySizeNode = arraySizeNode;
    node->arrayInitializeNode;
    return node;
}


// - - - - - - Array Size - - - - - -

ArraySizeNode * ArraySizeWithoutSizeNorChildrenAction(){
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = NotSizedSingle;
    node->child = NoChild;
    return node;
}

ArraySizeNode * ArraySizeWithSizeWithoutChildrenAction(NumConstantIntNode * numberConstant){
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = Sized;
    node->child = NoChild;
    node->numberConstant = numberConstant;
    return node;
}

ArraySizeNode * ArraySizeWithoutSizeWithChildrenAction(ArraySizeNode * arraySizeNode){
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = NotSizedSingle;
    node->child = HasChild;
    node->arraySizeNode = arraySizeNode;
    return node;
}

ArraySizeNode * ArraySizeWithSizeWithChildrenAction(NumConstantIntNode * numberConstant, ArraySizeNode * arraySizeNode){
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = Sized;
    node->child = NoChild;
    node->numberConstant = numberConstant;
    node->arraySizeNode = arraySizeNode;
    return node;
}

// - - - - - - Array Initialize - - - - - -

ArrayInitializeNode * ArrayInitializeEmptyAction(){
    ArrayInitializeNode * node = malloc(sizeof(ArrayInitializeNode));
    node->type = Empty;
    return node;
}
ArrayInitializeNode * ArrayInitializeWithListAction(ArrayListNode * arrayListNode){
    ArrayInitializeNode * node = malloc(sizeof(ArrayInitializeNode));
    node->type = WithList;
    node->arrayListNode = arrayListNode;

    return node;
}

// - - - - - - Array Initialize List - - - - - -

ArrayListNode * ArrayListAction(NumConstantIntNode integer){
    ArrayListNode * node = malloc(sizeof(ArrayListNode));
    node->child = NoChild;
    node->integer = integer;
    return node;
}
ArrayListNode * ArrayListManyAction(NumConstantIntNode integer, ArrayListNode * arrayListNode){
    ArrayListNode * node = malloc(sizeof(ArrayListNode));
    node->child = HasChild;
    node->integer = integer;
    node->arrayListNode = arrayListNode;
    return node;
}


AssigmentNode * AssignmentWithArrayDerefGrammarAction(ArrayDerefNode * arrayDerefNode, AssignmentType type, ExpressionNode * expressionNode ){
	AssigmentNode * assigmentNode = malloc(sizeof(AssigmentNode));
	assigmentNode->withType = withArrayDeref;
	assigmentNode->assignmentType = type;
	assigmentNode->variable = NULL;
	assigmentNode->expressionNode = expressionNode;
	assigmentNode->arrayDefinitionNode = arrayDerefNode;

	return assigmentNode;

}

AssigmentNode * AssignmentWithVarGrammarAction(Variable * var, AssignmentType type, ExpressionNode * expressionNode ){
	AssigmentNode * assigmentNode = malloc(sizeof(AssigmentNode));
	assigmentNode->withType = withVar;
	assigmentNode->assignmentType = type;
	assigmentNode->variable = var;
	assigmentNode->expressionNode = expressionNode;
	assigmentNode->arrayDefinitionNode = NULL;

	return assigmentNode;
}

ArrayDerefNode * ArrayDerefGrammarAction(Variable * var, SizeNode * sizeNode){
	ArrayDerefNode * arrayDerefNode = malloc(sizeof(ArrayDerefNode));
	arrayDerefNode->variable = var;
	arrayDerefNode->sizeNode = sizeNode;

	return arrayDerefNode;
}

ReturnStatementNode * ReturnStatementGrammarAction(ExpressionNode * expressionNode){
	ReturnStatementNode * returnStatementNode = malloc(sizeof(ReturnStatementNode));
	returnStatementNode->expressionNode = expressionNode;

	return returnStatementNode;
}

IfElseStatementNode * IfWithoutElseStatementGrammarAction(IfStatementNode * ifStatementNode){
	IfElseStatementNode * ifElseStatementNode = malloc(sizeof(IfElseStatementNode));
	ifElseStatementNode->type = withoutElse;
	ifElseStatementNode->ifStatementNode = ifStatementNode;
	ifElseStatementNode->elseStatementNode = NULL;

	return ifElseStatementNode;
}

IfElseStatementNode * IfWithElseStatementGrammarAction(IfStatementNode * ifStatementNode, ElseStatementNode * elseStatementNode){
	IfElseStatementNode * ifElseStatementNode = malloc(sizeof(IfElseStatementNode));
	ifElseStatementNode->type = withElse;
	ifElseStatementNode->ifStatementNode = ifStatementNode;
	ifElseStatementNode->elseStatementNode = elseStatementNode;

	return ifElseStatementNode;
}

IfStatementNode * IfStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode){
	IfStatementNode * ifStatementNode = malloc(sizeof(IfStatementNode));
	ifStatementNode->expressionNode = expressionNode;
	ifStatementNode->codeBlockNode = codeBlockNode;

	return ifStatementNode;
}

ElseStatementNode * ElseStatementGrammarAction(CodeBlockNode * codeBlockNode){
	ElseStatementNode * elseStatementNode = malloc(sizeof(ElseStatementNode));
	elseStatementNode->codeBlockNode = codeBlockNode;

	return elseStatementNode;
}

WhileStatementNode * WhileStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode){
	WhileStatementNode * whileStatementNode = malloc(sizeof(WhileStatementNode));
	whileStatementNode->expressionNode = expressionNode;
	whileStatementNode->codeBlockNode = codeBlockNode;

	return whileStatementNode;
}

ForStatementNode * ForStatementWithAssigmentGrammarAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, AssigmentNode * assignmentNode, CodeBlockNode * codeBlockNode ){
	ForStatementNode * forStatementNode = malloc(sizeof(ForStatementNode));
	forStatementNode->type = withExpression;
	forStatementNode->declarationNode = declarationNode;
	forStatementNode->firstExpressionNode = firstExpressionNode;
	forStatementNode->expressionNode = NULL;
	forStatementNode->codeBlockNode = codeBlockNode;
	forStatementNode->assigmentNode = assignmentNode;

	return forStatementNode;
}

ForStatementNode * ForStatementWithExpressionGrammarAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, ExpressionNode * secondExpressionNode, CodeBlockNode * codeBlockNode ){
	ForStatementNode * forStatementNode = malloc(sizeof(ForStatementNode));
	forStatementNode->type = withExpression;
	forStatementNode->declarationNode = declarationNode;
	forStatementNode->firstExpressionNode = firstExpressionNode;
	forStatementNode->expressionNode = secondExpressionNode;
	forStatementNode->codeBlockNode = codeBlockNode;
	forStatementNode->assigmentNode = NULL;

	return forStatementNode;
}

SwitchStatementNode * SwitchStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode){
	SwitchStatementNode * switchStatementNode = malloc(sizeof(SwitchStatementNode));
	switchStatementNode->expressionNode = expressionNode;
	switchStatementNode->codeBlockNode = codeBlockNode;

	return switchStatementNode;
}

SizeNode * SizeNumConstIntGrammarAction(NumConstantIntNode * numConstantIntNode){
	SizeNode * sizeNode = malloc(sizeof(SizeNode));
	sizeNode->type = NumConstantInt;
	sizeNode->numConstantIntNode = numConstantIntNode;
	sizeNode->variable = NULL;

	return sizeNode;
}

SizeNode * SizeVarGrammarAction(Variable * variableNode){
	SizeNode * sizeNode = malloc(sizeof(SizeNode));
	sizeNode->type = variable;
	sizeNode->numConstantIntNode = NULL;
	sizeNode->variable = variableNode;

	return sizeNode;
}

ExpressionNode * AddOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = AddOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * SubOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = SubOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * MultOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = MultOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * DivOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = DivOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * ModOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = ModOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * IncOpRightExpressionGrammarAction(ExpressionNode * leftExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = IncOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * IncOpLeftExpressionGrammarAction(ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = IncOp;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * DecOpRightExpressionGrammarAction(ExpressionNode * leftExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = DecOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * DecOpLeftExpressionGrammarAction(ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = DecOp;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * BitNotOpExpressionGrammarAction(ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = BitNotOp;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * BitRightOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = BitRightOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * BitLeftOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = BitLeftOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * BitXorOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = BitXorOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * BitOrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = BitOrOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * BitAndOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = BitAndOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * AndOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = AndOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * ParenthesisExpressionGrammarAction(ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = WithParenthesis;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * OrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = OrOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * NotOpExpressionGrammarAction(ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = NotOp;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * EqOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = EqOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * GrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = GrOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * GeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = GeOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * LtOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = LtOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * LeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = LeOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * NeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = NeOp;
	expressionNode->leftExpressionNode = leftExpressionNode;
	expressionNode->rightExpressionNode = rightExpressionNode;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * variableOpExpressionGrammarAction(Variable * variable){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = variable;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = variable;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * NumConstantFloatOpExpressionGrammarAction(NumConstantFloatNode * numConstantFloatNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = NumConstantFloat;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = numConstantFloatNode;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * NumConstantIntOpExpressionGrammarAction(NumConstantIntNode * numConstantIntNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = NumConstantInt;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = numConstantIntNode;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * SpecialVarOpExpressionGrammarAction(SpecialVariable * specialVariableNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = specialVariable;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = specialVariableNode;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * FunctionCallOpExpressionGrammarAction(FunctionCallNode * functionCallNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = functionCall;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = functionCallNode;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * ArrayDerefOpExpressionGrammarAction(ArrayDerefNode * arrayDerefNode){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = ArrayDeref;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = arrayDerefNode;
	expressionNode->StringNode = NULL;

	return  expressionNode;
}

ExpressionNode * StringOpExpressionGrammarAction(StringVar * stringVar){
	ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
	expressionNode->op = String;
	expressionNode->leftExpressionNode = NULL;
	expressionNode->rightExpressionNode = NULL;
	expressionNode->Variable = NULL;
	expressionNode->numConstantIntNode = NULL;
	expressionNode->numConstantFloatNode = NULL;
	expressionNode->specialVariable = NULL;
	expressionNode->functionCallNode = NULL;
	expressionNode->arrayDerefNode = NULL;
	expressionNode->StringNode = stringVar;

	return  expressionNode;
}



FunctionCallNode * WithArgsFunctionCallGrammarAction(Variable * variable, FunctionCallArgNode * functionCallArgNode){
	FunctionCallNode * functionCallNode = malloc(sizeof(FunctionCallNode));
	functionCallNode->type = WithArgs;
	functionCallNode->Variable = variable;
	functionCallNode->functionCallArgNode = functionCallArgNode;

	return functionCallNode;
}

FunctionCallNode * NoArgsFunctionCallGrammarAction(Variable * variable){
	FunctionCallNode * functionCallNode = malloc(sizeof(FunctionCallNode));
	functionCallNode->type = NoArgs;
	functionCallNode->Variable = variable;
	functionCallNode->functionCallArgNode = NULL;

	return functionCallNode;
}

FunctionCallArgNode * WithArgsFunctionCallArgGrammarAction(ExpressionNode * expressionNode, FunctionCallArgNode * functionCallArgNode){
	FunctionCallArgNode * functionCallArgNode = malloc(sizeof(FunctionCallArgNode));
	functionCallArgNode->type = WithArgs;
	functionCallArgNode->expressionNode = expressionNode;
	functionCallArgNode->functionCallArgNode = functionCallArgNode;

	return functionCallArgNode;
}

FunctionCallArgNode * NoArgsFunctionCallArgGrammarAction(ExpressionNode * expressionNode){
	FunctionCallArgNode * functionCallArgNode = malloc(sizeof(FunctionCallArgNode));
	functionCallArgNode->type = NoArgs;
	functionCallArgNode->expressionNode = expressionNode;
	functionCallArgNode->functionCallArgNode = NULL;

	return functionCallArgNode;
}


