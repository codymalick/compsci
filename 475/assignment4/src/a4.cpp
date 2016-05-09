#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<omp.h>
#include<unistd.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define NUMT 4

int	NowYear;		// 2016 - 2021
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;
const float AMP_PRECIP_PER_MONTH =		6.0;
const float RANDOM_PRECIP =			2.0;

const float AVG_TEMP =				50.0;
const float AMP_TEMP =				20.0;
const float RANDOM_TEMP =			10.0;

void GrainDeer();
void Grain();
void Watcher();
void MyAgent();
int Ranf(int ilow, int ihigh);
float Ranf(float low, float high);

int main(int argc, char *argv[])
{

	NowMonth =    0;
	NowYear  = 2016;

	// starting state (feel free to change this if you want):
	NowNumDeer = 1;
	NowHeight =  1.;

	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
		NowPrecip = 0.;

	omp_set_num_threads(NUMT);
	while(true) {
		double time0 = omp_get_wtime();

		#pragma omp parallel sections
		{
			#pragma omp section
			{
				GrainDeer( );
			}
			#pragma omp section
			{
				Grain( );
			}
			#pragma omp section
			{
				Watcher( );
			}
			#pragma omp section
			{	
				MyAgent( );
			}
		}       // implied barrier -- all functions must return in order to allow any of them to get past here
		double time1 = omp_get_wtime( );
		//double megaoperations = (double)(someBigNumber)/(time1-time0)/1000000.0;
		//printf("|Num Threads: %2i|MMults: %5f| padding: %3i | Time: %8f |\n", NUMT, megaoperations, 1, (time1-time0));

		sleep(1);
	}
	return 0;
}

void GrainDeer() {
	//Compute into tmp variables
	printf("GrainDeer() function called\n");
	#pragma omp barrier
	//copy into global variables
	#pragma omp barrier
	//do nothing
	#pragma omp barrier

}

void Grain() {
	printf("Grain() function called\n");
	//compute into tmp variables
	#pragma omp barrier
	//copy into global variables
	#pragma omp barrier
	//do nothing
	#pragma omp barrier

}

void Watcher() {
	//Barrier
	#pragma omp barrier
	//do nothing
	#pragma omp barrier
	//print results and increment month
	printf("#Deer: %3i | Height: %3f | Precip: %3f | Temp: %3f\n", NowNumDeer, NowHeight, NowPrecip, NowTemp);
	//Calculate new temperature and precipitation
	//do stuff here
	#pragma omp barrier

}

void MyAgent() {
	printf("MyAgent() function called\n");
	#pragma omp barrier
	#pragma omp barrier
	#pragma omp barrier

}

float Ranf( float low, float high )
{
	        float r = (float) rand( );               // 0 - RAND_MAX

		        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int Ranf( int ilow, int ihigh )
{
	        float low = (float)ilow;
		        float high = (float)ihigh + 0.9999f;

			        return (int)(  Ranf(low,high) );
}
