CC=clang++
CFLAGS=
EXE=bloom_filter

all: main

main: bloom.cpp
	clear
	$(CC) bloom.cpp -o $(EXE) $(CFLAGS)

run: main
	./$(EXE)

debug: main
	gdb -q -ex run ./$(EXE)

clean:
	rm -f $(EXE)
