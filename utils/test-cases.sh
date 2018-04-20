#!/bin/bash

colour-valgrind --leak-check=full --show-leak-kinds=all ./bin/Debug/ex17 omg.dat c db.dat
read;
colour-valgrind --leak-check=full --show-leak-kinds=all ./bin/Debug/ex17 omg.dat s db.dat 1 ceco ceco@ceco.com
read;
colour-valgrind --leak-check=full --show-leak-kinds=all ./bin/Debug/ex17 omg.dat g db.dat 1
read;
colour-valgrind --leak-check=full --show-leak-kinds=all ./bin/Debug/ex17 omg.dat l
read;
colour-valgrind --leak-check=full --show-leak-kinds=all ./bin/Debug/ex17 omg.dat d db.dat 1
read;
#valgrind --leak-check=full --show-leak-kinds=all ./bin/Debug/ex17 omg.dat l
