#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include "frontend/syntactic-analysis/bison-actions.h"
#include "backend/semantic-analysis/symbol-list.h"
#include "backend/semantic-analysis/error-list.h"
#include <stdio.h>
#include <stdlib.h>

// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.result = 0;
	state.succeed = true;

	//tabla de simbolos
	symbol_list * list = malloc(sizeof(symbol_list));
    if( list == NULL ){
        LogError("No se pudo reservar memoria para la aplicacion");
		return -1;
    }
    state.list = list;
    init_list(list);

	//lista de errores
	error_list * errors_list = malloc(sizeof(error_list));
	if( errors_list == NULL){
		LogError("No se pudo reservar memoria para la aplicacion");
		return -1;
	}
	state.errors_list = errors_list;
	init_error_list(errors_list);

	//pila de scopes
	StackList * stack = malloc(sizeof(StackList));
	if( stack == NULL ){
		LogError("No se pudo reservar memoria para la aplicacion");
		return -1;
	}
	state.stack = stack;
	init_stack(stack);

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}

	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
	const int result = yyparse();
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("La compilacion fue exitosa.");

				//Abro un archivo para escribir
				FILE * fileFD = fopen("code.c", "w+");
				if( fileFD == NULL ){
					printf("error al abrir archivo\n");
					exit(1);
				}
				state.fd = fileFD;

				GenProgramNode(state.program);

				//es necesario cerrar el archivo para que despues pueda ser modificado por el linter
				fclose(state.fd);
				LogInfo("En el archivo \"code.c\" está el código final");
			}
			else {
				LogError("Se produjo un error en la aplicacion.");
				printErrors(state.errors_list);
				return -1;
			}
			freeProgramNode(state.program);
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).", result);
	}
	freeSymbolList(state.list);
	freeErrorList(state.errors_list);
	freeScopeStack(state.stack);
	LogInfo("Fin.");
	return result;
}
