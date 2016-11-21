CC=clang
CFLAGS=-g
LDLIBS= -L/usr/local/ssl/lib -lssl -lcrypto
EXE=part3

all: main

main: part3.c
#	clear
	$(CC) part3.c -o $(EXE) $(CFLAGS) $(LDLIBS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm $(EXE)

