#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>

#include <omp.h>

#ifndef SIMD_H
#define SIMD_H

// SSE stands for Streaming SIMD Extensions
#define SSE_WIDTH	4

#define ALIGNED		__attribute__((aligned(16)))

#endif //simd_h

#define ONEM 1000000

void SimdMul(float *, float*, float*, int);
float SimdMulSum(float *, float *, int);

void man_mult(float *, float *, float *, int);
void man_sum(float *, float *, int);
int main(int argc, char *argv[])
{
	int iterations = 100;
	float *array1, *array2, *array3;
	double time0, time1, simd_time, manual_time, simd_both, man_both;
	double average_simd = 0, top_simd = 100, average_man = 0, top_man = 100;
	double mults, reducs, max_mults = 0, max_reducs = 0;
	printf("Size,SIMD Mult, Manual Mult, SIMD Mult/Reduction, Manual Mult/Reduction\n");

	for(int j = 0; j < 10; j++) {
		for(int i = 1000; i <= 3200000; i += 1000) {
			array1 = new float[i];
			array2 = new float[i];
			array3 = new float[i];

			time0 = omp_get_wtime();
				
			// Call simd
			SimdMul(array1, array2, array3, i);
			time1 = omp_get_wtime();
			simd_time = time1 - time0;

			time0 = omp_get_wtime();

			// Call manual mult
			man_mult(array1, array2, array3, i);
			time1 = omp_get_wtime();
			manual_time = time1 - time0;

			// simd both
			time0 = omp_get_wtime();
			SimdMulSum(array1, array2, i);
			time1 = omp_get_wtime();
			simd_both = time1 - time0;

			// man both
			time0 = omp_get_wtime();
			man_sum(array1, array2, i);
			time1 = omp_get_wtime();
			man_both = time1 - time0;

			mults = manual_time/simd_time;
			if(mults > max_mults)
				max_mults = mults;
			
			reducs = man_both/simd_both;
			if(reducs > max_reducs)
				max_reducs = reducs;

			printf("%i,%f,%f\n",i,mults,reducs);
//			printf("%i,%f,%f,%f,%f,\n",i,i/simd_time/ONEM,
//					i/manual_time/ONEM,
//					i/simd_both/ONEM,
//					i/man_both/ONEM);
			// Free arrays to prevent memory overload
			delete array1;
			delete array2;
			delete array3;
		}
	}
	// average_values
	printf("Max Mult Speedup: %f, Max Reduc Speedup: %f\n", max_mults, max_reducs);

	

	return 0;
}

void man_mult(float *a, float *b, float *c, int len) {
	for(int i = 0; i < len; i++) {
		c[i] = a[i] * b[i];
	}
}

void man_sum(float *a, float *b, int len) {
	float sum = 0;
	for(int i = 0; i < len; i++) {
		sum += a[i] * b[i];
	}
}

void SimdMul( float *a, float *b,   float *c,   int len )
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	__asm
		(
		 ".att_syntax\n\t"
		 "movq    -24(%rbp), %rbx\n\t"		// a
		 "movq    -32(%rbp), %rcx\n\t"		// b
		 "movq    -40(%rbp), %rdx\n\t"		// c
		);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
			(
			 ".att_syntax\n\t"
			 "movups	(%rbx), %xmm0\n\t"	// load the first sse register
			 "movups	(%rcx), %xmm1\n\t"	// load the second sse register
			 "mulps	%xmm1, %xmm0\n\t"	// do the multiply
			 "movups	%xmm0, (%rdx)\n\t"	// store the result
			 "addq $16, %rbx\n\t"
			 "addq $16, %rcx\n\t"
			 "addq $16, %rdx\n\t"
			);
	}

	for( int i = limit; i < len; i++ )
	{
		c[i] = a[i] * b[i];
	}
}



float SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;

	__asm
		(
		 ".att_syntax\n\t"
		 "movq    -40(%rbp), %rbx\n\t"		// a
		 "movq    -48(%rbp), %rcx\n\t"		// b
		 "leaq    -32(%rbp), %rdx\n\t"		// &sum[0]
		 "movups	 (%rdx), %xmm2\n\t"		// 4 copies of 0. in xmm2
		);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
			(
			 ".att_syntax\n\t"
			 "movups	(%rbx), %xmm0\n\t"	// load the first sse register
			 "movups	(%rcx), %xmm1\n\t"	// load the second sse register
			 "mulps	%xmm1, %xmm0\n\t"	// do the multiply
			 "addps	%xmm0, %xmm2\n\t"	// do the add
			 "addq $16, %rbx\n\t"
			 "addq $16, %rcx\n\t"
			);
	}

	__asm
		(
		 ".att_syntax\n\t"
		 "movups	 %xmm2, (%rdx)\n\t"	// copy the sums back to sum[ ]
		);

	for( int i = limit; i < len; i++ )
	{
		sum[i-limit] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}
