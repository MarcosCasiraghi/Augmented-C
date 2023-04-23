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

whitespace [ \t\r\f\n]+

integer {digit}+
variable {alpha}+{alnumext}*


%%

"<{"							{ BEGIN(SPECIAL_STATEMENT); return START_SPECIAL; } /* deberia poder devolver el "<{" */

<SPECIAL_STATEMENT>"REDUCE"		{ return REDUCE; }
<SPECIAL_STATEMENT>"MAP" 		{ return MAP; }
<SPECIAL_STATEMENT>"FILTER" 	{ return FILTER; }
<SPECIAL_STATEMENT>"FOREACH" 	{ return FOREACH; }
<SPECIAL_STATEMENT>"," 			{ return COMA; }
<SPECIAL_STATEMENT>{variable} 	{ return VARIABLE_NAME; }	/*aca le tengo que pasar la variable posta */
<SPECIAL_STATEMENT>{integer} 	{ return SIZE; }			/*aca le tengo que pasar la el integer posta */
<SPECIAL_STATEMENT>"{" 			{ return EXPRESION_START; }
<SPECIAL_STATEMENT>"}" 			{ return EXPRESION_END; }
<SPECIAL_STATEMENT>{whitespace} { ; }

<SPECIAL_STATEMENT>"}>"			{ BEGIN(INITIAL); return END_SPECIAL; }

{whitespace}	{ ; }/* ignoro whitespace */

. 				{ return UnknownPatternAction(yytext, yyleng); }/* error */

%%