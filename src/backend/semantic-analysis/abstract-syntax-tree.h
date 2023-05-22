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
typedef Variable char *;
typedef struct CodeBlockNode CodeBlockNode;
typedef struct PointerNode PointerNode;
typedef struct ExpressionNode ExpressionNode;
typedef struct FunctionCallArgNode FunctionCallArgNode;


enum FunctionDeclarationType{
	NoArgs,
	WithArgs,
	NoArgsVoid,
	WithArgsVoid
}

typedef struct FunctionDeclarationNode{
	FunctionDeclarationType * type;

	DataType * functionType;
	Variable variable;
	CodeBlockNode * codeBlock;
	FunctionCallArgNode * functionArgs;
}FunctionDeclarationNode;


typedef struct CodeBlockNode{
	DeclarationNode * declarationNode;
	SpecialStatementNode * specialStatement;
	ExpressionNode * expression;
	ReturnStatementNode * returnStatement;
	IfElseStatmentNode * ifElse;
	ForStatementNode * forStatement;
	WhileStatement * whileStatement
	SwitchStatement * switchStatement;
	AssigmentNode * assingment;

	CodeBlockNode * codeBlock;

	// TODO: falta case
}CodeBlockNode;


typedef struct PointerNode{
	// * is implicit
	PointerNode * pointerNode;
}PointerNode;


enum DeclarationType{
	SingleDeclaration,
	ArrayDeclaration
}

typedef struct DeclarationNode{
	DeclarationType * type;
	SingleDeclarationNode * singleDeclarationNode;
	ArrayDeclarationNode * arrayDeclarationNode;
}DeclarationNode;

typedef struct SingleDeclarationNode{
	PointerNode * pointer;		// NULL
	DataType * dataType;
	VariableNode * variable;
	SingleInitializeNode * singleInitializeNode;
}SingleDeclarationNode;

enum AssignmentType{
	NoAssign,
	AssignSingle
}

typedef struct SingleInitializeNode{
	// ASSIGN, SEMI_COLON implicit
	AssignmentType * type;
	ExpressionNode * expressionNode; // NULL
}SingleInitializeNode;


typedef struct ArrayDeclarationNode{
	DataType * dataType;
	Variable variable;
	ArraySizeNode * arraySizeNode;
	ArrayInitializeNode * arrayInitializeNode;
}ArrayDeclarationNode;

enum ArraySizeType{
	NotSizedSingle,
	Sized,
	NotSizedMultiple,
	SizedMutilple
}

typedef struct ArraySizeNode{
	ArraySizeType * type;
	NumConstantIntNode * numberConstant;
	ArraySizeNode * arraySizeNode;	// NULL
}ArraySizeNode;

enum ArrayInitializeType{
	WithList,
	Empty
}

typedef struct ArrayInitializeNode{
	// ASSIGN OBRACE CBRACE SEMI_COLON implicit
	ArrayInitializeType * type;
	ArrayListNode * arrayListNode;
}ArrayInitializeNode;

typedef struct ArrayListNode{
	// COMA implicit depending on arrayListNode * != NULL
	NumConstantIntNode integer;
	ArrayListNode * arrayListNode;		// NULL
}ArrayListNode;


enum AssingmentType{
	ASSIGN, 
	SUM_ASSIGN,
	SUB_ASSIGN,
	MULT_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN
}

typedef struct AssigmentNode{
	AssingmentType * type;
	ExpressionNode * expressionNode;

	Variable variable;	// NULL
	ArrayDefinitionNode * arrayDefinitionNode;	// NULL
}AssigmentNode;


typedef struct ArrayDerefNode {				//Open y Close bracket son implicitos
	Variable * variable;
	SizeNode * sizeNode;
}ArrayDerefNode;

enum DataType {
	Int,
	Float,
	Dobule, 
	Long,
	Short,
	Char,
	VoidPointer
};

typedef struct ReturnStatementNode {			//return y ; son implicitos
	ExpressionNode * expressionNode;
}ReturnStatementNode;

typedef struct IfElseStatement {
	IfStatementNode * ifStatementNode;
	ElseStatementNode * elseStatementNode;
}IfElseStatement;

typedef struct IfStatementNode {				//if, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;

}IfStatementNode;

typedef struct ElseStatementNode {				//else, {, } son implicitos
	CodeBlockNode * codeBlockNode;
}ElseStatementNode;

typedef struct WhileStatementNode {			//while, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;
}WhileStatementNode;

typedef struct ForStatementNode {				//for, (, ), ;, {, } son implicitos
	DeclarationNode * declarationNode;
	ExpressionNode * expressionNode;
	AssigmentNode * assigmentNode;			//either one of these
	ExpressionNode * expressionNode;		//
	CodeBlockNode * codeBlockNode;
}ForStatementNode;

typedef struct SwitchStatementNode {				//switch, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;
}SwitchStatementNode;

typedef struct SizeNode {
	Variable * variable;
	NumConstantIntNode * numConstantIntNode;
}SizeNode;

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


typedef struct ExpressionNode {						//(, ) son implicitos
	ExpressionNodeType op;
	ExpressionNode * leftExpressionNode;
	ExpressionNode * rightExpressionNode;
	Variable * Variable;
	NumConstantIntNode * numConstantIntode;
	NumConstatnFloatNode * numConstantFloatNode;
	SpecialVariable * specialVariable;
	FunctionCallNode * functionCallNode;
	ArrayDerefNode * arrayDerefNode;
	StringNode * StringNode;
}ExpressionNode;

typedef struct FunctionCallNode {						//(, ) son implicitos
	Variable * Variable;
	FunctionCallArgNode * functionCallArgNode;		//puede ser null
}FunctionCallNode;


typedef struct FunctionCallArgNode {				//',' es implicito
	ExpressionNode * expressionNode;
	FunctionCallArgNode * functionCallArgNode;		//puede ser null
}FunctionCallArgNode;





#endif
