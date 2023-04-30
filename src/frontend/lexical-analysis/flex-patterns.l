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
%x STRING
%x CHAR_STRING
%x DECLARATION
%x STRING_SPECIAL
%x CAST
%x ML_COMMENT
%x COMMENT
%x FUNC_ARG

digit	[0-9]
alpha	[a-zA-Z]
alnum	[a-zA-Z0-9]
alnumext [a-zA-Z0-9_]
alphaext [a-zA-Z_]


whitespace [ \t\r\f\n]+

variable_name {alphaext}+{alnumext}*
variable {variable_name}


num_constant_int {digit}+
num_constant {digit}+|{digit}*"."{digit}+
num_constant_float {digit}*"."{digit}+

%%

"\""						{ BEGIN(STRING); }
<STRING>"\""				{ BEGIN(INITIAL); }
<STRING>{whitespace}		{ ; }
<STRING>.					{ ; }

"\'"							{ BEGIN(CHAR_STRING); }
<CHAR_STRING>"\'"				{ BEGIN(INITIAL); }
<CHAR_STRING>{whitespace}		{ ; }
<CHAR_STRING>.					{ ; }

"/*"							{ BEGIN(ML_COMMENT); }
<ML_COMMENT>"*/"				{ BEGIN(INITIAL); }
<ML_COMMENT>{whitespace}		{ ; }
<ML_COMMENT>.					{ ; }

"//"						{ BEGIN(COMMENT); }
<COMMENT>"\n"				{ BEGIN(INITIAL); }
<COMMENT>{whitespace}		{ ; }
<COMMENT>.					{ ; }

"auto"			{ ; }
"break"			{ ; }
"case"			{ ; }
"const"			{ ; }
"continue"		{ ; }
"default"		{ ; }
"do"			{ ; }
"register"		{ ; }
"return"		{ ; }
"signed"		{ ; }
"sizeof"		{ ; }
"static"		{ ; }
"struct"		{ ; }
"switch"		{ ; }
"typedef"		{ ; }
"union"			{ ; }
"unsigned"		{ ; }
"void"			{ ; }
"volatile"		{ ; }
"while"			{ ; }
"else"			{ ; }
"enum"			{ ; }
"extern"		{ ; }
"for"			{ ; }
"goto"			{ ; }
"if"			{ ; }

"."				{ ; }
"->"			{ ; }
"["				{ ; }
"]"				{ ; }
"{"				{ ; }
"}"				{ ; }
","				{ ; }
";"				{ ; }
":"				{ ; }
"="				{ ; }
"#"				{ ; }


"+"				{ ; }
"-"				{ ; }
"*"				{ ; }
"/"				{ ; }
"%"				{ ; }
"!"				{ ; }
"=="			{ ; }
">"				{ ; }
">="			{ ; }
"<"				{ ; }
"<="			{ ; }
"!="			{ ; }
"++"			{ ; }
"--"			{ ; }
"~"				{ ; }
">>"			{ ; }
"<<"			{ ; }
"^"				{ ; }
"|"				{ ; }
"&"				{ ; } 	
"?"				{ ; } 	
"("				{ ; }
")"				{ ; } 	


"int"          		 { BEGIN(DECLARATION); return INT; } 
"float"              { BEGIN(DECLARATION); return FLOAT; }
"double"             { BEGIN(DECLARATION); return DOUBLE; }
"long"               { BEGIN(DECLARATION); return LONG; }
"short"              { BEGIN(DECLARATION); return SHORT; }
"char"               { BEGIN(DECLARATION); return CHAR; }

{variable}			 { ; }

{num_constant}		 { ; }
	
<DECLARATION>{variable} 			{ return VARIABLE_NAME; }				/* TODO: guardarse el nombre para backend */
<DECLARATION>"*"					{ return MULT_OP; }
<DECLARATION>","					{ return COMA; }
<DECLARATION>{whitespace} 			{ ; }

<DECLARATION>"("					{ BEGIN(FUNC_ARG); return OPAR; } 	/* caso argumentos en declaracion de funcion: int func(int a, int b) */
<DECLARATION>"["					{ BEGIN(INITIAL); return OPEN_BRACKET; }
<DECLARATION>";"					{ BEGIN(INITIAL); return SEMI_COLON; }
<DECLARATION>"="					{ BEGIN(INITIAL); return EQ_OP; } 		/* no nos importa lo que pasa despues ??? */
<DECLARATION>.						{ ; } 									/* por ahora ignoro */

<FUNC_ARG>"int"          	   { return INT; }
<FUNC_ARG>"float"              { return FLOAT; }
<FUNC_ARG>"double"             { return DOUBLE; }
<FUNC_ARG>"long"               { return LONG; }
<FUNC_ARG>"short"              { return SHORT; }
<FUNC_ARG>"char"               { return CHAR; }

<FUNC_ARG>{variable} 			{ printf("%s\n",yytext); return VARIABLE_NAME; }
<FUNC_ARG>"*"					{ return MULT_OP; }
<FUNC_ARG>","					{ return COMA; }
<FUNC_ARG>{whitespace} 			{ ; }

<FUNC_ARG>")"					{ BEGIN(INITIAL); return CPAR; }



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

<SPECIAL_STATEMENT>"\""					{ BEGIN(STRING_SPECIAL); return STRING_START;}

<SPECIAL_STATEMENT>{whitespace} 		{ ; }
<SPECIAL_STATEMENT>"}>"					{ BEGIN(INITIAL); return END_SPECIAL; }
<SPECIAL_STATEMENT>. 					{ return UnknownPatternAction(yytext, yyleng); } 	/* error */


<STRING_SPECIAL>"\""					{ BEGIN(SPECIAL_STATEMENT); return STRING_END; }	/* Tenemos un estado aparte porque NO queremos descartar su contenido */
<STRING_SPECIAL>{whitespace}			{ return STRING_CHARACTER; }
<STRING_SPECIAL>.						{ return STRING_CHARACTER; }


{whitespace}	{ ; }	/* ignoro whitespace */

. 				{ return UnknownPatternAction(yytext, yyleng); } 	/* lenguaje de c que no tenemos en cuenta. ESTE DEBERIA SER ERROR ESTO */

%%