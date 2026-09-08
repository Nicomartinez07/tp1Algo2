VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc


all: clean valgrind_alumno

valgrind_alumno: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

pruebas_alumno: src/*.c pruebas/*.c
	$(CC) $(CFLAGS) src/*.c pruebas/*.c -o pruebas_alumno

clean:
	rm -f pruebas_alumno