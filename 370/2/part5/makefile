CC=clang
CFLAGS=-g
LDLIBS= -L/usr/local/ssl/lib -lssl -lcrypto
EXE=part5

all: main

main: part5.c
	clear
	$(CC) part5.c -o $(EXE) $(CFLAGS) $(LDLIBS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm $(EXE)

