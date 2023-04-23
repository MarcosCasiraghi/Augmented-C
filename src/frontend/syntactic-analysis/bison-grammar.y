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

%token VARIABLE_NAME NUM_CONSTANT

%token ADD_OP SUB_OP MULT_OP DIV_OP MOD_OP


// Reglas de asociatividad y precedencia
%left ADD_OP SUB_OP
%left MULT_OP DIV_OP MOD_OP


// gramatica

%start program

%%

program: special_statement { ProgramGrammarAction(); };
special_statement : START_SPECIAL selector END_SPECIAL 	;

selector: REDUCE COMA reduce_statement | FILTER COMA filter_statement | FOREACH COMA foreach_statement | MAP COMA map_statement 	;

reduce_statement: variable COMA size COMA variable COMA lambda 	;
filter_statement: variable COMA size COMA variable COMA lambda 	;
map_statement: variable COMA size COMA variable COMA lambda 	;
foreach_statement: variable COMA size COMA lambda 				;

lambda: LAMBDA_START expression LAMBDA_END ;

size: variable | NUM_CONSTANT ;
variable: VARIABLE_NAME   ;

expression:  expression ADD_OP expression | expression SUB_OP expression | expression MULT_OP expression 
			| expression DIV_OP expression | expression MOD_OP expression | variable | NUM_CONSTANT ;

%%
