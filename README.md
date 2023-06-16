# Augmented-C
 Un compilador para aumentar el lenguaje de programación de C.

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

## Troubleshooting

Existe la posibilidad de que obtenga un error del estilo: "/bin/bash^M: bad interpreter: No such file or directory".
Si es el caso, ejecute:

```bash
user@machine:path/ $ sed -i -e 's/\r$//' ./script/*.sh 
```

## Ejecución

```bash
user@machine:path/ $ script/start.sh program_file
```

## Testing

```bash
user@machine:path/ $ script/test.sh
```

Para testear la gramatica de C:

```bash
user@machine:path/ $ script/test-clang.sh
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
para crear un array ya inicializado con valores secuenciales.
Ambos numeros tienen que ser positivos y distintos.

<{FILTER, nombre_arreglo1, tamaño, nombre_arreglo2, { "expresión booleana a cumplir"  } }>
para filtrar a arreglo2 a partir de una condicion. Adentro de la expresion con @elem se referencia al elemento del arreglo1 en cada iteración. "tamaño" puede ser pasado como una variable.

<{MAP, nombre_arreglo1, tamaño, nombre_arreglo2, { "expresión a ejecutar" } }>
para mapear los elementos de un arreglo a otro con cierta transformacion. Se puede referenciar a @elem.

<{REDUCE, nombre_arreglo, tamaño, variable, { "expresión que se le asigna a variable" } }>
para reducir los elementos de un arreglo a una variable. Para mantener el valor de la variable en cada iteración, debe ser utilizada en la expresión. Se puede referenciar a @elem.

<{FOREACH, nombre_array, size, { "llamado a funcion" } }>
para aplicar cierta funcion a cada elemento del arreglo. Tiene que pasarse una funcion. Se puede referenciar a @elem.

Cada una de estas ultimas 4 funciones se pueden ejecutar para un cierto rango.
Se invocan con: NOMBRERANGE y en vez de recibir un tamaño, se recibe el indice de inicio y el indice de fin.


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

