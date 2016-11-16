CC=clang
CFLAGS=-lssl -lcrypto
EXE=prog1

all: main

main: prog1.c
	clear
	$(CC) prog1.c -o $(EXE) $(CFLAGS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm -f $(EXE)

