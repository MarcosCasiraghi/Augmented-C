#include "../../backend/semantic-analysis/symbol-list.h"
#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Implementación de "bison-.h".
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
// int ProgramAction(const int value) {
//  LogDebug("\tProgramAction(%d)", value);
//  /*
//  * "state" es una variable global que almacena el estado del compilador,
//  * cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
//  * es utilizada en la función "main".
//  */
//  state.succeed = true;
//  /*
//  * Por otro lado, "result" contiene el resultado de aplicar el análisis
//  * sintáctico mediante Bison, y almacenar el nood raíz del AST construido
//  * en esta variable. Para el ejemplo de la calculadora, no hay AST porque
//  * la expresión se computa on-the-fly, y es la razón por la cual esta
//  * variable es un simple entero, en lugar de un nodo.
//  */
//  state.result = value;
//  return value;
// }

ProgramNode * ProgramAction(StatementNode * statement) {
    ProgramNode * node = malloc(sizeof(ProgramNode));
    node->statementNode = statement;
    // state.succeed = true;
    state.program = node;
    return node;
}

// - - - - - - Add to symbol list - - - - - 

void addToSymbolList(DataType dataType, Variable variable, bool is_pointer, bool is_array, bool is_function){
    symbol_node * list_node = malloc(sizeof(symbol_node));
    list_node->type = dataType;
    list_node->name = variable;
    list_node->is_pointer = is_pointer;
    list_node->is_array = is_array;
    list_node->is_function = is_function;
    list_node->next = NULL;
    add_symbol(state.list, list_node);
}

// - - - - - - Includes - - - - - - - - - -

MetaCommandNode * StringMetaCommandAction(StringVar string) {
    MetaCommandNode * node = malloc(sizeof(MetaCommandNode));
    node->type = MetaCommandString;
    node->string = string;
    return node;
}

MetaCommandNode * FileNameMetaCommandAction(StringVar fileName) {
    MetaCommandNode * node = malloc(sizeof(MetaCommandNode));
    node->type = MetaCommandFileName;
    node->string = fileName;
    return node;
}

// - - - - - - Dereferencing  - - - - - -

SizeNode * SizeNumConstIntAction(NumConstantIntNode numConstantIntNode){
    SizeNode * sizeNode = malloc(sizeof(SizeNode));
    sizeNode->type = ConstantSize;
    sizeNode->numConstantIntNode = numConstantIntNode;
    sizeNode->variable = NULL;

    return sizeNode;
}

SizeNode * SizeVarAction(Variable variableNode){
    SizeNode * sizeNode = malloc(sizeof(SizeNode));
    sizeNode->type = VariableSize;
    sizeNode->numConstantIntNode = 0;
    sizeNode->variable = variableNode;

      //chequeo si existen variable 1 y 2
    if( !contains_symbol(state.list, variableNode, false) ){
        state.succeed = false;
        //TODO - handle error
        printf("variable: %s not declared\n", variableNode );
    }

    return sizeNode;
}

ArrayDerefNode * ArrayDerefAction(Variable var, SizeNode * sizeNode){
    ArrayDerefNode * arrayDerefNode = malloc(sizeof(ArrayDerefNode));
    arrayDerefNode->variable = var;
    arrayDerefNode->sizeNode = sizeNode;

    return arrayDerefNode;
}

// - - - - - - Pointer - - - - - -

PointerNode * PointerAction() {
    PointerNode * node = malloc(sizeof(PointerNode));
    node->child = NoChild;

    return node;
}

PointerNode * PointerActionWithChild(PointerNode * pointerNode) {
    PointerNode * node = malloc(sizeof(PointerNode));
    node->child = HasChild;
    node->pointerNode = pointerNode;
    return node;
}

// - - - - - - Declaration - - - - - -

DeclarationNode * DeclarationOfSingleAction(SingleDeclarationNode * singleDeclarationNode) {
    DeclarationNode * node = malloc(sizeof(DeclarationNode));
    node->type = SingleDeclaration;
    node->singleDeclarationNode = singleDeclarationNode;
	node->arrayDeclarationNode = NULL;
    return node;
}

DeclarationNode * DeclarationOfArrayAction(ArrayDeclarationNode * arrayDeclarationNode) {
    DeclarationNode * node = malloc(sizeof(DeclarationNode));
    node->type = ArrayDeclaration;
	node->singleDeclarationNode = NULL;
    node->arrayDeclarationNode = arrayDeclarationNode;
    return node;
}


// - - - - - - Single Declaration - - - - - -

SingleDeclarationNode * SingleWithPointerDeclarationAction(PointerNode * pointer, DataType dataType, Variable variable, SingleInitializeNode * singleInitializeNode) {
    SingleDeclarationNode * node = malloc(sizeof(SingleDeclarationNode));
    node->type = SingleWithPointer;
    node->pointer = pointer;
    node->dataType = dataType;
    node->variable = variable;
    node->singleInitializeNode = singleInitializeNode;

    //se agrega a la tabla de simbolos
    addToSymbolList(dataType, variable, true, false, false);
    
    return node;
}


SingleDeclarationNode * SingleWithoutPointerDeclarationAction(DataType dataType, Variable variable, SingleInitializeNode * singleInitializeNode) {
    SingleDeclarationNode * node = malloc(sizeof(SingleDeclarationNode));
    node->type = SingleWithoutPointer;
    node->dataType = dataType;
    node->variable = variable;
    node->singleInitializeNode = singleInitializeNode;

    //se agrega a la tabla de simbolos
    addToSymbolList(dataType, variable, false, false, false);

    return node;
}

// - - - - - - Single Initialization - - - - - -

SingleInitializeNode * SingleInitializationWithoutAssignAction() {
    SingleInitializeNode * node = malloc(sizeof(SingleInitializeNode));
    node->type = NoAssign;
    return node;
}
SingleInitializeNode * SingleInitializationWithAssignAction(ExpressionNode * expressionNode) {
    SingleInitializeNode * node = malloc(sizeof(SingleInitializeNode));
    node->type = AssignSingle;
    node->expressionNode = expressionNode;
    return node;
}

// - - - - - - Array Declaration - - - - - -

ArrayDeclarationNode * ArrayDeclarationAction(DataType dataType, Variable variable, ArraySizeNode * arraySizeNode, ArrayInitializeNode * arrayInitializeNode) {
    ArrayDeclarationNode * node = malloc(sizeof(ArrayDeclarationNode));
    node->dataType = dataType;
    node->variable = variable;
    node->arraySizeNode = arraySizeNode;
    node->arrayInitializeNode = arrayInitializeNode;

    //se agrega a la tabla de simbolos
    addToSymbolList(dataType, variable, false, true, false);

    return node;
}

// - - - - - - Array Size - - - - - -

ArraySizeNode * ArraySizeWithoutSizeNorChildrenAction() {
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = NotSizedSingle;
    node->child = NoChild;
    return node;
}

ArraySizeNode * ArraySizeWithSizeWithoutChildrenAction(NumConstantIntNode numberConstant) {
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = Sized;
    node->child = NoChild;
    node->numberConstant = numberConstant;
    return node;
}

ArraySizeNode * ArraySizeWithoutSizeWithChildrenAction(ArraySizeNode * arraySizeNode) {
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = NotSizedSingle;
    node->child = HasChild;
    node->arraySizeNode = arraySizeNode;
    return node;
}

ArraySizeNode * ArraySizeWithSizeWithChildrenAction(NumConstantIntNode numberConstant, ArraySizeNode * arraySizeNode) {
    ArraySizeNode * node = malloc(sizeof(ArraySizeNode));
    node->type = Sized;
    node->child = NoChild;
    node->numberConstant = numberConstant;
    node->arraySizeNode = arraySizeNode;
    return node;
}

// - - - - - - Assignment Type  - - - - - -

AssignmentType AssignAction() {
    return ASSIGN_TYPE;
}
AssignmentType SumAssignAction() {
    return SUM_ASSIGN_TYPE;
}
AssignmentType SubAssignAction() {
    return SUB_ASSIGN_TYPE;
}
AssignmentType MultAssignAction() {
    return MULT_ASSIGN_TYPE;
}
AssignmentType DivAssignAction() {
    return DIV_ASSIGN_TYPE;
}
AssignmentType ModAssignAction() {
    return MOD_ASSIGN_TYPE;
}

// - - - - - - - Data Type - - - - - - - -
DataType IntAction() {
    return Int;
}
DataType FloatAction() {
    return Float;
}
DataType DoubleAction() {
    return Double;
}
DataType LongAction() {
    return Long;
}
DataType ShortAction() {
    return Short;
}
DataType CharAction() {
    return Char;
}
DataType VoidPointerAction() {
    return VoidPointer;
}

 
// - - - - - - Array Initialize - - - - - -

ArrayInitializeNode * ArrayInitializeEmptyAction() {
    ArrayInitializeNode * node = malloc(sizeof(ArrayInitializeNode));
    node->type = Empty;
    return node;
}
ArrayInitializeNode * ArrayInitializeWithListAction(ArrayListNode * arrayListNode) {
    ArrayInitializeNode * node = malloc(sizeof(ArrayInitializeNode));
    node->type = WithList;
    node->arrayListNode = arrayListNode;

    return node;
}

// - - - - - - Array Initialize List - - - - - -

ArrayListNode * ArrayListAction(NumConstantIntNode integer) {
    ArrayListNode * node = malloc(sizeof(ArrayListNode));
    node->child = NoChild;
    node->integer = integer;
    return node;
}
ArrayListNode * ArrayListManyAction(NumConstantIntNode integer, ArrayListNode * arrayListNode) {
    ArrayListNode * node = malloc(sizeof(ArrayListNode));
    node->child = HasChild;
    node->integer = integer;
    node->arrayListNode = arrayListNode;
    return node;
}

// - - - - - - Assignment - - - - - -

AssignmentNode * AssignmentWithVarAction(Variable var, AssignmentType type, ExpressionNode * expressionNode ) {
    AssignmentNode * assignmentNode = malloc(sizeof(AssignmentNode));
    assignmentNode->withType = withVar;
    assignmentNode->assignmentType = type;
    assignmentNode->variable = var;
    assignmentNode->expressionNode = expressionNode;
    assignmentNode->arrayDefinitionNode = NULL;

    return assignmentNode;
}

AssignmentNode * AssignmentWithArrayDerefAction(ArrayDerefNode * arrayDerefNode, AssignmentType type, ExpressionNode * expressionNode ) {
    AssignmentNode * assignmentNode = malloc(sizeof(AssignmentNode));
    assignmentNode->withType = withArrayDeref;
    assignmentNode->assignmentType = type;
    assignmentNode->variable = NULL;
    assignmentNode->expressionNode = expressionNode;
    assignmentNode->arrayDefinitionNode = arrayDerefNode;

    return assignmentNode;
}

// - - - - - - Function Call - - - - - - - - - -

FunctionCallNode * WithArgsFunctionCallAction(Variable variable, FunctionCallArgNode * functionCallArgNode) {
    FunctionCallNode * functionCallNode = malloc(sizeof(FunctionCallNode));
    functionCallNode->type = WithArgs;
    functionCallNode->Variable = variable;
    functionCallNode->functionCallArgNode = functionCallArgNode;

    //chequeo si existe funcion
    // if( !contains_symbol(state.list, variable) ){
    //     //TODO - handle error
    //     printf("function: %s not declared\n", variable );
    // }

    return functionCallNode;
}

FunctionCallNode * NoArgsFunctionCallAction(Variable variable){
    FunctionCallNode * functionCallNode = malloc(sizeof(FunctionCallNode));
    functionCallNode->type = NoArgs;
    functionCallNode->Variable = variable;
    functionCallNode->functionCallArgNode = NULL;

    //chequeo si existe funcion
    // if( !contains_symbol(state.list, variable) ){
    //     //TODO - handle error
    //     //tal vez no hacer este chequeo porque se podrian llegar a llamar
    //     //funciones de otro lado - como printf
    //     printf("function: %s not declared\n", variable );
    // }

    return functionCallNode;
}

FunctionCallArgNode * WithArgsFunctionCallArgAction(ExpressionNode * expressionNode, FunctionCallArgNode * functionCallArgNode){
    FunctionCallArgNode * node = malloc(sizeof(FunctionCallArgNode));
    node->type = FunctionCallWithArgs;
    node->expressionNode = expressionNode;
    node->functionCallArgNode = functionCallArgNode;

    return node;
}

FunctionCallArgNode * NoArgsFunctionCallArgAction(ExpressionNode * expressionNode){
    FunctionCallArgNode * functionCallArgNode = malloc(sizeof(FunctionCallArgNode));
    functionCallArgNode->type = FunctionCallWithNoArgs;
    functionCallArgNode->expressionNode = expressionNode;
    functionCallArgNode->functionCallArgNode = NULL;

    return functionCallArgNode;
}

// - - - - - - Expression - - - - - -

ExpressionNode * AddOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = AddOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * SubOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = SubOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * MultOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = MultOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * DivOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = DivOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * ModOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = ModOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * IncOpRightExpressionAction(ExpressionNode * leftExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = IncOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * IncOpLeftExpressionAction(ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = IncOp;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * DecOpRightExpressionAction(ExpressionNode * leftExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = DecOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * DecOpLeftExpressionAction(ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = DecOp;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * BitNotOpExpressionAction(ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = BitNotOp;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * BitRightOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = BitRightOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * BitLeftOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = BitLeftOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * BitXorOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = BitXorOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * BitOrOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = BitOrOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * BitAndOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = BitAndOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * AndOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = AndOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * ParenthesisExpressionAction(ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = WithParenthesis;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * OrOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = OrOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * NotOpExpressionAction(ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = NotOp;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * EqOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = EqOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * GrOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = GrOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * GeOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = GeOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * LtOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = LtOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * LeOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = LeOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * NeOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = NeOp;
    expressionNode->leftExpressionNode = leftExpressionNode;
    expressionNode->rightExpressionNode = rightExpressionNode;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;

    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * variableOpExpressionAction(Variable variable){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = VariableType;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = variable;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;

    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * NumConstantFloatOpExpressionAction(NumConstantFloatNode numConstantFloatNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = NumConstantFloat;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = numConstantFloatNode;

    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * NumConstantIntOpExpressionAction(NumConstantIntNode numConstantIntNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = NumConstantInt;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = numConstantIntNode;
    expressionNode->numConstantFloatNode = NULL;

    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * SpecialVarOpExpressionAction(SpecialVariable specialVariableNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = specialVariable;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;

    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * FunctionCallOpExpressionAction(FunctionCallNode * functionCallNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = functionCall;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = functionCallNode;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * ArrayDerefOpExpressionAction(ArrayDerefNode * arrayDerefNode){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = ArrayDeref;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = arrayDerefNode;
    expressionNode->StringNode = NULL;

    return  expressionNode;
}

ExpressionNode * StringOpExpressionAction(StringVar stringVar){
    ExpressionNode * expressionNode = malloc(sizeof(ExpressionNode));
    expressionNode->op = String;
    expressionNode->leftExpressionNode = NULL;
    expressionNode->rightExpressionNode = NULL;
    expressionNode->Variable = NULL;
    expressionNode->numConstantIntNode = 0;
    expressionNode->numConstantFloatNode = NULL;
 
    expressionNode->functionCallNode = NULL;
    expressionNode->arrayDerefNode = NULL;
    expressionNode->StringNode = stringVar;

    return  expressionNode;
}

// - - - - - - Scope Code  - - - - - -

ReturnStatementNode * ReturnStatementAction(ExpressionNode * expressionNode){
    ReturnStatementNode * returnStatementNode = malloc(sizeof(ReturnStatementNode));
    returnStatementNode->expressionNode = expressionNode;

    return returnStatementNode;
}

IfElseStatementNode * IfWithoutElseStatementAction(IfStatementNode * ifStatementNode){
    IfElseStatementNode * ifElseStatementNode = malloc(sizeof(IfElseStatementNode));
    ifElseStatementNode->type = withoutElse;
    ifElseStatementNode->ifStatementNode = ifStatementNode;
    ifElseStatementNode->elseStatementNode = NULL;

    return ifElseStatementNode;
}

IfElseStatementNode * IfWithElseStatementAction(IfStatementNode * ifStatementNode, ElseStatementNode * elseStatementNode){
    IfElseStatementNode * ifElseStatementNode = malloc(sizeof(IfElseStatementNode));
    ifElseStatementNode->type = withElse;
    ifElseStatementNode->ifStatementNode = ifStatementNode;
    ifElseStatementNode->elseStatementNode = elseStatementNode;

    return ifElseStatementNode;
}

IfStatementNode * IfStatementAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode){
    IfStatementNode * ifStatementNode = malloc(sizeof(IfStatementNode));
    ifStatementNode->expressionNode = expressionNode;
    ifStatementNode->codeBlockNode = codeBlockNode;

    return ifStatementNode;
}

ElseStatementNode * ElseStatementAction(CodeBlockNode * codeBlockNode){
    ElseStatementNode * elseStatementNode = malloc(sizeof(ElseStatementNode));
    elseStatementNode->codeBlockNode = codeBlockNode;

    return elseStatementNode;
}

WhileStatementNode * WhileStatementAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode){
    WhileStatementNode * whileStatementNode = malloc(sizeof(WhileStatementNode));
    whileStatementNode->expressionNode = expressionNode;
    whileStatementNode->codeBlockNode = codeBlockNode;

    return whileStatementNode;
}

ForStatementNode * ForStatementWithAssigmentAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, AssignmentNode * assignmentNode, CodeBlockNode * codeBlockNode ){
    ForStatementNode * forStatementNode = malloc(sizeof(ForStatementNode));
    forStatementNode->type = withAssignment;
    forStatementNode->declarationNode = declarationNode;
    forStatementNode->firstExpressionNode = firstExpressionNode;
    forStatementNode->expressionNode = NULL;
    forStatementNode->codeBlockNode = codeBlockNode;
    forStatementNode->AssignmentNode = assignmentNode;

    return forStatementNode;
}

ForStatementNode * ForStatementWithExpressionAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, ExpressionNode * secondExpressionNode, CodeBlockNode * codeBlockNode ){
    ForStatementNode * forStatementNode = malloc(sizeof(ForStatementNode));
    forStatementNode->type = withExpression;
    forStatementNode->declarationNode = declarationNode;
    forStatementNode->firstExpressionNode = firstExpressionNode;
    forStatementNode->expressionNode = secondExpressionNode;
    forStatementNode->codeBlockNode = codeBlockNode;
    forStatementNode->AssignmentNode = NULL;

    return forStatementNode;
}

SwitchStatementNode * SwitchStatementAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode){
    SwitchStatementNode * switchStatementNode = malloc(sizeof(SwitchStatementNode));
    switchStatementNode->expressionNode = expressionNode;
    switchStatementNode->codeBlockNode = codeBlockNode;

    return switchStatementNode;
}

// - - - - - - Code Block - - - - - -


CodeBlockNode * DeclarationCodeBlockAction(DeclarationNode * declarationNode) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = DeclarationStatement;
    node->declarationNode = declarationNode;
    return node;
}
CodeBlockNode * SpecialStatementCodeBlockAction(SpecialStatementNode * specialStatement) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = SpecialStatement;
    node->specialStatement = specialStatement;
    return node;
}
CodeBlockNode * ExpressionCodeBlockAction(ExpressionNode * expression) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = ExpressionStatement;
    node->expressionNode = expression;
    return node;
}

CodeBlockNode * ReturnCodeBlockAction(ReturnStatementNode * returnStatementNode) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = ReturnStatement;
    node->returnStatement = returnStatementNode;
    return node;
}

CodeBlockNode * IfElseCodeBlockAction(IfElseStatementNode * ifElse) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = IfElseStatement;
    node->ifElse = ifElse;
    return node;
}
CodeBlockNode * ForCodeBlockAction(ForStatementNode * forStatement) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = ForStatement;
    node->forStatement = forStatement;
    return node;
}
CodeBlockNode * WhileCodeBlockAction(WhileStatementNode * whileStatement) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = WhileStatement;
    node->whileStatement = whileStatement;
    return node;
}
CodeBlockNode * SwitchCodeBlockAction(SwitchStatementNode * switchStatement) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = SwitchStatement;
    node->switchStatement = switchStatement;
    return node;
}
CodeBlockNode * AssignmentCodeBlockAction(AssignmentNode * assingment) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = AssignmentStatement;
    node->assingment = assingment;
    return node;
}
CodeBlockNode * ContinueCodeBlockAction(CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = ContinueStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * BreakCodeBlockAction(CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = BreakStatement;
    node->codeBlock = codeBlock;
    return node;
}

CodeBlockNode * ContinueAction() {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = ContinueStatement;
    node->codeBlock = NULL;
    return node;
}
CodeBlockNode * BreakAction() {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = NoChild;
    node->type = BreakStatement;
    node->codeBlock = NULL;
    return node;
}
CodeBlockNode * CaseCodeBlockAction(ExpressionNode * expression, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = CaseStatement;
    node->expression = expression;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * DefaultCaseCodeBlockAction(CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = DefaultCaseStatement;
    node->expression = NULL;
    node->codeBlock = codeBlock;
    return node;
}

CodeBlockNode * DeclarationCodeBlockActionWithChild(DeclarationNode * declarationNode, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = DeclarationStatement;
    node->declarationNode = declarationNode;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * SpecialStatementCodeBlockActionWithChild(SpecialStatementNode * specialStatement, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = SpecialStatement;
    node->specialStatement = specialStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * ExpressionCodeBlockActionWithChild(ExpressionNode * expression, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = ExpressionStatement;
    node->expression = expression;
    node->codeBlock = codeBlock;
    return node;
}

CodeBlockNode * ReturnCodeBlockActionWithChild(ReturnStatementNode * returnStatementNode, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = ReturnStatement;
    node->returnStatement = returnStatementNode;
    node->codeBlock = codeBlock;
    return node;
}

CodeBlockNode * IfElseCodeBlockActionWithChild(IfElseStatementNode * ifElse, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = IfElseStatement;
    node->ifElse = ifElse;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * ForCodeBlockActionWithChild(ForStatementNode * forStatement, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = ForStatement;
    node->forStatement = forStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * WhileCodeBlockActionWithChild(WhileStatementNode * whileStatement, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = WhileStatement;
    node->whileStatement = whileStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * SwitchCodeBlockActionWithChild(SwitchStatementNode * switchStatement, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = SwitchStatement;
    node->switchStatement = switchStatement;
    node->codeBlock = codeBlock;
    return node;
}
CodeBlockNode * AssignmentCodeBlockActionWithChild(AssignmentNode * assingment, CodeBlockNode * codeBlock) {
    CodeBlockNode * node = malloc(sizeof(CodeBlockNode));
    node->child = HasChild;
    node->type = AssignmentStatement;
    node->assingment = assingment;
    node->codeBlock = codeBlock;
    return node;
}

// - - - - - - Function Args - - - - - - - - - -

FunctionArgNode * PointerFunctionArgAction(DataType dataType, PointerNode * Pointer, Variable variable) {
    FunctionArgNode * node = malloc(sizeof(FunctionArgNode));
    node->type = pointer;
    node->dataType = dataType;
    node->pointer = Pointer;
    node->variable = variable;
    return node;
}

FunctionArgNode * NoPointerFunctionArgAction(DataType dataType, Variable variable) {
    FunctionArgNode * node = malloc(sizeof(FunctionArgNode));
    node->type = noPointer;
    node->dataType = dataType;
    node->pointer = NULL;
    node->variable = variable;
    return node;
}

FunctionArgsNode * SingleFunctionArgsAction(FunctionArgNode * functionArgNode) {
    FunctionArgsNode * node = malloc(sizeof(FunctionArgsNode));
    node->type = single;
    node->functionArgNode = functionArgNode;
    node->functionArgsNode = NULL;
    return node;
}

FunctionArgsNode * MultipleFunctionArgsAction(FunctionArgNode * functionArgNode, FunctionArgsNode * functionArgsNode) {
    FunctionArgsNode * node = malloc(sizeof(FunctionArgsNode));
    node->type = multiple;
    node->functionArgNode = functionArgNode;
    node->functionArgsNode = functionArgsNode;
    return node;
}

// - - - - - - Function Declaration - - - - - -

FunctionDeclarationNode* FunctionDeclarationNoArgsAction(DataType functionType, Variable variable, CodeBlockNode * codeBlock) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = FunctionDeclarationNoArgs;
    node->functionType = functionType;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = NULL;

    //se agrega a la tabla de simbolos
    addToSymbolList(functionType, variable, false, false, true);

    return node;
}

FunctionDeclarationNode* FunctionDeclarationWithArgsAction(DataType functionType, Variable variable, CodeBlockNode * codeBlock, FunctionArgsNode* functionArgs) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = FunctionDeclarationWithArgs;
    node->functionType = functionType;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = functionArgs;

    //se agrega a la tabla de simbolos
    addToSymbolList(functionType, variable, false, false, true);

    return node;
}

FunctionDeclarationNode* VoidFunctionDeclarationAction(Variable variable, CodeBlockNode * codeBlock) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = FunctionDeclarationNoArgsVoid;
    node->functionType = Void;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = NULL;

    //se agrega a la tabla de simbolos
    addToSymbolList(Void, variable, false, false, true);

    return node;
}

FunctionDeclarationNode* VoidFunctionDeclarationWithArgsAction(Variable variable, CodeBlockNode * codeBlock, FunctionArgsNode* functionArgs) {
    FunctionDeclarationNode* node = malloc(sizeof(FunctionDeclarationNode));
    node->type = FunctionDeclarationWithArgsVoid;
    node->functionType = Void;
    node->variable = variable;
    node->codeBlock = codeBlock;
    node->functionArgs = functionArgs;

    //se agrega a la tabla de simbolos
    addToSymbolList(Void, variable, false, false, true);

    return node;
}


// - - - - - - C Statements  - - - - - -

StatementNode * MetacommandWithStatementNodeAction(StatementNode * statementNode, MetaCommandNode * metaCommandNode) {
    StatementNode * node = malloc(sizeof(StatementNode));
    node->type = MetaCommand;
    node->child = HasChild;
    node->statement = statementNode;
    node->metacommand = metaCommandNode;
    node->functionDeclarationNode = NULL;
    node->declarationNode = NULL;
    return node;
}

StatementNode * FunctionDeclarationWithStatementNodeAction(StatementNode * statementNode,FunctionDeclarationNode *functionDeclarationNode) {
    StatementNode * node = malloc(sizeof(StatementNode));
    node->type = FunctionDeclaration;
    node->child = HasChild;
    node->statement = statementNode;
    node->metacommand = NULL;
    node->functionDeclarationNode = functionDeclarationNode;
    node->declarationNode = NULL;
    return node;
}

StatementNode * DeclarationWithStatementNodeAction(StatementNode * statementNode ,DeclarationNode *declarationNode) {
    StatementNode * node = malloc(sizeof(StatementNode));
    node->type = Declaration;
    node->child = HasChild;
    node->statement = statementNode;
    node->metacommand = NULL;
    node->functionDeclarationNode = NULL;
    node->declarationNode = declarationNode;
    return node;
}

StatementNode * MetaCommandNodeAction(MetaCommandNode * metaCommandNode) {
    StatementNode * node = malloc(sizeof(StatementNode));
    node->type = MetaCommand;
    node->child = NoChild;
    node->statement = NULL;
    node->metacommand = metaCommandNode;
    node->functionDeclarationNode = NULL;
    node->declarationNode = NULL;
    return node;
}

StatementNode * FunctionDeclarationNodeAction(FunctionDeclarationNode * functionDeclarationNode) {
    StatementNode * node = malloc(sizeof(StatementNode));
    node->type = FunctionDeclaration;
    node->child = NoChild;
    node->statement = NULL;
    node->metacommand = NULL;
    node->functionDeclarationNode = functionDeclarationNode;
    node->declarationNode = NULL;
    return node;
}

StatementNode * DeclarationStatementNodeAction(DeclarationNode *declarationNode) {
    StatementNode * node = malloc(sizeof(StatementNode));
    node->type = Declaration;
    node->child = NoChild;
    node->statement = NULL;
    node->metacommand = NULL;
    node->functionDeclarationNode = NULL;
    node->declarationNode = declarationNode;
    return node;
}



/* = = = = = = =  SPECIAL STATEMENT ACTIONS  = = = = = = = */

SpecialStatementNode * specialStatementAction(SelectorNode * selectorNode) {
    SpecialStatementNode * node = malloc(sizeof(SpecialStatementNode));
    node->selectorNode = selectorNode;
    return node;
}

// - - - - - - Special Statement Selector - - - - - - - - - - - -

SelectorNode * ReduceStatementSelectorAction(ReduceStatementNode * reduceStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = ReduceStatement;
    node->reduceStatement = reduceStatementNode;
    node->filterStatement = NULL;
    node->foreachStatement = NULL;
    node->mapStatement = NULL;
    node->createStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->filterRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * FilterStatementSelectorAction(FilterStatementNode * filterStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = FilterStatement;
    node->filterStatement = filterStatementNode;
    node->reduceStatement = NULL;
    node->createStatement = NULL;
    node->foreachStatement = NULL;
    node->mapStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->filterRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * ForeachStatementSelectorAction(ForeachStatementNode * foreachStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = ForeachStatement;
    node->foreachStatement = foreachStatementNode;
    node->reduceStatement = NULL;
    node->filterStatement = NULL;
    node->mapStatement = NULL;
    node->createStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->filterRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * MapStatementSelectorAction(MapStatementNode * mapStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = MapStatement;
    node->mapStatement = mapStatementNode;
    node->reduceStatement = NULL;
    node->filterStatement = NULL;
    node->foreachStatement = NULL;
    node->createStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->filterRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * CreateStatementSelectorAction(CreateStatementNode * createStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = CreateStatement;
    node->createStatement = createStatementNode;
    node->reduceStatement = NULL;
    node->filterStatement = NULL;
    node->foreachStatement = NULL;
    node->mapStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->filterRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * ReduceRangeStatementSelectorAction(ReduceRangeStatementNode * reduceRangeStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = ReduceRangeStatement;
    node->reduceRangeStatement = reduceRangeStatementNode;
    node->reduceStatement = NULL;
    node->filterStatement = NULL;
    node->foreachStatement = NULL;
    node->mapStatement = NULL;
    node->createStatement = NULL;
    node->filterRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * FilterRangeStatementSelectorAction(FilterRangeStatementNode * filterRangeStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = FilterRangeStatement;
    node->filterRangeStatement = filterRangeStatementNode;
    node->reduceStatement = NULL;
    node->filterStatement = NULL;
    node->foreachStatement = NULL;
    node->mapStatement = NULL;
    node->createStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * ForeachRangeStatementSelectorAction(ForeachRangeStatementNode * foreachRangeStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = ForeachRangeStatement;
    node->foreachRangeStatement = foreachRangeStatementNode;
    node->reduceStatement = NULL;
    node->filterStatement = NULL;
    node->foreachStatement = NULL;
    node->mapStatement = NULL;
    node->createStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->filterRangeStatement = NULL;
    node->mapRangeStatement = NULL;
    return node;
}

SelectorNode * MapRangeStatementSelectorAction(MapRangeStatementNode * mapRangeStatementNode) {
    SelectorNode * node = malloc(sizeof(SelectorNode));
    node->selectorNodeType = MapRangeStatement;
    node->mapRangeStatement = mapRangeStatementNode;
    node->reduceStatement = NULL;
    node->filterStatement = NULL;
    node->foreachStatement = NULL;
    node->mapStatement = NULL;
    node->createStatement = NULL;
    node->reduceRangeStatement = NULL;
    node->filterRangeStatement = NULL;
    node->foreachRangeStatement = NULL;
    return node;
}

// - - - - - - Lambda Expressions - - - - - - -

RangeNode * RangeAction(SizeNode * sizeNode1, SizeNode * sizeNode2){
    RangeNode * rangeNode = malloc(sizeof(RangeNode));
    rangeNode->sizeNode1 = sizeNode1;
    rangeNode->sizeNode2 = sizeNode2;

    return rangeNode;
}

ConsumerFunctionNode * ConsumerFunctionAction(FunctionCallNode * functionCallNode){
    ConsumerFunctionNode * consumerFunctionNode = malloc(sizeof(ConsumerFunctionNode));
    consumerFunctionNode->functionCallNode = functionCallNode;

    return consumerFunctionNode;
}

UnboundedParametersNode * UnboundedParametersAction(Variable variable1, SizeNode * sizeNode, Variable variable2 ){
    UnboundedParametersNode * unboundedParametersNode = malloc(sizeof(UnboundedParametersNode));
    unboundedParametersNode->variable1 = variable1;
    unboundedParametersNode->SizeNode = sizeNode;
    unboundedParametersNode->variable2 = variable2;

    //chequeo si existen variable 1 y 2
    if( !contains_symbol(state.list, variable1, true) ){
        //TODO - handle error
        state.succeed = false;
        printf("array: %s not declared\n", variable1 );
    }
    if( !contains_symbol(state.list, variable2, false) && !contains_symbol(state.list, variable2, true) ){
        state.succeed = false;
        printf("variable: %s not declared\n", variable2 );
    }

    return unboundedParametersNode;
}

BoundedParametersNode * BoundedParametersAction(Variable variable1, RangeNode * rangeNode, Variable variable2){
    BoundedParametersNode * boundedParametersNode = malloc(sizeof(BoundedParametersNode));
    boundedParametersNode->variable1 = variable1;
    boundedParametersNode->rangeNode = rangeNode;
    boundedParametersNode->variable2 = variable2;

     //chequeo si existen variable 1 y 2
    if( !contains_symbol(state.list, variable1, true) ){
        state.succeed = false;
        //TODO - handle error
        printf("array: %s not declared\n", variable1 );
    }
    if( !contains_symbol(state.list, variable2, false) && !contains_symbol(state.list, variable2, true)){
        state.succeed = false;
        printf("variable: %s not declared\n", variable2 );
    }

    return boundedParametersNode;
}

Lambda * LambdaAction(ExpressionNode * expressionNode) {
    Lambda * node = malloc(sizeof(Lambda));
    node->expressionNode = expressionNode;
    return node;
}
CreateLambda * CreateLambdaAction(NumConstantIntNode constant1, NumConstantIntNode constant2) {
    CreateLambda * node = malloc(sizeof(CreateLambda));
    node->constant1 = constant1;
    node->constant2 = constant2;
    return node;
}

// - - - - - - Special Statements - - - - - - -
ReduceStatementNode * ReduceStatementAction(UnboundedParametersNode * unboundedParametersNode, Lambda * lambda) {
    ReduceStatementNode * node = malloc(sizeof(ReduceStatementNode));
    node->unboundedParametersNode = unboundedParametersNode;
    node->lambda = lambda;
    return node;
}

FilterStatementNode * FilterStatementAction(UnboundedParametersNode * unboundedParametersNode, Lambda * lambda) {
    FilterStatementNode * node = malloc(sizeof(FilterStatementNode));
    node->unboundedParametersNode = unboundedParametersNode;
    node->lambda = lambda;
    return node;
}

ForeachStatementNode * ForeachStatementAction(Variable variable, SizeNode * sizeNode, ConsumerFunctionNode * consumerFunctionNode) {
    ForeachStatementNode * node = malloc(sizeof(ForeachStatementNode));
    node->variable = variable;
    node->sizeNode = sizeNode;
    node->consumerFunctionNode = consumerFunctionNode;

    //chequeo si existen variable
    if( !contains_symbol(state.list, variable, true) ){
        //TODO - handle error
        state.succeed = false;
        printf("array: %s not declared\n", variable );
    }
       

    return node;
}

MapStatementNode * MapStatementAction(UnboundedParametersNode * unboundedParametersNode, Lambda * lambda) {
    MapStatementNode * node = malloc(sizeof(MapStatementNode));
    node->unboundedParametersNode = unboundedParametersNode;
    node->lambda = lambda;
    return node;
}

CreateStatementNode * CreateStatementAction(Variable variable1, DataType dataType, CreateLambda * createLambda) {
    CreateStatementNode * node = malloc(sizeof(CreateStatementNode));
    node->variable1 = variable1;
    node->dataType = dataType;
    node->createLambda = createLambda;

    //se agrega a symbol list
    if(contains_symbol(state.list, variable1, true) || contains_symbol(state.list, variable1, false)){
        state.succeed = false;
        printf("variable: %s was already declared\n", variable1);
        //TODO - manejo de errores
    }else 
        addToSymbolList(dataType, variable1, false, true, false);

    return node;
}

ReduceRangeStatementNode * ReduceRangeStatementAction(BoundedParametersNode * boundedParametersNode, Lambda * lambda) {
    ReduceRangeStatementNode * node = malloc(sizeof(ReduceRangeStatementNode));
    node->boundedParametersNode = boundedParametersNode;
    node->lambda = lambda;
    return node;
}


FilterRangeStatementNode * FilterRangeStatementAction(BoundedParametersNode * boundedParametersNode, Lambda * lambda) {
    FilterRangeStatementNode * node = malloc(sizeof(FilterRangeStatementNode));
    node->boundedParametersNode = boundedParametersNode;
    node->lambda = lambda;
    return node;
}

ForeachRangeStatementNode * ForeachRangeStatementAction(Variable variable, RangeNode * rangeNode, ConsumerFunctionNode * consumerFunctionNode) {
    ForeachRangeStatementNode * node = malloc(sizeof(ForeachRangeStatementNode));
    node->variable = variable;
    node->rangeNode = rangeNode;
    node->consumerFunctionNode = consumerFunctionNode;

      //chequeo si existen variable 1
    if( !contains_symbol(state.list, variable, true) ){
        //TODO - handle error
        state.succeed = false;
        printf("array: %s not declared\n", variable );
    }
       
    return node;
}

MapRangeStatementNode * MapRangeStatementAction(BoundedParametersNode * boundedParametersNode, Lambda * lambda) {
    MapRangeStatementNode * node = malloc(sizeof(MapRangeStatementNode));
    node->boundedParametersNode = boundedParametersNode;
    node->lambda = lambda;
    return node;
}



/* = = = = = = =  FREE FUNCTIONS  = = = = = = = */

// - - - - - - Free Includes - - - - - - - - 
/*
void freeMetaCommandNode(MetaCommandNode * node) {
	free(node);
}

// - - - - - - Free Dereferencing - - - - - -

void freeSizeNode(SizeNode * node) {
	free(node->variable);
	free(node->numConstantIntNode);
	free(node);
}

void freeArrayDerefNode(ArrayDerefNode * node) {
	freeSizeNode(node->sizeNode);
	free(node->variable);
	free(node);
}

// - - - - - - Free Pointer - - - - - - - - -

void freePointerNode(PointerNode * node) {
	if(node->pointerNode != NULL)
		freePointerNode(node->pointerNode);
	free(node);
}

// - - - - - - Free Declaration - - - - - - -

void freeDeclarationNode(DeclarationNode * node) {
	if(node->singleDeclarationNode != NULL)
		//freeSingleDeclarationNode(node->singleDeclarationNode); TODO
	if(node->arrayDeclarationNode != NULL)
		//freeArrayDeclarationNode(node->arrayDeclarationNode); TODO
	free(node);
}

// - - - - - - Free Single Declaration - - - -

void freeSingleDeclarationNode(SingleDeclarationNode * node) {
	free(node->variable);
	freePointerNode(node->pointer);
	//freeSingleInitializeNode(node->singleInitializeNode);  TODO
	free(node);
}

// - - - - - - Free Lambdas - - - - - - - - -

void freeLambda(Lambda * node) {
    free(node->expressionNode);
    free(node);
}

void freeCreateLambda(CreateLambda * node) {
    free(node->constant1);
    free(node->constant2);
    free(node);
}

// - - - - - - Free Selector - - - - - - - - -

void freeReduceStatementNode(ReduceStatementNode * node) {
    free(node->variable1);
    free(node->size);
    free(node->variable2);
    freeLambda(node->lambda);
    free(node);
}

void freeFilterStatementNode(FilterStatementNode * node) {
    free(node->variable1);
    free(node->size);
    free(node->variable2);
    freeLambda(node->lambda);
    free(node);
}

void freeForeachStatementNode(ForeachStatementNode * node) {
    free(node->variable1);
    free(node->size);
    // freeFunctionCallNode(node->functionCallNode);  TODO: DESCOMENTAR
    free(node);
}

void freeMapStatementNode(MapStatementNode * node) {
    free(node->variable1);
    free(node->size);
    free(node->variable2);
    freeLambda(node->lambda);
    free(node);
}

void freeCreateStatementNode(CreateStatementNode * node) {
    free(node->variable1);
    free(node);
}

void freeReduceRangeStatementNode(ReduceRangeStatementNode * node) {
    free(node->variable1);
    free(node->size1);
    free(node->variable2);
    free(node->size2);
    freeLambda(node->lambda);
    free(node);
}

void freeFilterRangeStatementNode(FilterRangeStatementNode * node) {
    free(node->variable1);
    free(node->size1);
    free(node->size2);
    free(node->variable2);
    freeLambda(node->lambda);
    free(node);
}

void freeForeachRangeStatementNode(ForeachRangeStatementNode * node) {
    free(node->variable1);
    free(node->size1);
    free(node->size1);
    //freeFunctionCallNode(node->functionCallNode); TODO: DESCOMENTAR
    free(node);
}

void freeMapRangeStatementNode(MapRangeStatementNode * node) {
    free(node->variable1);
    free(node->size1);
    free(node->size2);
    free(node->variable2);
    freeLambda(node->lambda);
    free(node);
}

void freeSelectorNode(SelectorNode * node) {
    switch (node->selectorNodeType) {
        case ReduceStatement:
            freeReduceStatementNode(node->reduceStatement);
            break;
        case FilterStatement:
            freeFilterStatementNode(node->filterStatement);
            break;
        case ForeachStatement:
            freeForeachStatementNode(node->foreachStatement);
            break;
        case MapStatement:
            freeMapStatementNode(node->mapStatement);
            break;
        case CreateStatement:
            freeCreateStatementNode(node->createStatement);
            break;
        case ReduceRangeStatement:
            freeReduceRangeStatementNode(node->reduceRangeStatement);
            break;
        case FilterRangeStatement:
            freeFilterRangeStatementNode(node->filterRangeStatement);
            break;
        case ForeachRangeStatement:
            freeForeachRangeStatementNode(node->foreachRangeStatement);
            break;
        case MapRangeStatement:
            freeMapRangeStatementNode(node->mapRangeStatement);
            break;
    }
    free(node);
}
*/

