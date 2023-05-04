# Augmented-C
 A compiler to augment C programming language.

## Requerimientos

Para construir el compilador, se requieren las siguientes dependencias:

* [Bison v3.8.2](https://www.gnu.org/software/bison/)
* [CMake v3.24.1](https://cmake.org/)
* [Flex v2.6.4](https://github.com/westes/flex)
* [GCC v11.1.0](https://gcc.gnu.org/)
* [Make v4.3](https://www.gnu.org/software/make/)


## Construcción

```bash
user@machine:path/ $ chmod u+x --recursive script
user@machine:path/ $ script/build.sh
```

## Ejecución

```bash
user@machine:path/ $ script/start.sh program_file
```

## Testing

En Linux:

```bash
user@machine:path/ $ script/test.sh
```

## Casos válidos

1) El compilador acepta declaracion de funciones (como main()) y llamados.
2) El compilador acepta declaraciones de variables y asignaciones (incluye arreglos).
3) Se aceptan bloques if-else (necesariamente tiene que tener {}).
4) Se aceptan ciclos for/while.
5) Se aceptan bloques switch.
6) Se acpetan #include's.

## Special Statement
Nuestra aumento de C se particuliza con poder reconocer una lineas para ahorrar trabajo al programador.

Estas tienen el siguiente formato:
<{CREATE, nombre_array, data_type, { number...number } }>   
## para crear un array ya inicializado con valores secuenciales.

<{FILTER, nombre_array1, size, nombre_array2, { "expresion booleana a cumplir"  } }>
## para filtrar a otro arreglo a partir de una condicion. Adentro de la expresion con @elem se refiere al elemento del arreglo1 en cada iteracion. Size puede ser pasado como una variable.

<{MAP, nombre_array1, size, nombre_array2, { "expresion a ejecutar" } }>
## para mapear los elementos de un arreglo a otro con cierta transformacion. Se usa @elem nuevamente.

<{REDUCE, nombre_array, size, variable, { "expresion que se le asigna a variable" } }>
## para reducir los elementos de un arreglo a una variable. Para mantener el valor de la variable, deberia ser utilizada en cada iteracion.

<{FOREACH, nombre_array, size, { "llamado a funcion" } }>
## para aplicar cierta funcion a cada elemento del arreglo. Tiene que pasarse una funcion.

Cada una de estas ultimas 4 funciones se pueden ejecutar para un cierto rango.
Se invocan con: NOMBRERANGE y en vez de recibir un size, se recibe el indice de inicio y el indice de fin.


## Limitaciones

1) Solo acepta tipo de datos: short, char, int, long, float, double, void*.
2) No acepta desreferenciar structs. Ejemplo: struct_name.struct_field  o  struct_name->struct_field
3) No acepta creacion de structs.
4) No acepta definicion de nuevos tipos de datos (typedef).
5) No se permite casteos explicitos.
6) No permite puteros a funciones.
7) No permite asignaciones de tipo: int a = 4, b = 2, c, ni creacion de multiples variables en una linea
8) No acepta asignacion con operadores de bitshift. Ejemplo: var &= 2  o var >>= 2
9) No se permite creacion de arreglos del estilo: char * argv[]. Se debe hacer de esta forma: char ** argv.

