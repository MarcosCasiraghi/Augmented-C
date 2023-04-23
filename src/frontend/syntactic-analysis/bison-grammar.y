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
%token COMA
%token VARIABLE_NAME SIZE

// gramatica

%start program

%%

program: special_statement { ProgramGrammarAction(); };
special_statement : START_SPECIAL selector END_SPECIAL

selector: REDUCE COMA reduce_statement | FILTER COMA filter_statement | FOREACH COMA foreach_statement | MAP COMA map_statement

reduce_statement: variable COMA size COMA variable COMA EXPRESION_START expression EXPRESION_END 
filter_statement: variable COMA size COMA variable COMA EXPRESION_START expression EXPRESION_END 
map_statement: variable COMA size COMA variable COMA EXPRESION_START expression EXPRESION_END 
foreach_statement: variable COMA size COMA EXPRESION_START expression EXPRESION_END

variable: VARIABLE_NAME
size: SIZE

expression: SIZE		// change

%%
