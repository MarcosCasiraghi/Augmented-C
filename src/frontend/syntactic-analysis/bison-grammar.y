%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semÃ¡nticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/
	

	// No-terminales (frontend).
	int program;
	

	//Variable variable;		creo que no va
	ReduceStatementNode * reduce_statement;
	FilterStatementNode * filter_statement;
	MapStatementNode * map_statement;
	ForeachStatementNode * foreach_statement;
	CreateStatementNode * create_statement;
	ReduceRangeStatementNode * reduce_range_statement;
	FilterRangeStatementNode * filter_range_statement;
	MapRangeStatementNode * map_range_statement;
	ForeachRangeStatementNode * foreach_range_statement;
	SelectorNode * selector;
	SpecialStatementNode * special_statement;
	Lambda * lambda;
	Lambda * boolean_lambda;
	CreateLambda * create_lambda;
	FunctionCallNode * function_call;
	StatementNode * statements;
	MetaCommandNode * meta_command;
	FunctionArgNode * function_arg;
	FunctionArgsNode * function_args;
	FunctionDeclarationNode * function_declaration;
	CodeBlockNode * code_block;
	PointerNode * pointers;
	DeclarationNode * declaration;
	SingleDeclarationNode * single_declaration;
	SingleInitializeNode * single_initialization;
	ArrayDeclarationNode * array_declaration;
	ArraySizeNode * array_declaration_size;
	ArrayListNode * array_list;
	ArrayInitializeNode * array_initialization;
	AssignmentNode * assignment;
	ArrayDerefNode * array_deref;
	ReturnStatementNode * return_statement;
	IfElseStatementNode * if_else_statement;
	IfStatementNode * if_statement;
	ElseStatementNode * else_statement;
	WhileStatementNode * while_statement;
	ForStatementNode * for_statement;
	SwitchStatementNode * switch_statement;
	SizeNode * size;
	ExpressionNode * expression;
	FunctionCallArgNode * function_call_arg;
	AssignmentType assignment_type;
	DataType data_type;
	NumConstantIntNode NUM_CONSTANT_INT;

	// Terminales.
	token token;

	char * num_constant_float;
	int num_int;
	char * STRING;
}


//  = = = = = = = = = = = =  Definicion de simbolos y otros  = = = = = = = = = = = = 

%token REDUCE MAP FILTER FOREACH CREATE REDUCERANGE MAPRANGE FILTERRANGE FOREACHRANGE START_SPECIAL END_SPECIAL 
%token <STRING> VARIABLE_NAME 
%token <STRING> SPECIAL_VARIABLE 
%token <num_constant_float> NUM_CONSTANT_FLOAT
%token <num_int> NUM_CONSTANT_INT
%token <STRING> STRING
%token <STRING> FILE_NAME

%token INT FLOAT DOUBLE LONG SHORT CHAR VOID

%token ADD_OP SUB_OP MULT_OP DIV_OP MOD_OP
%token INC_OP DEC_OP
%token BIT_NOT_OP BIT_RIGHT_OP BIT_LEFT_OP BIT_XOR_OP BIT_OR_OP BIT_AND_OP

%token ASSIGN SUM_ASSIGN SUB_ASSIGN MULT_ASSIGN DIV_ASSIGN MOD_ASSIGN

%token NOT_OP AND_OP OR_OP
%token EQ_OP GR_OP GE_OP LT_OP LE_OP NE_OP

%token COMA THREE_DOT COLON SEMI_COLON
%token OBRACKET CBRACKET OBRACE CBRACE OPAR CPAR 

%token NUMBER_SIGN INCLUDE RETURN IF WHILE FOR ELSE CONTINUE BREAK CASE DEFAULT SWITCH

// Tipos de dato para los no-terminales generados desde Bison.
%type <array_deref> array_deref
%type <return_statement> return_statement
%type <if_else_statement> if_else_statement
%type <if_statement> if_statement
%type <else_statement> else_statement
%type <while_statement> while_statement
%type <for_statement> for_statement
%type <switch_statement> switch_statement
%type <expression> expression
%type <function_call_arg> function_call_arg
%type <reduce_statement> reduce_statement
%type <filter_statement> filter_statement
%type <map_statement> map_statement
%type <foreach_statement> foreach_statement
%type <create_statement> create_statement
%type <reduce_range_statement> reduce_range_statement
%type <filter_range_statement> filter_range_statement
%type <map_range_statement> map_range_statement
%type <foreach_range_statement> foreach_range_statement
%type <selector> selector
%type <special_statement> special_statement
%type <size> size
%type <lambda> lambda
%type <boolean_lambda> boolean_lambda
%type <create_lambda> create_lambda
%type <function_call> function_call
%type <statements> statements
%type <meta_command> meta_command
%type <function_arg> function_arg
%type <function_args> function_args
%type <function_declaration> function_declaration
%type <code_block> code_block
%type <pointers> pointers
%type <declaration> declaration
%type <single_declaration> single_declaration
%type <single_initialization> single_initialization
%type <array_declaration> array_declaration
%type <array_declaration_size> array_declaration_size
%type <array_list> array_list
%type <array_initialization> array_initialization
%type <assignment> assignment
%type <assignment_type> assignment_type
%type <data_type> data_type



//  = = = = = = = = = = = = Reglas de asociatividad y precedencia  = = = = = = = = = = = = 

%left COMA
%left ASSIGN SUM_ASSIGN SUB_ASSIGN MULT_ASSIGN DIV_ASSIGN MOD_ASSIGN
%left OR_OP
%left AND_OP
%left BIT_OR_OP
%left BIT_XOR_OP
%left BIT_AND_OP
%left EQ_OP NE_OP
%left GR_OP GE_OP LT_OP LE_OP
%left BIT_LEFT_OP BIT_RIGHT_OP
%left ADD_OP SUB_OP
%left MULT_OP DIV_OP MOD_OP
%left BIT_NOT_OP NOT_OP
%left INC_OP DEC_OP


//  = = = = = = = = = = = =  Inicio de gramatica  = = = = = = = = = = = = 

%start program

%%

program: statements 																			{ ProgramAction($1); }
		;

// = = = = = = = = = = = =  Lambda  = = = = = = = = = = = = = = = = 

special_statement : START_SPECIAL selector END_SPECIAL											{ $$ = specialStatementAction($2); };		

selector:     REDUCE COMA reduce_statement 														{ $$ = ReduceStatementSelectorAction($3); }
			| FILTER COMA filter_statement 														{ $$ = FilterStatementSelectorAction($3); }
			| FOREACH COMA foreach_statement 													{ $$ = ForeachStatementSelectorAction($3);}		
			| MAP COMA map_statement 															{ $$ = MapStatementSelectorAction($3); }		
			| CREATE COMA create_statement 														{ $$ = CreateStatementSelectorAction($3); }
			| REDUCERANGE COMA reduce_range_statement											{ $$ = ReduceRangeStatementSelectorAction($3); }
			| FILTERRANGE COMA filter_range_statement 											{ $$ = FilterRangeStatementSelectorAction($3); }
			| FOREACHRANGE COMA foreach_range_statement											{ $$ = ForeachRangeStatementSelectorAction($3); }
			| MAPRANGE COMA map_range_statement													{ $$ = MapRangeStatementSelectorAction($3); }
			;


reduce_statement: VARIABLE_NAME COMA size COMA VARIABLE_NAME COMA lambda 									{ $$ = ReduceStatementAction($1, $3, $5, $7); }
			;
filter_statement: VARIABLE_NAME COMA size COMA VARIABLE_NAME COMA boolean_lambda							{ $$ = FilterStatementAction($1, $3, $5, $7); }
			;
map_statement: VARIABLE_NAME COMA size COMA VARIABLE_NAME COMA lambda 									{ $$ = MapStatementAction($1, $3, $5, $7); }
			;
foreach_statement: VARIABLE_NAME COMA size COMA OBRACE function_call CBRACE							{ $$ = ForeachStatementAction($1, $3, $6); }
			;				
create_statement: VARIABLE_NAME COMA data_type COMA create_lambda 									{ $$ = CreateStatementAction($1, $3, $5); }
			;
reduce_range_statement: VARIABLE_NAME COMA size COMA size COMA VARIABLE_NAME COMA lambda					{ $$ = ReduceRangeStatementAction($1, $3, $5, $7, $9); }
			;
filter_range_statement: VARIABLE_NAME COMA size COMA size COMA VARIABLE_NAME COMA boolean_lambda			{ $$ = FilterRangeStatementAction($1, $3, $5, $7, $9); }
			;
map_range_statement: VARIABLE_NAME COMA size COMA size COMA VARIABLE_NAME COMA lambda						{ $$ = MapRangeStatementAction($1, $3, $5, $7, $9); }
			;
foreach_range_statement: VARIABLE_NAME COMA size COMA size COMA OBRACE function_call CBRACE			{ $$ = ForeachRangeStatementAction($1, $3, $5, $8);}
			;

lambda: OBRACE expression CBRACE 																{ $$ = LambdaAction($2); }
			;
boolean_lambda: OBRACE expression CBRACE 														{ $$ = LambdaAction($2);}
			;
create_lambda : OBRACE NUM_CONSTANT_INT THREE_DOT NUM_CONSTANT_INT CBRACE 						{ $$ = CreateLambdaAction($2, $4); }
			;


// = = = = = = = = = = = =  C Lang  = = = = = = = = = = = = = = = = 

statements: meta_command statements																{ $$ = MetacommandWithStatementNodeAction($2, $1); }		
		| function_declaration statements														{ $$ = FunctionDeclarationWithStatementNodeAction($2, $1); }
		| declaration statements																{ $$ = DeclarationWithStatementNodeAction($2, $1); }
		| meta_command																			{ $$ = MetaCommandNodeAction($1); }
		| function_declaration																	{ $$ = FunctionDeclarationNodeAction($1); }
		| declaration																			{ $$ = DeclarationStatementNodeAction($1); }
		;

meta_command: NUMBER_SIGN INCLUDE STRING 														{ $$ = StringMetaCommandAction($3); }
			| NUMBER_SIGN INCLUDE LT_OP FILE_NAME GR_OP											{ $$ = FileNameMetaCommandAction($4); }
			;

function_arg: data_type VARIABLE_NAME 																{ $$ = NoPointerFunctionArgAction($1, $2); }
			| data_type pointers VARIABLE_NAME 														{ $$ = PointerFunctionArgAction($1, $2, $3); }
			;
function_args: 	function_arg 																		{ $$ = SingleFunctionArgsAction($1); }
			 | function_arg COMA function_args														{ $$ = MultipleFunctionArgsAction($1, $3); }
			 ;

function_declaration: data_type VARIABLE_NAME OPAR CPAR OBRACE code_block CBRACE							{ $$ = FunctionDeclarationNoArgsAction($1, $2, $6); }
					| data_type VARIABLE_NAME OPAR function_args CPAR OBRACE code_block CBRACE			{ $$ = FunctionDeclarationWithArgsAction($1, $2, $7, $4); }
					| VOID VARIABLE_NAME OPAR CPAR OBRACE code_block CBRACE								{ $$ = VoidFunctionDeclarationAction($2, $6); }
					| VOID VARIABLE_NAME OPAR function_args CPAR OBRACE code_block CBRACE				{ $$ = VoidFunctionDeclarationWithArgsAction($2, $7, $4); }


code_block: declaration code_block																{ $$ = DeclarationCodeBlockActionWithChild($1, $2); }
		|	special_statement code_block														{ $$ = SpecialStatementCodeBlockActionWithChild($1, $2); }
		|	expression SEMI_COLON code_block													{ $$ = ExpressionCodeBlockActionWithChild($1, $3); }
		| 	return_statement code_block															{ $$ = ReturnCodeBlockActionWithChild($1, $2); }
		|	if_else_statement code_block														{ $$ = IfElseCodeBlockActionWithChild($1, $2); }
		|	for_statement code_block															{ $$ = ForCodeBlockActionWithChild($1, $2); }
		|	while_statement code_block															{ $$ = WhileCodeBlockActionWithChild($1, $2); }
		|	switch_statement code_block															{ $$ = SwitchCodeBlockActionWithChild($1, $2); }
		|	assignment SEMI_COLON code_block													{ $$ = AssignmentCodeBlockActionWithChild($1, $3); }

		| 	CONTINUE SEMI_COLON	code_block														{ $$ = ContinueCodeBlockAction($3); } // WARNING: only allowed within a while or for loop
		|	BREAK SEMI_COLON code_block															{ $$ = BreakCodeBlockAction($3); } // WARNING: only allowed in while, for or switch

		| 	CASE expression COLON code_block													{ $$ = CaseCodeBlockAction($2, $4); } 
		|	DEFAULT COLON code_block															{ $$ = DefaultCaseCodeBlockAction($3); }

		|	declaration																			{ $$ = DeclarationCodeBlockAction($1); }		
		|	special_statement																	{ $$ = SpecialStatementCodeBlockAction($1); }
		|	expression SEMI_COLON																{ $$ = ExpressionCodeBlockAction($1); }
		|	return_statement																	{ $$ = ReturnCodeBlockAction($1); }
		|	if_else_statement																	{ $$ = IfElseCodeBlockAction($1); }
		|	for_statement																		{ $$ = ForCodeBlockAction($1); }
		|	while_statement																		{ $$ = WhileCodeBlockAction($1); }
		|	switch_statement																	{ $$ = SwitchCodeBlockAction($1); }
		|	assignment SEMI_COLON																{ $$ = AssignmentCodeBlockAction($1); }
		| 	CONTINUE SEMI_COLON			/* WARNING: only allowed within a while or for loop*/	{ $$ = ContinueAction(); }	
		|	BREAK SEMI_COLON			/* WARNING: only allowed in while, for or switch*/		{ $$ = BreakAction(); }


pointers: MULT_OP 																				{ $$ = PointerAction(); }
		| MULT_OP pointers																		{ $$ = PointerActionWithChild($2); }
		;

declaration: single_declaration 																{ $$ = DeclarationOfSingleAction($1); }
			| array_declaration																	{ $$ = DeclarationOfArrayAction($1); }
			;

single_declaration: data_type VARIABLE_NAME single_initialization									{ $$ = SingleWithoutPointerDeclarationAction($1, $2, $3); } // different from assignment since you cannot do: int var += 3;
				| data_type pointers VARIABLE_NAME single_initialization								{ $$ = SingleWithPointerDeclarationAction($2, $1, $3, $4); }
				;
single_initialization: ASSIGN expression SEMI_COLON 											{ $$ = SingleInitializationWithAssignAction($2); }
					| SEMI_COLON																{ $$ = SingleInitializationWithoutAssignAction(); } 
					; 

array_declaration: data_type VARIABLE_NAME array_declaration_size array_initialization				{ $$ = ArrayDeclarationAction($1, $2, $3, $4); }
					;
array_declaration_size: OBRACKET CBRACKET array_declaration_size								{ $$ = ArraySizeWithoutSizeWithChildrenAction($3); }					
					|   OBRACKET NUM_CONSTANT_INT CBRACKET array_declaration_size				{ $$ = ArraySizeWithSizeWithChildrenAction($2, $4);}
					| 	OBRACKET NUM_CONSTANT_INT CBRACKET										{ $$ = ArraySizeWithSizeWithoutChildrenAction($2); }
					|	OBRACKET CBRACKET														{ $$ = ArraySizeWithoutSizeNorChildrenAction(); }
					;
array_list: NUM_CONSTANT_INT COMA array_list													{ $$ = ArrayListManyAction($1, $3); }
			| NUM_CONSTANT_INT																	{ $$ = ArrayListAction($1); }
			;
array_initialization: ASSIGN OBRACE array_list CBRACE SEMI_COLON 								{ $$ = ArrayInitializeWithListAction($3); }
					| SEMI_COLON																{ $$ = ArrayInitializeEmptyAction();}
					;

assignment_type: ASSIGN 																		{ $$ = AssignAction();}
				| SUM_ASSIGN 																	{ $$ = SumAssignAction(); }
				| SUB_ASSIGN 																	{ $$ = SubAssignAction(); }
				| MULT_ASSIGN 																	{ $$ = MultAssignAction(); }
				| DIV_ASSIGN 																	{ $$ = DivAssignAction(); }
				| MOD_ASSIGN																	{ $$ = ModAssignAction(); }
				;
assignment: VARIABLE_NAME assignment_type expression 												{ $$ = AssignmentWithVarAction($1, $2, $3); }
		| array_deref assignment_type expression												{ $$ = AssignmentWithArrayDerefAction($1, $2, $3); }
		;



array_deref: VARIABLE_NAME OBRACKET size CBRACKET													{ $$ = ArrayDerefAction($1, $3); }
			;


data_type: INT 																					{ $$ = IntAction(); }
		| FLOAT 																				{ $$ = FloatAction();}
		| DOUBLE 																				{ $$ = DoubleAction(); }
		| LONG 																					{ $$ = LongAction(); }
		| SHORT 																				{ $$ = ShortAction(); }
		| CHAR 																					{ $$ = CharAction();}
		| VOID MULT_OP																			{$$ = VoidPointerAction(); }
		;

return_statement: RETURN expression SEMI_COLON													{ $$ = ReturnStatementAction($2); }
	;

if_else_statement: if_statement 																{ $$ = IfWithoutElseStatementAction($1); }
				 | if_statement else_statement 													{ $$ = IfWithElseStatementAction($1, $2); }
				 ;
if_statement: IF OPAR expression CPAR OBRACE code_block CBRACE									{ $$ = IfStatementAction($3, $6); }
	;
else_statement: ELSE OBRACE code_block CBRACE													{ $$ = ElseStatementAction($3); }
	;
while_statement: WHILE OPAR expression CPAR OBRACE code_block CBRACE							{ $$ = WhileStatementAction($3, $6); }
	;


for_statement: FOR OPAR declaration expression SEMI_COLON assignment CPAR OBRACE code_block CBRACE		{ $$ = ForStatementWithAssigmentAction($3, $4, $6, $9); }
			|  FOR OPAR declaration expression SEMI_COLON expression CPAR OBRACE code_block CBRACE		{ $$ = ForStatementWithExpressionAction($3, $4, $6, $9); }
			;

switch_statement: SWITCH OPAR expression CPAR OBRACE code_block CBRACE 	 						{ $$ = SwitchStatementAction($3, $6); }


size: VARIABLE_NAME 																					{ $$ = SizeVarAction($1); }
	| NUM_CONSTANT_INT 																			{ $$ = SizeNumConstIntAction($1); }	
	;

expression:  expression ADD_OP expression 								{ $$ = AddOpExpressionAction($1, $3); }
			| expression SUB_OP expression 								{ $$ = SubOpExpressionAction($1, $3); }
			| expression MULT_OP expression 							{ $$ = MultOpExpressionAction($1, $3); }
			| expression DIV_OP expression 								{ $$ = DivOpExpressionAction($1, $3); }
			| expression MOD_OP expression 								{ $$ = ModOpExpressionAction($1, $3); }
			| expression INC_OP											{ $$ = IncOpRightExpressionAction($1); }
			| INC_OP expression											{ $$ = IncOpLeftExpressionAction($2); }
			| expression DEC_OP											{ $$ = DecOpRightExpressionAction($1); }
			| DEC_OP expression											{ $$ = DecOpLeftExpressionAction($2); }
			| BIT_NOT_OP expression										{ $$ = BitNotOpExpressionAction($2); }
			| expression BIT_RIGHT_OP expression						{ $$ = BitRightOpExpressionAction($1, $3); }
			| expression BIT_LEFT_OP expression							{ $$ = BitLeftOpExpressionAction($1, $3); }
			| expression BIT_XOR_OP expression							{ $$ = BitXorOpExpressionAction($1, $3); }
			| expression BIT_OR_OP expression							{ $$ = BitOrOpExpressionAction($1, $3); }
			| expression BIT_AND_OP expression							{ $$ = BitAndOpExpressionAction($1, $3); }

			| expression AND_OP expression								{ $$ = AndOpExpressionAction($1, $3); }
			| OPAR expression CPAR										{ $$ = ParenthesisExpressionAction($2); }
			| expression OR_OP expression								{ $$ = OrOpExpressionAction($1, $3); }
			| NOT_OP OPAR expression CPAR								{ $$ = NotOpExpressionAction($3); }

			| expression EQ_OP expression								{ $$ = EqOpExpressionAction($1, $3); }
			| expression GR_OP expression								{ $$ = GrOpExpressionAction($1, $3); }
			| expression GE_OP expression								{ $$ = GeOpExpressionAction($1, $3); }
			| expression LT_OP expression								{ $$ = LtOpExpressionAction($1, $3); }
			| expression LE_OP expression								{ $$ = LeOpExpressionAction($1, $3); }
			| expression NE_OP expression								{ $$ = NeOpExpressionAction($1, $3); }

			| VARIABLE_NAME 													{ $$ = variableOpExpressionAction($1); }
			| NUM_CONSTANT_FLOAT 										{ $$ = NumConstantFloatOpExpressionAction($1); }
			| NUM_CONSTANT_INT 											{ $$ = NumConstantIntOpExpressionAction($1); }
			| SPECIAL_VARIABLE 											{ $$ = SpecialVarOpExpressionAction($1); }
			| function_call 											{ $$ = FunctionCallOpExpressionAction($1); }
			| array_deref												{ $$ = ArrayDerefOpExpressionAction($1); }
			| STRING													{ $$ = StringOpExpressionAction($1); }
			;


function_call: VARIABLE_NAME OPAR function_call_arg CPAR 					{ $$ = WithArgsFunctionCallAction($1, $3);}
			| VARIABLE_NAME OPAR CPAR 										{ $$ = NoArgsFunctionCallAction($1); }
			;

function_call_arg: expression COMA function_call_arg					{ $$ = WithArgsFunctionCallArgAction($1, $3); }
			 | expression												{ $$ = NoArgsFunctionCallArgAction($1); }
			  ;


%%