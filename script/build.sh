#! /bin/bash

ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT"
pwd

echo "Removing previous verion..."
rm ../src/frontend/lexical-analysis/flex-scanner.c ../src/frontend/syntactic-analysis/bison-parser.c ../src/frontend/syntactic-analysis/bison-parser.h
rm -r ../bin

echo "Building the compiler..."
cmake -S .. -B ../bin
cd ../bin
make

echo "All done."
