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
ArrayInitializeNode * ArrayInitializeEmptyGrammarAction();
ArrayInitializeNode * ArrayInitializeWithListGrammarAction(ArrayListNode * arrayListNode);
AssigmentNode * AssignmentWithArrayDerefGrammarAction(ArrayDerefNode * arrayDerefNode, AssignmentType type, ExpressionNode * expressionNode );
AssigmentNode * AssignmentWithVarGrammarAction(Variable * var, AssignmentType type, ExpressionNode * expressionNode );
ArrayDerefNode * ArrayDerefGrammarAction(Variable * var, SizeNode * sizeNode);
ReturnStatementNode * ReturnStatementGrammarAction(ExpressionNode * expressionNode);
IfElseStatementNode * IfWithoutElseStatementGrammarAction(IfStatementNode * ifStatementNode);
IfElseStatementNode * IfWithElseStatementGrammarAction(IfStatementNode * ifStatementNode, ElseStatementNode * elseStatementNode);
IfStatementNode * IfStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
ElseStatementNode * ElseStatementGrammarAction(CodeBlockNode * codeBlockNode);
WhileStatementNode * WhileStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
ForStatementNode * ForStatementWithAssigmentGrammarAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, AssigmentNode * assignmentNode, CodeBlockNode * codeBlockNode );
ForStatementNode * ForStatementWithExpressionGrammarAction(DeclarationNode * declarationNode, ExpressionNode * firstExpressionNode, ExpressionNode * secondExpressionNode, CodeBlockNode * codeBlockNode );
SwitchStatementNode * SwitchStatementGrammarAction(ExpressionNode * expressionNode, CodeBlockNode * codeBlockNode);
SizeNode * SizeNumConstIntGrammarAction(NumConstantIntNode * numConstantIntNode);
SizeNode * SizeVarGrammarAction(Variable * variableNode);
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
ExpressionNode * OrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * NotOpExpressionGrammarAction(ExpressionNode * rightExpressionNode);
ExpressionNode * EqOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * GrOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * GeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * LtOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * LeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * NeOpExpressionGrammarAction(ExpressionNode * leftExpressionNode, ExpressionNode * rightExpressionNode);
ExpressionNode * variableOpExpressionGrammarAction(Variable * variable);
ExpressionNode * NumConstantFloatOpExpressionGrammarAction(NumConstantFloatNode * numConstantFloatNode);
ExpressionNode * NumConstantIntOpExpressionGrammarAction(NumConstantIntNode * numConstantIntNode);
ExpressionNode * SpecialVarOpExpressionGrammarAction(SpecialVariable * specialVariableNode);
ExpressionNode * FunctionCallOpExpressionGrammarAction(FunctionCallNode * functionCallNode);
ExpressionNode * ArrayDerefOpExpressionGrammarAction(ArrayDerefNode * arrayDerefNode);
ExpressionNode * StringOpExpressionGrammarAction(StringVar * stringVar);

FunctionCallNode * WithArgsFunctionCallGrammarAction(Variable * variable, FunctionCallArgNode * functionCallArgNode);
FunctionCallNode * NoArgsFunctionCallGrammarAction(Variable * variable);
FunctionCallArgNode * WithArgsFunctionCallArgGrammarAction(ExpressionNode * expressionNode, FunctionCallArgNode * functionCallArgNode);
FunctionCallArgNode * NoArgsFunctionCallArgGrammarAction(ExpressionNode * expressionNode);

#endif
