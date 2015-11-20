CC=gcc
CFLAGS=
EXE=smallsh

all: main

main: smallsh.c
	clear
	$(CC) smallsh.c -o $(EXE) $(CFLAGS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm -f $(EXE)

