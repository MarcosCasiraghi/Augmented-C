#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"
#include "../support/shared.h"
#include "../semantic-analysis/symbol-list.h"

void Generator(int result);
void GenMetaCommandNode(MetaCommandNode * node);
void GenSizeNode(SizeNode * node);
void GenArrayDerefNode(ArrayDerefNode * node);
void GenPointerNode(PointerNode * node);
void GenSingleInitializeNode(SingleInitializeNode * node, int isFor);
void GenSingleDeclarationNode(SingleDeclarationNode * node, int ifFOr);
void GenArrayInitializeNode(ArrayInitializeNode * node);
void GenArrayListNode(ArrayListNode * node);
void GenArraySizeNode(ArraySizeNode * node);
void GenArrayDeclarationNode(ArrayDeclarationNode * node);
void GenDeclarationNode(DeclarationNode * node, int isFor);
void GenAssignmentNode(AssignmentNode * node, int isFor);
void GenFunctionCallNode(FunctionCallNode * node, int isFor);
void GenFunctionCallArgNode(FunctionCallArgNode * node, int isFor);
void GenExpressionNode(ExpressionNode * node, int isFor, char * variableName, char * index);
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
void GenProgramNode(ProgramNode * node);

//---------- Special Statement -------------
void GenSpecialStatementNode(SpecialStatementNode * node);
void GenSelectorNode(SelectorNode * node);
void GenReduceStatementNode(ReduceStatementNode * node);
void GenReduceRangeStatementNode(ReduceRangeStatementNode * node);
void GenFilterStatementNode(FilterStatementNode * node);
void GenFilterRangeStatementNode(FilterRangeStatementNode * node);
void GenForeachStatementNode(ForeachStatementNode * node);
void GenForeachRangeStatementNode(ForeachRangeStatementNode * node);
void GenMapStatementNode(MapStatementNode * node);
void GenMapRangeStatementNode(MapRangeStatementNode * node);
void GenCreateStatementNode(CreateStatementNode * node);
void GenRangeNode(RangeNode * node);
void GenConsumerFunctionNode(ConsumerFunctionNode * node, int isFor);
void GenUnboundedParametersNode(UnboundedParametersNode * node);
void GenBoundedParametersNode(BoundedParametersNode * node);
#endif
