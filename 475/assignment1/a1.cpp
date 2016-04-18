#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<omp.h>

#define XMIN 0.
#define XMAX 3.
#define YMIN 0.
#define YMAX 3.

#define Z00 0.
#define Z10 1.
#define Z20 0.
#define Z30 0.

#define Z01 1.
#define Z11 6.
#define Z21 1.
#define Z31 0.

#define Z02 0.
#define Z12 1.
#define Z22 0.
#define Z32 4.

#define Z03 3.
#define Z13 2.
#define Z23 3.
#define Z33 3.


float Height( int, int, int );

int main( int argc, char *argv[ ] )
{

    for(int NUMT = 1; NUMT <= 4; NUMT++) {
        for(int NUMS = 5; NUMS < 50000; NUMS*=10) {
            float volume = 0.0;
            // the area of a single full-sized tile:

            float fta = (  ( (XMAX-XMIN)/(float)(NUMS-1) )  * 
                ( ( YMAX - YMIN )/(float)(NUMS-1) )  );
            omp_set_num_threads(NUMT);


            double time0 = omp_get_wtime();
 
            // sum up the weighted heights into the variable "volume"
            // using an OpenMP for loop and a reduction:
            #pragma omp parallel for reduction(+:volume)
            for( long i = 0; i <NUMS*NUMS; i++) {
                int iu = i % NUMS;
                int iv = i / NUMS;

                //edge, corner variable, 1 == not edge or corner
                int multiplier = 1;

                //if iu == bottom or top edge
                if(iu == 0 || iu == (NUMS - 1))
                    multiplier *= .5;

                if(iv == 0 || iv == (NUMS - 1))
                    multiplier *= .5;
                volume += multiplier*fta*Height(iu, iv, NUMS);
            }
            // Implied barrier
            
            double time1 = omp_get_wtime();
            double mmults = (double)(NUMS*NUMS)/(time1-time0)/1000000.0;
            printf("|volume: %10f|Num Threads: %2i|Num Sections: %6i|MMults: %5f\n", volume, NUMT, NUMS, mmults);
        }

            printf("----------------------------------------------------------\n");
    }
          
    return 0;
}


float Height( int iu, int iv, int NUMS )    // iu,iv = 0 .. NUMS-1
{
    float u = (float)iu / (float)(NUMS-1);
    float v = (float)iv / (float)(NUMS-1);

    // the basis functions:

    float bu0 = (1.-u) * (1.-u) * (1.-u);
    float bu1 = 3. * u * (1.-u) * (1.-u);
    float bu2 = 3. * u * u * (1.-u);
    float bu3 = u * u * u;

    float bv0 = (1.-v) * (1.-v) * (1.-v);
    float bv1 = 3. * v * (1.-v) * (1.-v);
    float bv2 = 3. * v * v * (1.-v);
    float bv3 = v * v * v;

    // finally, we get to compute something:

    float height =    bu0 * ( bv0*Z00 + bv1*Z01 + bv2*Z02 + bv3*Z03 )
            + bu1 * ( bv0*Z10 + bv1*Z11 + bv2*Z12 + bv3*Z13 )
            + bu2 * ( bv0*Z20 + bv1*Z21 + bv2*Z22 + bv3*Z23 )
            + bu3 * ( bv0*Z30 + bv1*Z31 + bv2*Z32 + bv3*Z33 );

    return height;
}
