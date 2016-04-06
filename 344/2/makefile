CC=gcc
CFLAGS=
EXE=adventure

all: main

main: malickc.adventure.c
	clear
	$(CC) malickc.adventure.c -o $(EXE) $(CFLAGS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm -f $(EXE)
	rm -rf malickc.rooms.*
