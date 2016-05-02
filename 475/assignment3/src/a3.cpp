#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<omp.h>

#define NUMT 4

struct a
{
	float value;
	int pad[1];
};
struct b
{
	float value;
	int pad[2];
};
struct c
{
	float value;
	int pad[3];
};
struct d
{
	float value;
	int pad[4];
};
struct e
{
	float value;
	int pad[5];
};
struct f
{
	float value;
	int pad[6];
};
struct g
{
	float value;
	int pad[7];
};
struct h
{
	float value;
	int pad[8];
};
struct i
{
	float value;
	int pad[9];
};
struct j
{
	float value;
	int pad[10];
};
struct k
{
	float value;
	int pad[11];
};
struct l
{
	float value;
	int pad[12];
};
struct m
{
	float value;
	int pad[13];
};
struct n
{
	float value;
	int pad[14];
};
struct o
{
	float value;
	int pad[15];
};
struct p
{
	float value;
	int pad[16];
};

int main(int argc, char *argv[])
{
	omp_set_num_threads(NUMT);
	struct a A[4];
	struct b B[4];
	struct c C[4];
	struct d D[4];
	struct e E[4];
	struct f F[4];
	struct g G[4];
	struct h H[4];
	struct i I[4];
	struct j J[4];
	struct k K[4];
	struct l L[4];
	struct m M[4];
	struct n N[4];
	struct o O[4];
	struct p P[4];

	unsigned int someBigNumber = 1000000000;	// if > 4B, use "long unsigned int"
	double time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			A[ i ].value = A[ i ].value + 2.;
		}
	}
	double time1 = omp_get_wtime( );
	double megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 1, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			B[ i ].value = B[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 2, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			C[ i ].value = C[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 3, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			D[ i ].value = D[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 4, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			E[ i ].value = E[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 5, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			F[ i ].value = F[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 6, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			G[ i ].value = G[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 7, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			H[ i ].value = H[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 8, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			I[ i ].value = I[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 9, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			J[ i ].value = J[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 10, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			K[ i ].value = K[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 11, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			L[ i ].value = L[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 12, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			M[ i ].value = M[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 13, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			N[ i ].value = N[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 14, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			O[ i ].value = O[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 15, (time1-time0));

	time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			P[ i ].value = P[ i ].value + 2.;
		}
	}
	time1 = omp_get_wtime( );
	megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 16, (time1-time0));


	return 0;
}
