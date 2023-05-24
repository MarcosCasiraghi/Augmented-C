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
typedef char * StringVar;
typedef char * Variable;
typedef char * NumConstantIntNode;
typedef char * NumConstantFloatNode;
typedef char * SpecialVariable;
typedef struct FunctionArgsNode FunctionArgsNode;
typedef struct CodeBlockNode CodeBlockNode;
typedef struct PointerNode PointerNode;
typedef struct ExpressionNode ExpressionNode;
typedef struct FunctionCallArgNode FunctionCallArgNode;
typedef struct StatementNode StatementNode;


struct ProgramNode {
	ExpressionNode * expressionNode;
};


typedef struct SpecialStatementNode {
	SelectorNode * selectorNode;
} SpecialStatementNode;

typedef struct SelectorNode {
	ReduceStatementNode * reduceStatement;
	FilterStatementNode * filterStatement;
	ForeachStatementNode * foreachStatement;
	MapStatementNode * mapStatement;
	CreateStatementNode * createStatement;
	ReduceRangeStatementNode * reduceRangeStatement;
	FilterRangeStatementNode * filterRangeStatement;
	ForeachRangeStatementNode * foreachRangeStatement;
	MapRangeStatementNode * mapRangeStatement;
} SelectorNode;

typedef struct Lambda {
	ExpressionNode * expressionNode;
} Lambda;

typedef struct CreateLambda {
	NumConstantIntNode * constant1;
	NumConstantIntNode * constant2;		// el ... es obviado
} CreateLambda;


typedef struct ReduceStatementNode {
	Variable variable1;
	NumConstantIntNode size;
	Variable variable2;
	Lambda * lambda;
} ReduceStatementNode;

typedef struct FilterStatementNode {
	Variable variable1;
	NumConstantIntNode size;
	Variable variable2;
	Lambda * lambda;
} FilterStatementNode;

typedef struct ForeachStatementNode {
	Variable variable1;
	NumConstantIntNode size;
	FunctionCallNode * functionCallNode;
} ForeachStatementNode;

typedef struct MapStatementNode {
	Variable variable1;
	NumConstantIntNode size;
	Variable variable2;
	CreateLambda * createLambda;
} MapStatementNode;

typedef struct CreateStatementNode {
	Variable variable1;
	NumConstantIntNode size;
	Variable variable2;
	Lambda * lambda;
} CreateStatementNode;

typedef struct ReduceRangeStatementNode {
	Variable variable1;
	NumConstantIntNode size1;
	NumConstantIntNode size2;
	Variable variable2;
	Lambda * lambda;
} ReduceRangeStatementNode;

typedef struct FilterRangeStatementNode {
	Variable variable1;
	NumConstantIntNode size1;
	NumConstantIntNode size2;
	Variable variable2;
	Lambda * lambda;
} FilterRangeStatementNode;

typedef struct ForeachRangeStatementNode {
	Variable variable1;
	NumConstantIntNode size1;
	NumConstantIntNode size2;
	FunctionCallNode * functionCallNode;
} ForeachRangeStatementNode;

typedef struct MapRangeStatementNode {
	Variable variable1;
	NumConstantIntNode size1;
	NumConstantIntNode size2;
	Variable variable2;
	CreateLambda * createLambda;
} MapRangeStatementNode;

typedef enum MetaCommandType {
	String,
	File_Name
} MetaCommandType;

typedef struct MetaCommandNode {
	MetaCommandType type;
} MetaCommandNode;

typedef enum StatementType {
	Statement,
	Pointer,
	FunctionDeclaration,
	Declaration
} StatementType;

typedef struct StatementNode {
	StatementType type;

	StatementNode *statement;
	MetaCommandNode metacommand;
	FunctionDeclarationNode functionDeclarationNode;
	DeclarationNode declarationNode;
} Statements;


typedef enum FunctionArgType {
	pointer,
	noPointer
} FunctionArgType;

typedef struct FunctionArgNode {
	FunctionArgType type;

	DataType dataType;
	PointerNode * pointer;
	Variable variable;
} FunctionArgNode;

typedef enum FunctionArgsType {
	single,
	multiple
} FunctionArgsType;

typedef struct FunctionArgsNode {
	FunctionArgsType type;

	FunctionArgNode * functionArgNode;
	FunctionArgsNode * functionArgsNode;
} FunctionArgsNode;

typedef enum FunctionDeclarationType{
	NoArgs,
	WithArgs,
	NoArgsVoid,
	WithArgsVoid
} FunctionDeclarationType;

typedef struct FunctionDeclarationNode{
	FunctionDeclarationType type;

	DataType functionType;
	Variable variable;
	CodeBlockNode * codeBlock;
	FunctionCallArgNode * functionArgs;
} FunctionDeclarationNode;


typedef struct CodeBlockNode{
	DeclarationNode * declarationNode;
	SpecialStatementNode * specialStatement;
	ExpressionNode * expression;
	ReturnStatementNode * returnStatement;
	IfElseStatementNode * ifElse;
	ForStatementNode * forStatement;
	WhileStatementNode * whileStatement;
	SwitchStatementNode * switchStatement;
	AssigmentNode * assingment;

	CodeBlockNode * codeBlock;

	// TODO: falta case
} CodeBlockNode;


typedef struct PointerNode{
	// * is implicit
	PointerNode * pointerNode;
} PointerNode;


typedef enum DeclarationType{
	SingleDeclaration,
	ArrayDeclaration
} DeclarationType;

typedef struct DeclarationNode{
	DeclarationType type;
	SingleDeclarationNode * singleDeclarationNode;
	ArrayDeclarationNode * arrayDeclarationNode;
} DeclarationNode;

typedef struct SingleDeclarationNode{
	PointerNode * pointer;		// NULL
	DataType dataType;
	Variable * variable;
	SingleInitializeNode * singleInitializeNode;
} SingleDeclarationNode;

typedef enum AssignmentType{
	NoAssign,
	AssignSingle
} AssignmentType;

typedef struct SingleInitializeNode{
	// ASSIGN, SEMI_COLON implicit
	AssignmentType type;
	ExpressionNode * expressionNode; // NULL
} SingleInitializeNode;


typedef struct ArrayDeclarationNode{
	DataType dataType;
	Variable variable;
	ArraySizeNode * arraySizeNode;
	ArrayInitializeNode * arrayInitializeNode;
} ArrayDeclarationNode;

typedef enum ArraySizeType{
	NotSizedSingle,
	Sized,
	NotSizedMultiple,
	SizedMutilple
} ArraySizeType;

typedef struct ArraySizeNode{
	ArraySizeType type;
	NumConstantIntNode * numberConstant;
	ArraySizeNode * arraySizeNode;	// NULL
} ArraySizeNode;

typedef enum ArrayInitializeType{
	WithList,
	Empty
} ArrayInitializeType;

typedef struct ArrayInitializeNode{
	// ASSIGN OBRACE CBRACE SEMI_COLON implicit
	ArrayInitializeType type;
	ArrayListNode * arrayListNode;
} ArrayInitializeNode;

typedef struct ArrayListNode{
	// COMA implicit depending on arrayListNode * != NULL
	NumConstantIntNode integer;
	ArrayListNode * arrayListNode;		// NULL
} ArrayListNode;


typedef enum AssingmentType{
	ASSIGN, 
	SUM_ASSIGN,
	SUB_ASSIGN,
	MULT_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN
} AssignmentType;

typedef struct AssigmentNode{
	AssignmentType type;
	ExpressionNode * expressionNode;

	Variable variable;	// NULL
	ArrayDerefNode * arrayDefinitionNode;	// NULL
} AssigmentNode;


typedef struct ArrayDerefNode {				//Open y Close bracket son implicitos
	Variable * variable;
	SizeNode * sizeNode;
} ArrayDerefNode;

typedef enum DataType {
	Int,
	Float,
	Dobule, 
	Long,
	Short,
	Char,
	VoidPointer
} DataType;

typedef struct ReturnStatementNode {			//return y ; son implicitos
	ExpressionNode * expressionNode;
} ReturnStatementNode;

typedef struct IfElseStatementNode {
	IfStatementNode * ifStatementNode;
	ElseStatementNode * elseStatementNode;
} IfElseStatementNode;

typedef struct IfStatementNode {				//if, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;

} IfStatementNode;

typedef struct ElseStatementNode {				//else, {, } son implicitos
	CodeBlockNode * codeBlockNode;
} ElseStatementNode;

typedef struct WhileStatementNode {			//while, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;
} WhileStatementNode;

typedef struct ForStatementNode {				//for, (, ), ;, {, } son implicitos
	DeclarationNode * declarationNode;
	ExpressionNode * expressionNode;
	AssigmentNode * assigmentNode;			//either one of these
	ExpressionNode * expressionNode;		//
	CodeBlockNode * codeBlockNode;
} ForStatementNode;

typedef struct SwitchStatementNode {				//switch, (, ), {, } son implicitos
	ExpressionNode * expressionNode;
	CodeBlockNode * codeBlockNode;
} SwitchStatementNode;

typedef struct SizeNode {
	Variable * variable;
	NumConstantIntNode * numConstantIntNode;
} SizeNode;

typedef enum ExpressionNodeType{
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
	NeOp,
	variable,
	NumConstantFloat,
	NumConstantInt,
	specialVariable,
	ArrayDeref,
	String
} ExpressionNodeType;


typedef struct ExpressionNode {						//(, ) son implicitos
	ExpressionNodeType op;
	ExpressionNode * leftExpressionNode;
	ExpressionNode * rightExpressionNode;
	Variable * Variable;
	NumConstantIntNode * numConstantIntode;
	NumConstantFloatNode * numConstantFloatNode;
	SpecialVariable * specialVariable;
	FunctionCallNode * functionCallNode;
	ArrayDerefNode * arrayDerefNode;
	StringVar * StringNode;
}ExpressionNode;

typedef enum FunctionCallType {
	NoArgs,
	WithArgs
}FunctionCallType;

typedef struct FunctionCallNode {						//(, ) son implicitos
	FunctionCallType type;

	Variable * Variable;
	FunctionCallArgNode * functionCallArgNode;		//puede ser null
}FunctionCallNode;

typedef enum FunctionCallArgType {
	NoArgs,
	WithArgs
}FunctionCallArgType;

typedef struct FunctionCallArgNode {				//',' es implicito
	FunctionCallArgType type;

	ExpressionNode * expressionNode;
	FunctionCallArgNode * functionCallArgNode;		//puede ser null
}FunctionCallArgNode;





#endif
