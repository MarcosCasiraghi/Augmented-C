#include "../../backend/support/logger.h"
#include "flex-actions.h"


/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

token StringPatternAction(const char * lexeme, const int length) {
	char * string = (char *) calloc(length + 1, sizeof(char));
	strncpy(string, lexeme, length);
	yylval.STRING = string;
	return STRING;
}

token FileNamePatternAction(const char * lexeme, const int length) {
	char * string = (char *) calloc(length + 1, sizeof(char));
	strncpy(string, lexeme, length);
	yylval.STRING = string;
	return FILE_NAME;
}

token VariableNamePatternAction(const char * lexeme, const int length) {
	char * string = (char *) calloc(length + 1, sizeof(char));
	strncpy(string, lexeme, length);
	yylval.STRING = string;
	return VARIABLE_NAME;
}
token NumConstantFloatPatternAction(const char * lexeme, const int length) {
	char * string = (char *) calloc(length + 1, sizeof(char));
	strncpy(string, lexeme, length);
	yylval.STRING = string;
	return NUM_CONSTANT_FLOAT;
}
token NumConstantIntPatternAction(const char * Lexeme, const int length) {
	char * string = (char *) calloc(length + 1, sizeof(char));
	strncpy(string, Lexeme, length);
	yylval.num_int = atoi(string);
	return NUM_CONSTANT_INT;
}




token UnknownPatternAction(const char * lexeme, const int length) {
	LogDebug("UnknownPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.token = YYUNDEF;
	// Al emitir este token, el compilador aborta la ejecución.
	return YYUNDEF;
}

// void IgnoredPatternAction(const char * lexeme, const int length) {
// 	LogDebug("IgnoredPatternAction: '%s' (length = %d).", lexeme, length);
// 	// Como no debe hacer nada con el patrón, solo se loguea en consola.
// }
