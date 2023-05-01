%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
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
	int expression;
	int factor;
	int constant;

	// Terminales.
	token token;
	int integer;
}

//  = = = = = = = = = = = =  Definicion de simbolos y otros  = = = = = = = = = = = = 

%token REDUCE MAP FILTER FOREACH CREATE START_SPECIAL END_SPECIAL
%token EXPRESION_START EXPRESION_END 
%token LAMBDA_START LAMBDA_END
%token COMA
%token STRING_START STRING_CHARACTER STRING_END

%token VARIABLE_NAME NUM_CONSTANT_FLOAT NUM_CONSTANT_INT SPECIAL_VARIABLE

%token ADD_OP SUB_OP MULT_OP DIV_OP MOD_OP
%token INC_OP DEC_OP
%token BIT_NOT_OP BIT_RIGHT_OP BIT_LEFT_OP BIT_XOR_OP BIT_OR_OP BIT_AND_OP

%token NOT_OP OPAR CPAR AND_OP OR_OP
%token EQ_OP GR_OP GE_OP LT_OP LE_OP NE_OP

%token INT FLOAT DOUBLE LONG SHORT CHAR
%token THREE_DOT

%token SEMI_COLON CLOSE_BRACKET OPEN_BRACKET OBRACE CBRACE
%token NUMBER_SIGN INCLUDE RETURN IF WHILE FOR

%token ASSIGN SUM_ASSIGN SUB_ASSIGN MULT_ASSIGN DIV_ASSIGN MOD_ASSIGN // TODO: add bitwise

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

selector: REDUCE COMA reduce_statement | FILTER COMA filter_statement | FOREACH COMA foreach_statement 
			| MAP COMA map_statement | CREATE COMA create_statement ;


reduce_statement: variable COMA size COMA variable COMA lambda 	;
filter_statement: variable COMA size COMA variable COMA boolean_lambda 	;
map_statement: variable COMA size COMA variable COMA lambda 	;
foreach_statement: variable COMA size COMA LAMBDA_START function_call LAMBDA_END			;
create_statement: variable COMA data_type COMA create_lambda 	;

lambda: LAMBDA_START expression LAMBDA_END ;
boolean_lambda: LAMBDA_START boolean_expression LAMBDA_END ;
create_lambda : LAMBDA_START NUM_CONSTANT_INT THREE_DOT NUM_CONSTANT_INT LAMBDA_END ;


// = = = = = = = = = = = =  C Lang  = = = = = = = = = = = = = = = = 

statements: meta_command statements
		| function_declaration statements
		| declaration statements
		| meta_command
		| function_declaration
		| declaration

meta_command: NUMBER_SIGN INCLUDE string 			// TODO: defines 
			// | NUMBER_SIGN INCLUDE GR_OP FILE_NAME LT_OP



function_arg: data_type variable 
			| data_type pointers variable
function_args: function_arg | function_arg COMA function_args
function_declaration: data_type variable OPAR CPAR OBRACE code_block CBRACE
					| data_type variable OPAR function_args CPAR OBRACE code_block CBRACE

code_block: declaration code_block
		|	special_statement code_block
		| 	return_statement code_block
		|	if_statement code_block
		|	for_statement code_block
		|	while_statement code_block

		|	declaration
		|	special_statement
		|	return_statement
		|	if_statement
		|	for_statement
		|	while_statement


pointers: MULT_OP | MULT_OP pointers ;

declartion_end: ASSIGN expression SEMI_COLON | SEMI_COLON ; 			// solo permite asignacion de tipo a = 3.
declaration: data_type variable declartion_end 
			| data_type pointers variable declartion_end

return_statement: RETURN expression SEMI_COLON

if_statement: IF OPAR boolean_expression CPAR OBRACE code_block CBRACE   // TODO: add else

while_statement: WHILE OPAR boolean_expression CPAR OBRACE code_block CBRACE

// el ; entre declaration y boolean no esta dado que declaration ya tiene uno
for_statement: FOR OPAR declaration boolean_expression SEMI_COLON assigment CPAR OBRACE code_block CBRACE
			|  FOR OPAR declaration boolean_expression SEMI_COLON expression CPAR OBRACE code_block CBRACE

assignment_type: ASSIGN | SUM_ASSIGN | SUB_ASSIGN | MULT_ASSIGN | DIV_ASSIGN | MOD_ASSIGN

assigment: variable assignment_type expression



variable: VARIABLE_NAME  					;	

data_type: INT | FLOAT | DOUBLE | LONG | SHORT | CHAR ;

size: variable | NUM_CONSTANT_INT 				;

string: STRING_START string_character STRING_END 						;
string_character: string_character STRING_CHARACTER | STRING_CHARACTER 	;

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
			| variable 
			| NUM_CONSTANT_FLOAT 
			| NUM_CONSTANT_INT 
			| SPECIAL_VARIABLE 		// check in backend if you are in c_lang, if so, reject
			| function_call 
			| string

boolean_expression: boolean_expression AND_OP boolean_expression
					| OPAR boolean_expression CPAR
					| boolean_expression OR_OP boolean_expression
					| NOT_OP OPAR boolean_expression CPAR
					| relational_expression ;

relational_expression: expression EQ_OP expression
						| expression GR_OP expression
						| expression GE_OP expression
						| expression LT_OP expression
						| expression LE_OP expression
						| expression NE_OP expression ;

function_call: variable OPAR function_call_arg CPAR 
			| variable OPAR CPAR ;

function_call_arg: expression COMA function_call_arg
			 | expression


%%
