#!/bin/bash

# Find all .cpp and .c files
clang++ -std=c++2b -c $(find -L ./src -type f -iregex ".*\.cpp") -g $(find -L ./src -type f -iregex ".*\.c") || { echo "Compiling object failed"; exit 1; }

# linking libraries
clang++ -std=c++2b -pthread *.o -o main.exec -lncurses || { echo "Compiling exec failed"; rm -r *.o ; exit 1; } 

# clean up *.o
rm -r *.o 