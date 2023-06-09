#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"

void Generator(int result);

void GenMetaCommandNode(MetaCommandNode * node);

void GenSizeNode(SizeNode * node);

void GenArrayDerefNode(ArrayDerefNode * node);

void GenPointerNode(PointerNode * node);

void GenSingleInitializeNode(SingleInitializeNode * node);

void GenSingleDeclarationNode(SingleDeclarationNode * node);

void GenDataType(DataType dataType);

void GenArrayInitializeNode(ArrayInitializeNode * node);

void GenArrayListNode(ArrayListNode * node);

void GenArraySizeNode(ArraySizeNode * node);

void GenArrayDeclarationNode(ArrayDeclarationNode * node)

#endif
