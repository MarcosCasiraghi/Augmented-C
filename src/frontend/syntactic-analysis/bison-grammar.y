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

	// Terminales.
	token token;
}

//  = = = = = = = = = = = =  Definicion de simbolos y otros  = = = = = = = = = = = = 

%token REDUCE MAP FILTER FOREACH CREATE REDUCERANGE MAPRANGE FILTERRANGE FOREACHRANGE START_SPECIAL END_SPECIAL 

%token INT FLOAT DOUBLE LONG SHORT CHAR VOID
%token VARIABLE_NAME NUM_CONSTANT_FLOAT NUM_CONSTANT_INT SPECIAL_VARIABLE STRING
%token FILE_NAME

%token ADD_OP SUB_OP MULT_OP DIV_OP MOD_OP
%token INC_OP DEC_OP
%token BIT_NOT_OP BIT_RIGHT_OP BIT_LEFT_OP BIT_XOR_OP BIT_OR_OP BIT_AND_OP

%token ASSIGN SUM_ASSIGN SUB_ASSIGN MULT_ASSIGN DIV_ASSIGN MOD_ASSIGN

%token NOT_OP AND_OP OR_OP
%token EQ_OP GR_OP GE_OP LT_OP LE_OP NE_OP

%token COMA THREE_DOT COLON SEMI_COLON
%token OBRACKET CBRACKET OBRACE CBRACE OPAR CPAR 

%token NUMBER_SIGN INCLUDE RETURN IF WHILE FOR ELSE CONTINUE BREAK CASE DEFAULT SWITCH

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

program: statements { ProgramGrammarAction(); };

// = = = = = = = = = = = =  Lambda  = = = = = = = = = = = = = = = = 

special_statement : START_SPECIAL selector END_SPECIAL 	;

selector:     REDUCE COMA reduce_statement 
			| FILTER COMA filter_statement 
			| FOREACH COMA foreach_statement 
			| MAP COMA map_statement 
			| CREATE COMA create_statement 
			| REDUCERANGE COMA reduce_range_statement
			| FILTERRANGE COMA filter_range_statement 
			| FOREACHRANGE COMA foreach_range_statement
			| MAPRANGE COMA map_range_statement ;


reduce_statement: variable COMA size COMA variable COMA lambda 	;
filter_statement: variable COMA size COMA variable COMA boolean_lambda 	;
map_statement: variable COMA size COMA variable COMA lambda 	;
foreach_statement: variable COMA size COMA OBRACE function_call CBRACE			;
create_statement: variable COMA data_type COMA create_lambda 	;
reduce_range_statement: variable COMA size COMA size COMA variable COMA lambda	;
filter_range_statement: variable COMA size COMA size COMA variable COMA boolean_lambda	;
map_range_statement: variable COMA size COMA size COMA variable COMA lambda	;
foreach_range_statement: variable COMA size COMA size COMA OBRACE function_call CBRACE	;

lambda: OBRACE expression CBRACE ;
boolean_lambda: OBRACE expression CBRACE ;
create_lambda : OBRACE NUM_CONSTANT_INT THREE_DOT NUM_CONSTANT_INT CBRACE ;


// = = = = = = = = = = = =  C Lang  = = = = = = = = = = = = = = = = 

statements: meta_command statements
		| function_declaration statements
		| declaration statements
		| meta_command
		| function_declaration
		| declaration

meta_command: NUMBER_SIGN INCLUDE STRING 
			| NUMBER_SIGN INCLUDE LT_OP FILE_NAME GR_OP

function_arg: data_type variable 
			| data_type pointers variable
function_args: function_arg | function_arg COMA function_args

function_declaration: data_type variable OPAR CPAR OBRACE code_block CBRACE
					| data_type variable OPAR function_args CPAR OBRACE code_block CBRACE
					| VOID variable OPAR CPAR OBRACE code_block CBRACE
					| VOID variable OPAR function_args CPAR OBRACE code_block CBRACE

code_block: declaration code_block
		|	special_statement code_block
		|	expression SEMI_COLON code_block
		| 	return_statement code_block
		|	if_else_statment code_block
		|	for_statement code_block
		|	while_statement code_block
		|	switch_statement code_block
		|	assigment SEMI_COLON code_block

		| 	CONTINUE SEMI_COLON	code_block			// WARNING: only allowed within a while or for loop
		|	BREAK SEMI_COLON	code_block			// WARNING: only allowed in while, for or switch

		| 	CASE expression COLON code_block		// WARNING: only allowed in switch. Expression should only allow: variable, NUM_CONSTANT_FLOAT, function_call, string
		|	DEFAULT COLON code_block				// WARNING: only allowed in switch. Expression should only allow: variable, NUM_CONSTANT_FLOAT, function_call, string

		|	declaration
		|	special_statement
		|	expression SEMI_COLON
		|	return_statement
		|	if_else_statment
		|	for_statement
		|	while_statement
		|	switch_statement
		|	assigment SEMI_COLON

		| 	CONTINUE SEMI_COLON			// WARNING: only allowed within a while or for loop
		|	BREAK SEMI_COLON			// WARNING: only allowed in while, for or switch


pointers: MULT_OP | MULT_OP pointers ;

declaration: single_declaration | array_declaration ;

single_declaration: data_type variable single_inicialization			// different from assignment since you cannot do: int var += 3;
				| data_type pointers variable single_inicialization
single_inicialization: ASSIGN expression SEMI_COLON | SEMI_COLON ; 

array_declaration: data_type variable array_declaration_size array_inicialization
array_declaration_size: OBRACKET CBRACKET array_declaration_size
					|   OBRACKET NUM_CONSTANT_INT CBRACKET array_declaration_size
					| 	OBRACKET NUM_CONSTANT_INT CBRACKET
					|	OBRACKET CBRACKET
array_list: NUM_CONSTANT_INT COMA array_list
			| NUM_CONSTANT_INT
array_inicialization: ASSIGN OBRACE array_list CBRACE SEMI_COLON | SEMI_COLON;

assignment_type: ASSIGN | SUM_ASSIGN | SUB_ASSIGN | MULT_ASSIGN | DIV_ASSIGN | MOD_ASSIGN
assigment: variable assignment_type expression | array_deref assignment_type expression

variable: VARIABLE_NAME 
array_deref: variable OBRACKET size CBRACKET

data_type: INT | FLOAT | DOUBLE | LONG | SHORT | CHAR | VOID MULT_OP;

return_statement: RETURN expression SEMI_COLON

if_else_statment: if_statement | if_statement else_statement 
if_statement: IF OPAR expression CPAR OBRACE code_block CBRACE
else_statement: ELSE OBRACE code_block CBRACE

while_statement: WHILE OPAR expression CPAR OBRACE code_block CBRACE

// el ; entre declaration y boolean no esta dado que declaration ya tiene uno
for_statement: FOR OPAR declaration expression SEMI_COLON assigment CPAR OBRACE code_block CBRACE
			|  FOR OPAR declaration expression SEMI_COLON expression CPAR OBRACE code_block CBRACE

switch_statement: SWITCH OPAR expression CPAR OBRACE code_block CBRACE 		// WARNING: disallow NUM_CONSTANT_FLOAT, string, SPECIAL_VARIABLE

size: variable | NUM_CONSTANT_INT 				;

expression:  expression ADD_OP expression 
			| expression SUB_OP expression 
			| expression MULT_OP expression 
			| expression DIV_OP expression 
			| expression MOD_OP expression 
			| expression INC_OP
			| INC_OP expression
			| expression DEC_OP
			| DEC_OP expression
			| BIT_NOT_OP expression
			| expression BIT_RIGHT_OP expression
			| expression BIT_LEFT_OP expression
			| expression BIT_XOR_OP expression
			| expression BIT_OR_OP expression
			| expression BIT_AND_OP expression

			| expression AND_OP expression
			| OPAR expression CPAR
			| expression OR_OP expression
			| NOT_OP OPAR expression CPAR

			| expression EQ_OP expression
			| expression GR_OP expression
			| expression GE_OP expression
			| expression LT_OP expression
			| expression LE_OP expression
			| expression NE_OP expression

			| variable 
			| NUM_CONSTANT_FLOAT 
			| NUM_CONSTANT_INT 
			| SPECIAL_VARIABLE 		// WARNING: disallow in non special_statement context
			| function_call 
			| array_deref
			| STRING


function_call: variable OPAR function_call_arg CPAR 
			| variable OPAR CPAR ;

function_call_arg: expression COMA function_call_arg
			 | expression


%%