CC=clang
CFLAGS=-g
LDLIBS= -L/usr/local/ssl/lib -lssl -lcrypto
EXE=prog1

all: main

main: prog1.c
#	clear
	$(CC) prog1.c -o $(EXE) $(CFLAGS) $(LDLIBS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm $(EXE)

