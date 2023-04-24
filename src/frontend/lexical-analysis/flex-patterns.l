%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno


%x SPECIAL_STATEMENT



digit	[0-9]
alpha	[a-zA-Z]
alnum	[a-zA-Z0-9]
alnumext [a-zA-Z0-9_]
alphaext [a-zA-Z_]

whitespace [ \t\r\f\n]+

variable {alphaext}+{alnumext}*
num_constant {digit}+|{digit}*"."{digit}+


%%

"<{"									{ BEGIN(SPECIAL_STATEMENT); return START_SPECIAL; } /* deberia poder devolver el "<{" */

<SPECIAL_STATEMENT>"REDUCE"				{ return REDUCE; }
<SPECIAL_STATEMENT>"MAP" 				{ return MAP; }
<SPECIAL_STATEMENT>"FILTER" 			{ return FILTER; }
<SPECIAL_STATEMENT>"FOREACH" 			{ return FOREACH; }

<SPECIAL_STATEMENT>"," 					{ return COMA; }
<SPECIAL_STATEMENT>{num_constant} 		{ return NUM_CONSTANT; }				/*aca le tengo que pasar la el integer posta */
<SPECIAL_STATEMENT>{variable} 			{ return VARIABLE_NAME; }				/*aca le tengo que pasar la variable posta */

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



<SPECIAL_STATEMENT>{whitespace} 		{ ; }
<SPECIAL_STATEMENT>"}>"					{ BEGIN(INITIAL); return END_SPECIAL; }
<SPECIAL_STATEMENT>. 					{ return UnknownPatternAction(yytext, yyleng); } 	/* error */


{whitespace}	{ ; }	/* ignoro whitespace */

. 				{ return UnknownPatternAction(yytext, yyleng); }/* error */

%%