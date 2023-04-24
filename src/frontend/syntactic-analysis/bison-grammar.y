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

// definicion de simbolos y otros

%token REDUCE MAP FILTER FOREACH START_SPECIAL END_SPECIAL
%token EXPRESION_START EXPRESION_END 
%token LAMBDA_START LAMBDA_END
%token COMA

%token VARIABLE_NAME NUM_CONSTANT SPECIAL_VARIABLE

%token ADD_OP SUB_OP MULT_OP DIV_OP MOD_OP
%token INC_OP DEC_OP
%token BIT_NOT_OP BIT_RIGHT_OP BIT_LEFT_OP BIT_XOR_OP BIT_OR_OP BIT_AND_OP

%token NOT_OP OPAR CPAR AND_OP OR_OP
%token EQ_OP GR_OP GE_OP LT_OP LE_OP NE_OP


// Reglas de asociatividad y precedencia
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

// gramatica

%start program

%%

program: special_statement { ProgramGrammarAction(); };
special_statement : START_SPECIAL selector END_SPECIAL 	;

selector: REDUCE COMA reduce_statement | FILTER COMA filter_statement | FOREACH COMA foreach_statement | MAP COMA map_statement 	;

reduce_statement: variable COMA size COMA variable COMA lambda 	;
filter_statement: variable COMA size COMA variable COMA boolean_lambda 	;
map_statement: variable COMA size COMA variable COMA lambda 	;
foreach_statement: variable COMA size COMA lambda 				;

lambda: LAMBDA_START expression_with_lambda LAMBDA_END ;
boolean_lambda: LAMBDA_START boolean_expression_with_lambda LAMBDA_END ;

size: variable | NUM_CONSTANT ;
variable: VARIABLE_NAME   ;


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
			| NUM_CONSTANT ;

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



// lambda expression must also accept @elem, but c expression must not accept it => we need another expression

expression_with_lambda:  expression_with_lambda ADD_OP expression_with_lambda 
						| expression_with_lambda SUB_OP expression_with_lambda 
						| expression_with_lambda MULT_OP expression_with_lambda 
						| expression_with_lambda DIV_OP expression_with_lambda 
						| expression_with_lambda MOD_OP expression_with_lambda 
						| expression_with_lambda INC_OP
						| INC_OP expression_with_lambda
						| expression_with_lambda DEC_OP
						| DEC_OP expression_with_lambda
						| BIT_NOT_OP expression_with_lambda
						| expression_with_lambda BIT_RIGHT_OP expression_with_lambda
						| expression_with_lambda BIT_LEFT_OP expression_with_lambda
						| expression_with_lambda BIT_XOR_OP expression_with_lambda
						| expression_with_lambda BIT_OR_OP expression_with_lambda
						| variable 
						| NUM_CONSTANT 
						| SPECIAL_VARIABLE ;

relational_expression_with_lambda: expression_with_lambda EQ_OP expression_with_lambda
						| expression_with_lambda GR_OP expression_with_lambda
						| expression_with_lambda GE_OP expression_with_lambda
						| expression_with_lambda LT_OP expression_with_lambda
						| expression_with_lambda LE_OP expression_with_lambda
						| expression_with_lambda NE_OP expression_with_lambda ;

boolean_expression_with_lambda: boolean_expression_with_lambda AND_OP boolean_expression_with_lambda
					| OPAR boolean_expression_with_lambda CPAR
					| boolean_expression_with_lambda OR_OP boolean_expression_with_lambda
					| NOT_OP OPAR boolean_expression_with_lambda CPAR
					| relational_expression_with_lambda ;

%%
