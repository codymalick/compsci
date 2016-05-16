simd.p5.o: simd.p5.h simd.p5.cpp
	g++ -c simd.p5.cpp -o simd.p5.o

a5.o: a5.o simd.p5.o
	g++ -o a5.o a5.cpp simd.p5.o -lm -fopenmp
