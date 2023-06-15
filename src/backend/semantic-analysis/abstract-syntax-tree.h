#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

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
typedef int NumConstantIntNode;
typedef char * NumConstantFloatNode;
typedef char * SpecialVariable;
typedef struct FunctionArgsNode FunctionArgsNode;
typedef struct CodeBlockNode CodeBlockNode;
typedef struct PointerNode PointerNode;
typedef struct ExpressionNode ExpressionNode;
typedef struct FunctionCallArgNode FunctionCallArgNode;
typedef struct StatementNode StatementNode;
typedef struct ArrayListNode ArrayListNode;
typedef struct ArraySizeNode ArraySizeNode;
typedef struct SpecialStatementNode SpecialStatementNode;


/*
*   In many cases, in to distinguish between nodes
*   we only need to know if it has a child or not.
*   Use this instead of creating a new enum.
*/
typedef enum Child{
    HasChild,
    NoChild
}Child;

// - - - - - - Includes  - - - - - -

typedef enum MetaCommandType {
    MetaCommandString,
    MetaCommandFileName
} MetaCommandType;

typedef struct MetaCommandNode {
    MetaCommandType type;
    StringVar string;
    
} MetaCommandNode;

// - - - - - - Dereferencing  - - - - - -

typedef enum SizeNodeType {
    VariableSize,
    ConstantSize
} SizeNodeType;

typedef struct SizeNode {
    SizeNodeType type;
    Variable variable;
    NumConstantIntNode numConstantIntNode;
    
} SizeNode;

typedef struct ArrayDerefNode {             //Open y Close bracket son implicitos
    Variable variable;
    SizeNode * sizeNode;
    
} ArrayDerefNode;

// - - - - - - Declarations of Variables  - - - - - -

typedef enum DataType {
    Int,
    Float,
    Double,
    Long,
    Short,
    Char,
    VoidPointer,
    Void                    // only used for functions
} DataType;

typedef struct PointerNode{ // * is implicit
    Child child;
    PointerNode * pointerNode;
    
} PointerNode;

typedef enum AssignmentType{
    NoAssign,
    AssignSingle
} AssignmentType;

typedef struct SingleInitializeNode{    // =, ; implicit
    AssignmentType type;
    ExpressionNode * expressionNode; // NULL
    
} SingleInitializeNode;

typedef enum SingleDeclarationType{
    SingleWithPointer,
    SingleWithoutPointer
}SingleDeclarationType;

typedef struct SingleDeclarationNode{
    SingleDeclarationType type;

    PointerNode * pointer;      // NULL
    DataType dataType;
    Variable variable;
    SingleInitializeNode * singleInitializeNode;
    
} SingleDeclarationNode;

typedef enum ArrayInitializeType{
    WithList,
    Empty
} ArrayInitializeType;

typedef struct ArrayInitializeNode{     // =,  {,  }, ;  implicit
    ArrayInitializeType type;
    ArrayListNode * arrayListNode;      // NULL
    
} ArrayInitializeNode;

typedef struct ArrayListNode{           // , implicit depending on hasChild
    Child child;

    NumConstantIntNode integer;

    ArrayListNode * arrayListNode;      // NULL
    
} ArrayListNode;

typedef enum ArraySizeType{
    NotSizedSingle,
    Sized
} ArraySizeType;

typedef struct ArraySizeNode{           // [, ],  implicit
    ArraySizeType type;
    Child child;

    NumConstantIntNode numberConstant;    // NULL
    ArraySizeNode * arraySizeNode;  // NULL
    
} ArraySizeNode;

typedef struct ArrayDeclarationNode{
    DataType dataType;
    Variable variable;
    ArraySizeNode * arraySizeNode;
    ArrayInitializeNode * arrayInitializeNode;
    
} ArrayDeclarationNode;

typedef enum DeclarationType{
    SingleDeclaration,
    ArrayDeclaration
} DeclarationType;

typedef struct DeclarationNode{
    DeclarationType type;
    SingleDeclarationNode * singleDeclarationNode;
    ArrayDeclarationNode * arrayDeclarationNode;
    
} DeclarationNode;

typedef enum AssingmentType{
    ASSIGN_TYPE,
    SUM_ASSIGN_TYPE,
    SUB_ASSIGN_TYPE,
    MULT_ASSIGN_TYPE,
    DIV_ASSIGN_TYPE,
    MOD_ASSIGN_TYPE
} AssingmentType;

typedef enum AssignmentNodeType{
    withVar,
    withArrayDeref
}AssignmentNodeType;

typedef struct AssignmentNode{
    AssignmentNodeType withType;
    AssignmentType assignmentType;

    ExpressionNode * expressionNode;

    Variable variable;                          // NULL
    ArrayDerefNode * arrayDefinitionNode;       // NULL
    
} AssignmentNode;

// - - - - - - Expression and Derivatives  - - - - - -

typedef enum FunctionCallType {
    NoArgs,
    WithArgs
}FunctionCallType;

typedef struct FunctionCallNode {                       //(, ) son implicitos
    FunctionCallType type;

    Variable Variable;
    FunctionCallArgNode * functionCallArgNode;      //puede ser null
    
}FunctionCallNode;

typedef enum FunctionCallArgType {
    FunctionCallWithNoArgs,
    FunctionCallWithArgs
}FunctionCallArgType;

typedef struct FunctionCallArgNode {                //',' es implicito
    FunctionCallArgType type;

    ExpressionNode * expressionNode;
    FunctionCallArgNode * functionCallArgNode;      //puede ser null
    
}FunctionCallArgNode;

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
    VariableType,
    NumConstantFloat,
    NumConstantInt,
    specialVariable,
    functionCall,
    ArrayDeref,
    String,
    WithParenthesis
} ExpressionNodeType;

typedef struct ExpressionNode {                     //(, ) son implicitos
    ExpressionNodeType op;
    ExpressionNode * leftExpressionNode;
    ExpressionNode * rightExpressionNode;
    Variable Variable;
    NumConstantIntNode numConstantIntNode;
    NumConstantFloatNode numConstantFloatNode;
    FunctionCallNode * functionCallNode;
    ArrayDerefNode * arrayDerefNode;
    StringVar StringNode;
    
}ExpressionNode;

// - - - - - - Scope Code  - - - - - -

typedef struct ReturnStatementNode {            //return y ; son implicitos
    ExpressionNode * expressionNode;
    
} ReturnStatementNode;

typedef struct IfStatementNode {                //if, (, ), {, } son implicitos
    ExpressionNode * expressionNode;
    CodeBlockNode * codeBlockNode;
    
} IfStatementNode;

typedef struct ElseStatementNode {              //else, {, } son implicitos
    CodeBlockNode * codeBlockNode;
    
} ElseStatementNode;

typedef enum IfElseStatementType {
    withElse,
    withoutElse
}IfElseStatementType;

typedef struct IfElseStatementNode {
    IfElseStatementType type;
    IfStatementNode * ifStatementNode;
    ElseStatementNode * elseStatementNode;
    
} IfElseStatementNode;

typedef struct WhileStatementNode {         //while, (, ), {, } son implicitos
    ExpressionNode * expressionNode;
    CodeBlockNode * codeBlockNode;
    
} WhileStatementNode;

typedef enum ForStatementType{
    withExpression,
    withAssignment
}ForStatementType;

typedef struct ForStatementNode {               //for, (, ), ;, {, } son implicitos
    ForStatementType type;
    DeclarationNode * declarationNode;
    ExpressionNode * firstExpressionNode;
    AssignmentNode * AssignmentNode;          //either one of these
    ExpressionNode * expressionNode;        //
    CodeBlockNode * codeBlockNode;
    
} ForStatementNode;

typedef struct SwitchStatementNode {                //switch, (, ), {, } son implicitos
    ExpressionNode * expressionNode;
    CodeBlockNode * codeBlockNode;
    
} SwitchStatementNode;

typedef enum CodeBlockType{
    DeclarationStatement,
    SpecialStatement,
    ExpressionStatement,                    // ; is implicit
    ReturnStatement,
    IfElseStatement,
    ForStatement,
    WhileStatement,
    SwitchStatement,
    AssignmentStatement,
    CaseStatement,              // : is implicit
    DefaultCaseStatement,       // : is implicit
    ContinueStatement,          // ; is implicit
    BreakStatement,             // ; is implicit
}CodeBlockType;

typedef struct CodeBlockNode{
    CodeBlockType type;
    Child child;

    DeclarationNode * declarationNode;
    SpecialStatementNode * specialStatement;
    ExpressionNode * expression;            //  used in expression and used in CASE, DEAFU; Also, child will always be = HasChild
    ReturnStatementNode * returnStatement;
    IfElseStatementNode * ifElse;
    ForStatementNode * forStatement;
    WhileStatementNode * whileStatement;
    SwitchStatementNode * switchStatement;
    AssignmentNode * assingment;
    ExpressionNode * expressionNode;

    CodeBlockNode * codeBlock;
    
} CodeBlockNode;

// - - - - - - Function Declaration  - - - - - -

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
    FunctionDeclarationNoArgs,
    FunctionDeclarationWithArgs,
    FunctionDeclarationNoArgsVoid,
    FunctionDeclarationWithArgsVoid
} FunctionDeclarationType;

typedef struct FunctionDeclarationNode{
    FunctionDeclarationType type;

    DataType functionType;
    Variable variable;
    CodeBlockNode * codeBlock;
    FunctionArgsNode * functionArgs;
    
} FunctionDeclarationNode;

// - - - - - - C Statements  - - - - - -
// These encapsulate all statements you can do in C

typedef enum StatementType {
    MetaCommand,
    FunctionDeclaration,
    Declaration,
} StatementType;

typedef struct StatementNode {
    StatementType type;
    Child child;

    MetaCommandNode * metacommand;
    FunctionDeclarationNode * functionDeclarationNode;
    DeclarationNode * declarationNode;

    StatementNode * statement;
    
} StatementNode;

// - - - - - - Program  - - - - - -

typedef struct ProgramNode{
    StatementNode * statementNode;
    
}ProgramNode;



/* = = = = = = =  SPECIAL STATEMENT  = = = = = = = */

// - - - - - - Special Statements  - - - - - -

typedef struct RangeNode {
    SizeNode * sizeNode1;
    SizeNode * sizeNode2;
    
}RangeNode;

typedef struct ConsumerFunctionNode{
    FunctionCallNode * functionCallNode;
    
}ConsumerFunctionNode;

typedef struct UnboundedParametersNode {
    Variable variable1;
    SizeNode * SizeNode;
    Variable variable2;
    
}UnboundedParametersNode;

typedef struct BoundedParametersNode{
    Variable variable1;
    RangeNode * rangeNode;
    Variable variable2;
    
}BoundedParametersNode;

typedef struct Lambda {
    ExpressionNode * expressionNode;
    
} Lambda;

typedef struct CreateLambda {
    NumConstantIntNode constant1;
    NumConstantIntNode constant2;     // el ... es obviado
    
} CreateLambda;

typedef struct ReduceStatementNode {
    UnboundedParametersNode * unboundedParametersNode;
    Lambda * lambda;
    
} ReduceStatementNode;

typedef struct FilterStatementNode {
    UnboundedParametersNode * unboundedParametersNode;
    Lambda * lambda;
    
} FilterStatementNode;

typedef struct ForeachStatementNode {
    Variable variable;
    SizeNode * sizeNode;
    ConsumerFunctionNode * consumerFunctionNode;
    
} ForeachStatementNode;

typedef struct MapStatementNode {
    UnboundedParametersNode * unboundedParametersNode;
    Lambda * lambda;
    
} MapStatementNode;

typedef struct CreateStatementNode {
    Variable variable1;
    DataType dataType;
    CreateLambda * createLambda;
    
} CreateStatementNode;

typedef struct ReduceRangeStatementNode {
    BoundedParametersNode * boundedParametersNode;
    Lambda * lambda;
    
} ReduceRangeStatementNode;

typedef struct FilterRangeStatementNode {
    BoundedParametersNode * boundedParametersNode;
    Lambda * lambda;
    
} FilterRangeStatementNode;

typedef struct ForeachRangeStatementNode {
    Variable variable;
    RangeNode * rangeNode;
    ConsumerFunctionNode * consumerFunctionNode;
    
} ForeachRangeStatementNode;

typedef struct MapRangeStatementNode {
    BoundedParametersNode * boundedParametersNode;
    Lambda * lambda;
    
} MapRangeStatementNode;

typedef enum SelectorNodeType{
    ReduceStatement,
    FilterStatement,
    ForeachStatement,
    MapStatement,
    CreateStatement,
    ReduceRangeStatement,
    FilterRangeStatement,
    ForeachRangeStatement,
    MapRangeStatement
} SelectorNodeType;

typedef struct SelectorNode {
    SelectorNodeType selectorNodeType;

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

typedef struct SpecialStatementNode {
    SelectorNode * selectorNode;
    
} SpecialStatementNode;

#endif
