#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
void ProgramGrammarAction();


// Expresión.
int AdditionExpressionAction(const int leftValue, const int rightValue);
int SubtractionExpressionAction(const int leftValue, const int rightValue);
int MultiplicationExpressionAction(const int leftValue, const int rightValue);
int DivisionExpressionAction(const int leftValue, const int rightValue);
int FactorExpressionAction(const int value);

// Factores.
int ExpressionFactorAction(const int value);
int ConstantFactorAction(const int value);

// Constantes.
int IntegerConstantAction(const int value);


//AUGMENTED-C
ReduceStatementNode * ReduceStatementAction(Variable variable1, SizeNode * size, Variable variable2, Lambda * lambda);
FilterStatementNode * FilterStatementAction(Variable variable1, SizeNode * size, Variable variable2, Lambda * lambda);
ForeachStatementNode * ForeachStatementAction(Variable variable1, SizeNode * size, FunctionCallNode * functionCallNode);
MapStatementNode * MapStatementAction(Variable variable1, SizeNode * size, Variable variable2, Lambda * lambda);
CreateStatementNode * CreateStatementAction(Variable variable1, DataType dataType, CreateLambda * createLambda);
ReduceRangeStatementNode * ReduceRangeStatementAction(Variable variable1, SizeNode * size1, SizeNode * size2, Variable variable2, Lambda * lambda);
FilterRangeStatementNode * FilterRangeStatementAction(Variable variable1, SizeNode * size1, SizeNode * size2, Variable variable2, Lambda * lambda);
ForeachRangeStatementNode * ForeachRangeStatementAction(Variable variable1, SizeNode * size1, SizeNode * size2, FunctionCallNode * functionCallNode);
MapRangeStatementNode * MapRangeStatementAction(Variable variable1, SizeNode * size1, SizeNode * size2, Variable variable2, Lambda * lambda);

//ANSI C
ArrayInitializeNode * ArrayInitializeEmptyAction();
ArrayInitializeNode * ArrayInitializeWithListAction(ArrayListNode * arrayListNode);
AssignmentNode * AssignmentWithArrayDerefAction(ArrayDerefNode * arrayDerefNode, AssignmentType type, ExpressionNode * expressionNode );
AssignmentNode * AssignmentWithVarAction(Variable  var, AssignmentType type, ExpressionNode * expressionNode );
ArrayDerefNode * ArrayDerefAction(Variable  var, SizeNode * sizeNode);
ReturnStatementNode * ReturnStatementAction(ExpressionNode * expressionNode);
IfElseStatementNode * IfWithoutElseStatementAction(IfStatementNode * ifStatementNode);
IfElseStatementNode * IfWithElseStatementAction(IfStatementNode * ifStatementNode, ElseStatementNode * elseStatementNode);
IfStatementNode * IfStatementAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
ElseStatementNode * ElseStatementAction(CodeBlockNode * codeBlockNode);
WhileStatementNode * WhileStatementAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
ForStatementNode * ForStatementWithAssigmentAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, AssignmentNode * assignmentNode, CodeBlockNode * codeBlockNode );
ForStatementNode * ForStatementWithExpressionAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, ExpressionNode * secondExpressionNode, CodeBlockNode * codeBlockNode );
SwitchStatementNode * SwitchStatementAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
SizeNode * SizeNumConstIntAction(NumConstantIntNode * numConstantIntNode);
SizeNode * SizeVarAction(Variable  variableNode);
ExpressionNode * AddOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * SubOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * MultOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * DivOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * ModOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * IncOpRightExpressionAction(ExpressionNode * leftExpressionNode);
ExpressionNode * IncOpLeftExpressionAction(ExpressionNode * rightExpressionNode);
ExpressionNode * DecOpRightExpressionAction(ExpressionNode * leftExpressionNode);
ExpressionNode * DecOpLeftExpressionAction(ExpressionNode * rightExpressionNode);
ExpressionNode * BitNotOpExpressionAction(ExpressionNode * rightExpressionNode);
ExpressionNode * BitRightOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitLeftOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitXorOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitOrOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitAndOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * AndOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * ParenthesisExpressionAction(ExpressionNode * rightExpressionNode);
ExpressionNode * OrOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * NotOpExpressionAction(ExpressionNode * rightExpressionNode);
ExpressionNode * EqOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * GrOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * GeOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * LtOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * LeOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * NeOpExpressionAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * variableOpExpressionAction(Variable  variable);
ExpressionNode * NumConstantFloatOpExpressionAction(NumConstantFloatNode * numConstantFloatNode);
ExpressionNode * NumConstantIntOpExpressionAction(NumConstantIntNode * numConstantIntNode);
ExpressionNode * SpecialVarOpExpressionAction(SpecialVariable  specialVariableNode);
ExpressionNode * FunctionCallOpExpressionAction(FunctionCallNode * functionCallNode);
ExpressionNode * ArrayDerefOpExpressionAction(ArrayDerefNode * arrayDerefNode);
ExpressionNode * StringOpExpressionAction(StringVar * stringVar);

FunctionCallNode * WithArgsFunctionCallAction(Variable  variable, FunctionCallArgNode * functionCallArgNode);
FunctionCallNode * NoArgsFunctionCallAction(Variable  variable);
FunctionCallArgNode * WithArgsFunctionCallArgAction(ExpressionNode * expressionNode, FunctionCallArgNode * functionCallArgNode);
FunctionCallArgNode * NoArgsFunctionCallArgAction(ExpressionNode * expressionNode);

FunctionDeclarationNode* FunctionDeclarationNoArgsAction(DataType functionType, Variable variable, CodeBlockNode * codeBlock);
FunctionDeclarationNode* FunctionDeclarationWithArgsAction(DataType functionType, Variable variable, CodeBlockNode * codeBlock, FunctionArgsNode* functionArgs);
FunctionDeclarationNode* VoidFunctionDeclarationAction(Variable variable, CodeBlockNode * codeBlock, DataType functionType);
FunctionDeclarationNode* VoidFunctionDeclarationWithArgsAction(Variable variable, CodeBlockNode * codeBlock, DataType functionType, FunctionArgsNode* functionArgs);
CodeBlockNode * DeclarationCodeBlockAction(DeclarationNode * declarationNode);
CodeBlockNode * SpecialStatementCodeBlockAction(SpecialStatementNode * specialStatement);
CodeBlockNode * ExpressionCodeBlockAction(ExpressionNode * expression);
CodeBlockNode * ReturnCodeBlockAction(ReturnStatementNode * returnStatementNode);
CodeBlockNode * IfElseCodeBlockAction(IfElseStatementNode * ifElse);
CodeBlockNode * ForCodeBlockAction(ForStatementNode * forStatement);
CodeBlockNode * WhileCodeBlockAction(WhileStatementNode * whileStatement);
CodeBlockNode * SwitchCodeBlockAction(SwitchStatementNode * switchStatement);
CodeBlockNode * AssingmentCodeBlockAction(AssignmentNode * assingment);
CodeBlockNode * ContinueCodeBlockAction(CodeBlockNode * codeBlock);
CodeBlockNode * BreakCodeBlockAction(CodeBlockNode * codeBlock);
CodeBlockNode * CaseCodeBlockAction(ExpressionNode * expression, CodeBlockNode * codeBlock);
CodeBlockNode * DefaultCaseCodeBlockAction(CodeBlockNode * codeBlock);
CodeBlockNode * DeclarationCodeBlockActionWithChild(DeclarationNode * declarationNode, CodeBlockNode * codeBlock);
CodeBlockNode * SpecialStatementCodeBlockActionWithChild(SpecialStatementNode * specialStatement, CodeBlockNode * codeBlock);
CodeBlockNode * ExpressionCodeBlockActionWithChild(ExpressionNode * expression, CodeBlockNode * codeBlock);
CodeBlockNode * ReturnCodeBlockActionWithChild(ReturnStatementNode * returnStatementNode, CodeBlockNode * codeBlock);
CodeBlockNode * IfElseCodeBlockActionWithChild(IfElseStatementNode * ifElse, CodeBlockNode * codeBlock);
CodeBlockNode * ForCodeBlockActionWithChild(ForStatementNode * forStatement, CodeBlockNode * codeBlock);
CodeBlockNode * WhileCodeBlockActionWithChild(WhileStatementNode * whileStatement, CodeBlockNode * codeBlock);
CodeBlockNode * SwitchCodeBlockActionWithChild(SwitchStatementNode * switchStatement, CodeBlockNode * codeBlock);
CodeBlockNode * AssingmentCodeBlockActionWithChild(AssignmentNode * assingment, CodeBlockNode * codeBlock);
PointerNode * PointerAction();
PointerNode * PointerActionWithChild(PointerNode * pointerNode);
DeclarationNode * DeclarationOfSingleAction(SingleDeclarationNode * singleDeclarationNode);
DeclarationNode * DeclarationOfArrayAction(ArrayDeclarationNode * arrayDeclarationNode);
SingleDeclarationNode * SingleWithPointerDeclarationAction(PointerNode * pointer, DataType dataType, Variable variable, SingleInitializeNode * singleInitializeNode) ;
SingleDeclarationNode * SingleWithoutPointerDeclarationAction(DataType dataType, Variable variable, SingleInitializeNode * singleInitializeNode);
SingleInitializeNode * SingleInitializationWithoutAssignAction();
SingleInitializeNode * SingleInitializationWithAssignAction(ExpressionNode * expressionNode);
ArrayDeclarationNode * ArrayDeclarationAction(DataType dataType, Variable variable, ArraySizeNode * arraySizeNode, ArrayInitializeNode * arrayInitializeNode);
ArraySizeNode * ArraySizeWithoutSizeNorChildrenAction();
ArraySizeNode * ArraySizeWithSizeWithoutChildrenAction(NumConstantIntNode * numberConstant);
ArraySizeNode * ArraySizeWithoutSizeWithChildrenAction(ArraySizeNode * arraySizeNode);
ArraySizeNode * ArraySizeWithSizeWithChildrenAction(NumConstantIntNode * numberConstant, ArraySizeNode * arraySizeNode);

//Assignment Type
AssignmentType AssignAction();
AssignmentType SumAssignAction();
AssignmentType SubAssignAction();
AssignmentType MultAssignAction();
AssignmentType DivAssignAction();
AssignmentType ModAssignAction();

//Data Type
DataType IntAction();
DataType FloatAction();
DataType DoubleAction();
DataType LongAction();
DataType ShortAction();
DataType CharAction();
DataType VoidPointerAction();

//Array Initialization
ArrayInitializeNode * ArrayInitializeEmptyAction();
ArrayInitializeNode * ArrayInitializeWithListAction(ArrayListNode * arrayListNode);

//Array List
ArrayListNode * ArrayListAction(NumConstantIntNode integer);
ArrayListNode * ArrayListManyAction(NumConstantIntNode integer, ArrayListNode * arrayListNode);
AssignmentNode * AssignmentWithVarAction(Variable var, AssignmentType type, ExpressionNode * expressionNode );
AssignmentNode * AssignmentWithArrayDerefAction(ArrayDerefNode * arrayDerefNode, AssignmentType type, ExpressionNode * expressionNode );
FunctionCallNode * WithArgsFunctionCallAction(Variable variable, FunctionCallArgNode * functionCallArgNode);

// Special Statement
SpecialStatementNode * specialStatementAction(SelectorNode * selectorNode);
//Special Statement Selector
SelectorNode * ReduceStatementSelectorAction(ReduceStatementNode * reduceStatementNode);
SelectorNode * FilterStatementSelectorAction(FilterStatementNode * filterStatementNode);
SelectorNode * ForeachStatementSelectorAction(ForeachStatementNode * foreachStatementNode);
SelectorNode * MapStatementSelectorAction(MapStatementNode * mapStatementNode);
SelectorNode * CreateStatementSelectorAction(CreateStatementNode * createStatementNode);
SelectorNode * ReduceRangeStatementSelectorAction(ReduceRangeStatementNode * reduceRangeStatementNode);
SelectorNode * FilterRangeStatementSelectorAction(FilterRangeStatementNode * filterRangeStatementNode);
SelectorNode * ForeachRangeStatementSelectorAction(ForeachRangeStatementNode * foreachRangeStatementNode);
SelectorNode * MapRangeStatementSelectorAction(MapRangeStatementNode * mapRangeStatementNode);

//Lambda Expressions
Lambda * LambdaAction(ExpressionNode * expressionNode);
CreateLambda * CreateLambdaAction(NumConstantIntNode * constant1, NumConstantIntNode * constant2);

//Includes
MetaCommandNode * StringMetaCommandAction();
MetaCommandNode * FileNameMetaCommandAction();

//C statements
StatementNode * FunctionDeclarationWithStatementNodeAction(StatementNode * statementNode,FunctionDeclarationNode *functionDeclarationNode);
StatementNode * DeclarationWithStatementNodeAction(StatementNode * statementNode ,DeclarationNode *declarationNode);
StatementNode * FunctionDeclarationNodeAction(FunctionDeclarationNode * functionDeclarationNode);
StatementNode * DeclarationStatementNodeAction(DeclarationNode *declarationNode);
StatementNode * MetaCommandNodeAction(MetaCommandNode * metaCommandNode);
StatementNode * MetacommandWithStatementNodeAction(StatementNode * statementNode, MetaCommandNode * metaCommandNode);
StatementNode * MetacommandWithStatementNodeAction(StatementNode * statementNode, MetaCommandNode * metaCommandNode);

//FunctionArg
FunctionArgNode * NoPointerFunctionArgAction(DataType dataType, Variable variable);
FunctionArgNode * PointerFunctionArgAction(DataType dataType, PointerNode * Pointer, Variable variable);
FunctionArgsNode * SingleFunctionArgsAction(FunctionArgNode * functionArgNode);
FunctionArgsNode * MultipleFunctionArgsAction(FunctionArgNode * functionArgNode, FunctionArgsNode * functionArgsNode);

CodeBlockNode * AssignmentCodeBlockActionWithChild(AssignmentNode * assingment, CodeBlockNode * codeBlock);
CodeBlockNode * AssignmentCodeBlockAction(AssignmentNode * assingment);

#endif
