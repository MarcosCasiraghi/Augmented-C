#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"

void Generator(int result);
void GenMetaCommandNode(MetaCommandNode * node);
void GenSizeNode(SizeNode * node);
void GenArrayDerefNode(ArrayDerefNode * node);
void GenPointerNode(PointerNode * node);
void GenSingleInitializeNode(SingleInitializeNode * node);
void GenSingleDeclarationNode(SingleDeclarationNode * node);
void GenArrayInitializeNode(ArrayInitializeNode * node);
void GenArrayListNode(ArrayListNode * node);
void GenArraySizeNode(ArraySizeNode * node);
void GenArrayDeclarationNode(ArrayDeclarationNode * node);
void GenDeclarationNode(DeclarationNode * node);
void GenAssignmentNode(AssignmentNode * node);
void GenFunctionCallNode(FunctionCallNode * node);
void GenFunctionCallArgNode(FunctionCallArgNode * node);
void GenExpressionNode(ExpressionNode * node);
void GenReturnStatementNode(ReturnStatementNode * node);
void GenIfStatementNode(IfStatementNode * node);
void GenElseStatementNode(ElseStatementNode * node);
void GenIfElseStatementNode(IfElseStatementNode * node);
void GenWhileStatementNode(WhileStatementNode * node);
void GenForStatementNode(ForStatementNode * node);
void GenSwitchStatementNode(SwitchStatementNode * node);
void GenCodeBlockNode(CodeBlockNode * node);
void GenFunctionArgNode(FunctionArgNode * node);
void GenFunctionArgsNode(FunctionArgsNode * node);
void GenFunctionDeclarationNode(FunctionDeclarationNode * node);
void GenStatementNode(StatementNode * node);
void GenSpecialStatementNode(SpecialStatementNode * node);


#endif
