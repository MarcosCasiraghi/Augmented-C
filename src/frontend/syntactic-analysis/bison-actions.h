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
int AdditionExpressionGrammarAction(const int leftValue, const int rightValue);
int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue);
int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue);
int DivisionExpressionGrammarAction(const int leftValue, const int rightValue);
int FactorExpressionGrammarAction(const int value);

// Factores.
int ExpressionFactorGrammarAction(const int value);
int ConstantFactorGrammarAction(const int value);

// Constantes.
int IntegerConstantGrammarAction(const int value);


//AUGMENTED-C
ReduceStatementNode * ReduceStatementAction(Variable variable1, NumConstantIntNode size, Variable variable2, Lambda * lambda);
FilterStatementNode * FilterStatementAction(Variable variable1, NumConstantIntNode size, Variable variable2, Lambda * lambda);
ForeachStatementNode * ForeachStatementAction(Variable variable1, NumConstantIntNode size, FunctionCallNode * functionCallNode);
MapStatementNode * MapStatementAction(Variable variable1, NumConstantIntNode size, Variable variable2, CreateLambda * createLambda);
CreateStatementNode * CreateStatementAction(Variable variable1, DataType dataType, Lambda * lambda);
ReduceRangeStatementNode * ReduceRangeStatementAction(Variable variable1, NumConstantIntNode size1, NumConstantIntNode size2, Variable variable2, Lambda * lambda);
FilterRangeStatementNode * FilterRangeStatementAction(Variable variable1, NumConstantIntNode size1, NumConstantIntNode size2, Variable variable2, Lambda * lambda);
ForeachRangeStatementNode * ForeachRangeStatementAction(Variable variable1, NumConstantIntNode size1, NumConstantIntNode size2, FunctionCallNode * functionCallNode);
MapRangeStatementNode * MapRangeStatementAction(Variable variable1, NumConstantIntNode size1, NumConstantIntNode size2, Variable variable2, CreateLambda * createLambda);

//ANSI C
ArrayInitializeNode * ArrayInitializeEmptyGrammarAction();
ArrayInitializeNode * ArrayInitializeWithListGrammarAction(ArrayListNode * arrayListNode);
AssignmentNode * AssignmentWithArrayDerefGrammarAction(ArrayDerefNode * arrayDerefNode, AssignmentType type, ExpressionNode * expressionNode );
AssignmentNode * AssignmentWithVarGrammarAction(Variable  var, AssignmentType type, ExpressionNode * expressionNode );
ArrayDerefNode * ArrayDerefGrammarAction(Variable  var, SizeNode * sizeNode);
ReturnStatementNode * ReturnStatementGrammarAction(ExpressionNode * expressionNode);
IfElseStatementNode * IfWithoutElseStatementGrammarAction(IfStatementNode * ifStatementNode);
IfElseStatementNode * IfWithElseStatementGrammarAction(IfStatementNode * ifStatementNode, ElseStatementNode * elseStatementNode);
IfStatementNode * IfStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
ElseStatementNode * ElseStatementGrammarAction(CodeBlockNode * codeBlockNode);
WhileStatementNode * WhileStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
ForStatementNode * ForStatementWithAssigmentGrammarAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, AssignmentNode * assignmentNode, CodeBlockNode * codeBlockNode );
ForStatementNode * ForStatementWithExpressionGrammarAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, ExpressionNode * secondExpressionNode, CodeBlockNode * codeBlockNode );
SwitchStatementNode * SwitchStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
SizeNode * SizeNumConstIntGrammarAction(NumConstantIntNode * numConstantIntNode);
SizeNode * SizeVarGrammarAction(Variable  variableNode);
ExpressionNode * AddOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * SubOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * MultOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * DivOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * ModOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * IncOpRightExpressionGrammarAction(ExpressionNode * leftExpressionNode);
ExpressionNode * IncOpLeftExpressionGrammarAction(ExpressionNode * rightExpressionNode);
ExpressionNode * DecOpRightExpressionGrammarAction(ExpressionNode * leftExpressionNode);
ExpressionNode * DecOpLeftExpressionGrammarAction(ExpressionNode * rightExpressionNode);
ExpressionNode * BitNotOpExpressionGrammarAction(ExpressionNode * rightExpressionNode);
ExpressionNode * BitRightOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitLeftOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitXorOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitOrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * BitAndOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * AndOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * ParenthesisExpressionAction(ExpressionNode * rightExpressionNode);
ExpressionNode * OrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * NotOpExpressionGrammarAction(ExpressionNode * rightExpressionNode);
ExpressionNode * EqOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * GrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * GeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * LtOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * LeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * NeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * variableOpExpressionGrammarAction(Variable  variable);
ExpressionNode * NumConstantFloatOpExpressionGrammarAction(NumConstantFloatNode * numConstantFloatNode);
ExpressionNode * NumConstantIntOpExpressionGrammarAction(NumConstantIntNode * numConstantIntNode);
ExpressionNode * SpecialVarOpExpressionGrammarAction(SpecialVariable  specialVariableNode);
ExpressionNode * FunctionCallOpExpressionGrammarAction(FunctionCallNode * functionCallNode);
ExpressionNode * ArrayDerefOpExpressionGrammarAction(ArrayDerefNode * arrayDerefNode);
ExpressionNode * StringOpExpressionGrammarAction(StringVar * stringVar);

FunctionCallNode * WithArgsFunctionCallGrammarAction(Variable  variable, FunctionCallArgNode * functionCallArgNode);
FunctionCallNode * NoArgsFunctionCallGrammarAction(Variable  variable);
FunctionCallArgNode * WithArgsFunctionCallArgGrammarAction(ExpressionNode * expressionNode, FunctionCallArgNode * functionCallArgNode);
FunctionCallArgNode * NoArgsFunctionCallArgGrammarAction(ExpressionNode * expressionNode);

FunctionDeclarationNode* FunctionDeclarationNoArgsAction(DataType functionType, Variable variable, CodeBlockNode * codeBlock);
FunctionDeclarationNode* FunctionDeclarationWithArgsAction(DataType functionType, Variable variable, CodeBlockNode * codeBlock, FunctionCallArgNode* functionArgs);
FunctionDeclarationNode* VoidFunctionDeclarationAction(Variable variable, CodeBlockNode * codeBlock, DataType functionType);
FunctionDeclarationNode* VoidFunctionDeclarationWithArgsAction(Variable variable, CodeBlockNode * codeBlock, DataType functionType, FunctionCallArgNode* functionArgs);
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
AssignmentType AssignAction();
AssignmentType SumAssignAction();
AssignmentType SubAssignAction();
AssignmentType MultAssignAction();
AssignmentType DivAssignAction();
AssignmentType ModAssignAction();
ArrayInitializeNode * ArrayInitializeEmptyAction();
ArrayInitializeNode * ArrayInitializeWithListAction(ArrayListNode * arrayListNode);
ArrayListNode * ArrayListAction(NumConstantIntNode integer);
ArrayListNode * ArrayListManyAction(NumConstantIntNode integer, ArrayListNode * arrayListNode);
AssignmentNode * AssignmentWithVarAction(Variable var, AssignmentType type, ExpressionNode * expressionNode );
AssignmentNode * AssignmentWithArrayDerefAction(ArrayDerefNode * arrayDerefNode, AssignmentType type, ExpressionNode * expressionNode );
FunctionCallNode * WithArgsFunctionCallAction(Variable variable, FunctionCallArgNode * functionCallArgNode);


#endif
