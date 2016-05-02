#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<omp.h>

#define NUMT 4

struct a
{
	float value;
};

int main(int argc, char *argv[])
{
	omp_set_num_threads(NUMT);
	struct a A[4];

	unsigned int someBigNumber = 1000000000;	// if > 4B, use "long unsigned int"
	double time0 = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++) {
		float sum = A[i].value;
		for(unsigned int j = 0; j < someBigNumber; j++)	{
			sum = sum + 2.;
		}
	}
	double time1 = omp_get_wtime( );
	double megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
	printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 1, (time1-time0));

	return 0;
}
