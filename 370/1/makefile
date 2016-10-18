CC=clang
CFLAGS=
EXE=blm

all: main

main: bloom.c
	clear
	$(CC) bloom.c -o $(EXE) $(CFLAGS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm -f $(EXE)
