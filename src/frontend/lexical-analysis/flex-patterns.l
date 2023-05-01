%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Para poder usar yy_pop_state() */
%option stack


%x SPECIAL_STATEMENT
%x ML_COMMENT
%x INCLUDE_STATE

digit	[0-9]
alpha	[a-zA-Z]
alnum	[a-zA-Z0-9]
alnumext [a-zA-Z0-9_]
alphaext [a-zA-Z_]
any 	[ -~]

whitespace [ \t\r\f\n]+
space [ \t]+

file_name [a-zA-Z0-9_-]+\.{alpha}{1,3}

variable_name {alphaext}+{alnumext}*
variable {variable_name}

num_constant_int {digit}+
num_constant_float {digit}*"."{digit}+

string 			\"{any}*\" 

%%

"/*"							{ BEGIN(ML_COMMENT); }
<ML_COMMENT>"*/"				{ BEGIN(INITIAL); }
<ML_COMMENT>{whitespace}		{ ; }
<ML_COMMENT>.					{ ; }

"//".*							{ ; }	/* single line comment */

"include"					{ BEGIN(INCLUDE_STATE); return INCLUDE; }
<INCLUDE_STATE>{space}		{ ; }
<INCLUDE_STATE>"<"			{ return LT_OP; }
<INCLUDE_STATE>{file_name}	{ return FILE_NAME; }
<INCLUDE_STATE>{string}		{ BEGIN(INITIAL); return STRING; }
<INCLUDE_STATE>">"			{ BEGIN(INITIAL); return GR_OP; }
<INCLUDE_STATE>.			{ ; }


"return"		{ return RETURN; }
"while"			{ return WHILE; }
"for"			{ return FOR; }
"if"			{ return IF; }
"else"			{ return ELSE; }
"continue"		{ return CONTINUE; }
"break"			{ return BREAK; }
"switch"		{ return SWITCH; }
"case"			{ return CASE; }
"default"		{ return DEFAULT; }

"{"				{ return OBRACE; }
"}"				{ return CBRACE; }
"("				{ return OPAR; }
")"				{ return CPAR; } 	
","				{ return COMA; }
";"				{ return SEMI_COLON; }
":"				{ return COLON; }
"#"				{ return NUMBER_SIGN; }

"="				{ return ASSIGN; }
"+="			{ return SUM_ASSIGN; }
"-="			{ return SUB_ASSIGN; }
"*="			{ return MULT_ASSIGN; }
"/="			{ return DIV_ASSIGN; }
"%="			{ return MOD_ASSIGN; }

"+"				{ return ADD_OP;}
"-"				{ return SUB_OP;}
"*"				{ return MULT_OP;}
"/"				{ return DIV_OP;}
"%"				{ return MOD_OP;}
"++"			{ return INC_OP;}
"--"			{ return DEC_OP;}

"~"				{ return BIT_NOT_OP;}
">>"			{ return BIT_RIGHT_OP;}
"<<"			{ return BIT_LEFT_OP;}
"^"				{ return BIT_XOR_OP;}
"|"				{ return BIT_OR_OP;}
"&"				{ return BIT_AND_OP;} 	

"!"             { return NOT_OP;}
"&&"            { return AND_OP;}
"||"            { return OR_OP;}
"=="            { return EQ_OP;}
">"             { return GR_OP;}
">="            { return GE_OP;}
"<"             { return LT_OP;}
"<="            { return LE_OP;}
"!="            { return NE_OP;}

"int"          		 	{  return INT; } 
"float"             	{  return FLOAT; }
"double"             	{  return DOUBLE; }
"long"               	{  return LONG; }
"short"              	{  return SHORT; }
"char"               	{  return CHAR; }
"void"				 	{  return VOID; }

{string}			 { return STRING; }
{variable}			 { return VARIABLE_NAME; }

{num_constant_float} { return NUM_CONSTANT_FLOAT; }	
{num_constant_int}   { return NUM_CONSTANT_INT; }
	

"<{"									{ BEGIN(SPECIAL_STATEMENT); return START_SPECIAL; } /* deberia poder devolver el "<{" */

<SPECIAL_STATEMENT>"REDUCE"				{ return REDUCE; }
<SPECIAL_STATEMENT>"MAP" 				{ return MAP; }
<SPECIAL_STATEMENT>"FILTER" 			{ return FILTER; }
<SPECIAL_STATEMENT>"FOREACH" 			{ return FOREACH; }
<SPECIAL_STATEMENT>"CREATE"             { return CREATE; }

<SPECIAL_STATEMENT>"int"                { return INT; }
<SPECIAL_STATEMENT>"float"              { return FLOAT; }
<SPECIAL_STATEMENT>"double"             { return DOUBLE; }
<SPECIAL_STATEMENT>"long"               { return LONG; }
<SPECIAL_STATEMENT>"short"              { return SHORT; }
<SPECIAL_STATEMENT>"char"               { return CHAR; }
<SPECIAL_STATEMENT>"..."                { return THREE_DOT; }

<SPECIAL_STATEMENT>"," 					{ return COMA; }
<SPECIAL_STATEMENT>{num_constant_float} { return NUM_CONSTANT_FLOAT; }				/*aca le tengo que pasar la el integer posta */
<SPECIAL_STATEMENT>{num_constant_int}   { return NUM_CONSTANT_INT; }
<SPECIAL_STATEMENT>{variable} 			{ return VARIABLE_NAME; }				/*aca le tengo que pasar la variable posta */
<SPECIAL_STATEMENT>{string}				{ return STRING; }

<SPECIAL_STATEMENT>"@elem"				{ return SPECIAL_VARIABLE; }

<SPECIAL_STATEMENT>"{" 					{ return LAMBDA_START; }
<SPECIAL_STATEMENT>"}"					{ return LAMBDA_END; }

<SPECIAL_STATEMENT>"+"					{ return ADD_OP;}
<SPECIAL_STATEMENT>"-"					{ return SUB_OP;}
<SPECIAL_STATEMENT>"*"					{ return MULT_OP;}
<SPECIAL_STATEMENT>"/"					{ return DIV_OP;}
<SPECIAL_STATEMENT>"%"					{ return MOD_OP;}

<SPECIAL_STATEMENT>"!"                  {return NOT_OP;}
<SPECIAL_STATEMENT>"("                  {return OPAR;}
<SPECIAL_STATEMENT>")"                  {return CPAR;}
<SPECIAL_STATEMENT>"&&"                 {return AND_OP;}
<SPECIAL_STATEMENT>"||"                 {return OR_OP;}

<SPECIAL_STATEMENT>"=="                 {return EQ_OP;}
<SPECIAL_STATEMENT>">"                  {return GR_OP;}
<SPECIAL_STATEMENT>">="                 {return GE_OP;}
<SPECIAL_STATEMENT>"<"                  {return LT_OP;}
<SPECIAL_STATEMENT>"<="                 {return LE_OP;}
<SPECIAL_STATEMENT>"!="                 {return NE_OP;}

<SPECIAL_STATEMENT>"++"					{ return INC_OP;}
<SPECIAL_STATEMENT>"--"					{ return DEC_OP;}

<SPECIAL_STATEMENT>"~"					{ return BIT_NOT_OP;}
<SPECIAL_STATEMENT>">>"					{ return BIT_RIGHT_OP;}
<SPECIAL_STATEMENT>"<<"					{ return BIT_LEFT_OP;}
<SPECIAL_STATEMENT>"^"					{ return BIT_XOR_OP;}
<SPECIAL_STATEMENT>"|"					{ return BIT_OR_OP;}
<SPECIAL_STATEMENT>"&"					{ return BIT_AND_OP;} 	

<SPECIAL_STATEMENT>{whitespace} 		{ ; }
<SPECIAL_STATEMENT>"}>"					{ BEGIN(INITIAL); return END_SPECIAL; }
<SPECIAL_STATEMENT>. 					{ return UnknownPatternAction(yytext, yyleng); } 	/* error */

{whitespace}	{ ; }	/* ignoro whitespace */

. 				{ return UnknownPatternAction(yytext, yyleng); } 	/* lenguaje de c que no tenemos en cuenta. ESTE DEBERIA SER ERROR ESTO */

%%