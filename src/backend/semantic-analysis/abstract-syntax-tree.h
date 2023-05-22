#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/
typedef struct {
	int value;
} Constant;

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/
typedef enum {
	EXPRESSION,
	CONSTANT
} FactorType;

typedef struct {
	FactorType type;
	Expression * expression;
} Factor;

typedef enum {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	FACTOR
} ExpressionType;

struct Expression {
	ExpressionType type;
	Expression * leftExpression;
	Expression * rightExpression;
};

typedef struct {
	Expression * expression;
} Program;


struct VariableNode {
	VARIABLENAME * variableName;
};

struct ArrayDerefNode {				//Open y Close bracket son implicitos
	VariableNode * variableNode;
	SizeNode * sizeNode;
};

enum DataType {
	Int,
	Float,
	Dobule, 
	Long,
	Short,
	Char,
	VoidPointer
};

struct ReturnStatementNode {			//return y ; son implicitos
	ExpressionNode * expressionNode;
};

struct IfElseStatement {
	IfStatementNode * ifStatementNode;
	ElseStatementNode * elseStatementNode;
};

struct IfStatementNode {				//if, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;

};

struct ElseStatementNode {				//else, {, } son implicitos
	CodeBlockNode * codeBlockNode;
};

struct WhileStatementNode {			//while, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;
};

struct ForStatementNode {				//for, (, ), ;, {, } son implicitos
	DeclarationNode * declarationNode;
	ExpressionNode * expressionNode;
	AssigmentNode * assigmentNode;			//either one of these
	ExpressionNode * expressionNode;		//
	CodeBlockNode * codeBlockNode;
};

struct SwitchStatementNode {				//switch, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;
};

struct SizeNode {
	VariableNode * variableNode;
	NumConstantIntNode * numConstantIntNode;
};

enum ExpressionNodeType{
	AddOp,
	SubOp,
	MultOp,
	DivOp,
	ModOp,
	IncOp,
	DecOp,
	BitNotOp,
	BitRightOp,
	BitLeftOp,
	BitXorOp,
	BitOrOp,
	BitAndOp,
	AndOp,
	OrOp,
	NotOp,
	EqOp,
	GrOp,
	GeOp,
	LtOp,
	LeOp,
	NeOp
};

typedef struct ExpressionNode ExpressionNode;

struct ExpressionNode {						//(, ) son implicitos
	ExpressionNodeType op;
	ExpressionNode * leftExpressionNode;
	ExpressionNode * rightExpressionNode;
	VariableNode * variableNode;
	NumConstantIntNode * numConstantIntode;
	NumConstatnFloatNode * numConstantFloatNode;
	SpecialVariableNode * specialVariableNode;
	FunctionCallNode * functionCallNode;
	ArrayDerefNode * arrayDerefNode;
	StringNode * StringNode;
};

struct FunctionCallNode {						//(, ) son implicitos
	VariableNode * variableNode;
	FunctionCallArgNode * functionCallArgNode;		//puede ser null
};


typedef struct FunctionCallArgNode FunctionCallArgNode;

struct FunctionCallArgNode {				//',' es implicito
	ExpressionNode * expressionNode;
	FunctionCallArgNode * functionCallArgNode;		//puede ser null
};





#endif
