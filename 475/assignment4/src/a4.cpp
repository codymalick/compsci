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
bool	ALIENS = 0;

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;
const float AMP_PRECIP_PER_MONTH =		6.0;
const float RANDOM_PRECIP =			2.0;

const float AVG_TEMP =				50.0;
const float AMP_TEMP =				20.0;
const float RANDOM_TEMP =			10.0;

const float e = 2.71828182845904523536;

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

	omp_set_num_threads(NUMT);

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


	return 0;
}

void GrainDeer() {
	while( NowYear <= 2021 ) {
		
		//Compute into tmp variables
		int tempNumDeer = NowNumDeer;
		if (NowNumDeer*ONE_DEER_EATS_PER_MONTH > NowHeight)
			tempNumDeer--;
		
		if (NowNumDeer*ONE_DEER_EATS_PER_MONTH < NowHeight)
			tempNumDeer++;
		else if(1*ONE_DEER_EATS_PER_MONTH < NowHeight)
			tempNumDeer++;

		if(ALIENS)
			tempNumDeer--;	
		if (tempNumDeer < 0)
			tempNumDeer = 0;

		#pragma omp barrier
		NowNumDeer = tempNumDeer;	
		//copy into global variables
		#pragma omp barrier
		//do nothing
		#pragma omp barrier
	}
}
// Grain growth
void Grain() {
	while( NowYear <= 2021 ) {
		float temp_fact = exp(-1*(pow(((NowTemp - AVG_TEMP)/10),2)));
		float temp_prec = exp(-1*(pow(((NowPrecip - AVG_PRECIP_PER_MONTH)/10),2)));
		//printf("temp_fact: %f, Prec_fact: %f\n", temp_fact, temp_prec);

		//compute into tmp variables
		float temp_height = temp_fact * temp_prec * GRAIN_GROWS_PER_MONTH;
		temp_height -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

		//handle zero case
		if(temp_height < 0)
			temp_height = 0;
		#pragma omp barrier
		//copy into global variables
		NowHeight = temp_height;

		#pragma omp barrier
		//do nothing
		#pragma omp barrier
	}
}

void Watcher() {
	while( NowYear <= 2021 ) {
		//Barrier
		#pragma omp barrier
		//do nothing
		#pragma omp barrier
		//print results and increment month
		printf("%i,%i,%i,%f,%f,%f\n", 
				NowMonth, NowYear, NowNumDeer, NowHeight, NowPrecip, NowTemp);
		if(NowMonth == 11) {
			NowMonth = 0;
			NowYear++;
		} else {
			NowMonth++;
		}
		//Calculate new temperature and precipitation
		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
		NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
		if( NowPrecip < 0. )
			NowPrecip = 0.;
	
		#pragma omp barrier
	}
}

void MyAgent() {
	while( NowYear <= 2021 ) {
		int alien_chance = Ranf(0, 10);
		if(alien_chance < 4 && NowNumDeer > 1) {
			ALIENS = true;
			//printf("Aliens are coming\n");
		} else {
			ALIENS = false;
		}
		#pragma omp barrier
		#pragma omp barrier
		#pragma omp barrier
	}
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
