#!/bin/bash

VALGRIND_CALL="colour-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/Debug/ex17 db.dat "
PARAMETERS=(
	"c 100 512" "s 1 ceco ceco@ceco.com" "g 1" "l" "d 1"
)

for i in "${PARAMETERS[@]}"; do
	eval "${VALGRIND_CALL}${i}";
	read;
done
