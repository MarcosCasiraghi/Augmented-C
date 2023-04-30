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

## Limitaciones

1) Solo acepta tipo de datos: short, char, int, long, float, double.
2) No acepta desreferenciar structs. Ejemplo: struct_name.struct_field  o  struct_name->struct_field
3) No permite puteros a funciones.
4) No permite casteos de tipo: (double) variable
5) No permite asignaciones de tipo: int a = 4, b = 2, c;

