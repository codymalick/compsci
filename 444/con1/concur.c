#include<stdlib.h>
#include<unistd.h>
#include<pthread.h> /*pthread.h include for pthreads*/
#include"mt19937ar.c" /*twister*/
#include"rdrand_comp.c" /*functions for rdrand call*/
#include<stdint.h>

/*constants for random numbers in the instruction for the assignment*/
#define PUP 7
#define PLO 3
#define CUP 9
#define CLO 2
#define UP 999
#define LO 0

/*global set to tell the program to use rdrand or not*/
int rdrand = 0;

/*mutex declaration*/
pthread_mutex_t mutex;

struct message {
	int number;
	int wait_period;
};

/*buffer is a basic queue, first in, first out implementation*/
struct buffer {
	struct message b[32];
	int size;
};

void buff_push(struct buffer *buff, struct message m);
void buff_pop(struct buffer *buff);
int rand_gen(int upr_bound, int lwr_bound);
void *producer_function(void *ptr);
void *consumer_function(void *ptr);
void print_column(char col1[], int col2, int col3, int col4);

int main(int argc, char *argv[])
{
	
	/*check if rdrand is supported*/
	if(get_drng_support()) {
		printf("Using rdrand\n");
		rdrand = 1;
	} else {
		printf("Using Mersenne Twister\n");
		//seed random number generator
		init_genrand(time(NULL));
	}

	/*thread declaration*/
	pthread_t producer;
	pthread_t consumer;

	/*buffer struct init*/
	struct buffer *buff;
	buff = (struct buffer *)malloc(sizeof(struct buffer));
	buff->size = 0;

	/*mutex init*/
	pthread_mutex_init(&mutex, NULL);

	/*variables for error checking*/
	int t_1, t_2;

	/*
	 *Create new thread
	 *pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	 *	void *(*start_routine) (void *), void *arg);
	 */
	t_1 = pthread_create(&producer, NULL, producer_function, (void*)buff);

	if(t_1) {
		fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_1);
		exit(t_1);
	}

	t_2 = pthread_create(&consumer, NULL, consumer_function, (void*)buff);

	if(t_2) {
		fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_2);
		exit(t_2);
	}

	/*
	 * This code is never run due to infinite loops, but if the program does
	 * terminate, all memory is freed and threads complete properly.
	 */

	/*wait for threads to complete, barrier*/
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);

	free(buff);
	exit(0);
}

void buff_push(struct buffer *buff, struct message m)
{
	buff->b[buff->size] = m;
	buff->size++;
}

void buff_pop(struct buffer *buff)
{
	/*shift all items towards the front one space*/
	for(int i = 1; i < buff->size; i++) {
			buff->b[i-1] = buff->b[i];
	}
	buff->size--;
}

int rand_gen(int upr_bound, int lwr_bound)
{
	if(rdrand) {
		/*rdrand required unsigned 32 bit int*/
		uint32_t num = 0;
		if(rdrand32_step(&num))
			return abs((int)num) % upr_bound + lwr_bound;
	}
	/*abs to solve negative numbers from generator*/
	return abs((int)genrand_int32()) % upr_bound + lwr_bound;
}

void *producer_function(void *ptr)
{
	/*cast void pointer to buffer pointer*/
	struct buffer *buff = (buffer *)ptr;
	struct message item;

	char prod[] = "Producer";

	while(1) {
		/*check if buffer is full*/
		if(buff->size < (sizeof(buff->b)/sizeof(struct message))) {
			pthread_mutex_lock(&mutex);

			item.number = rand_gen(UP, LO);
			item.wait_period = rand_gen(CUP, CLO);

			buff_push(buff, item);

			print_column(prod, item.number, item.wait_period, buff->size);
			pthread_mutex_unlock(&mutex);

			sleep(rand_gen(PUP, PLO));
			}
		}
}

void *consumer_function(void *ptr)
{
	/*cast void pointer to buffer pointer*/
	struct buffer *buff = (buffer*)ptr;
	struct message item;

	char cons[] = "Consumer";

	int wait_time = 0;
	while(1) {

		if(buff->size > 0) {

			pthread_mutex_lock(&mutex);

			item = buff->b[0];
			buff_pop(buff);

			print_column(cons, item.number, item.wait_period, buff->size);
			pthread_mutex_unlock(&mutex);

			sleep(item.wait_period);
		}
	}
}

void print_column(char col1[], int col2, int col3, int col4) {
	printf("%s | Number:%3i | Wait:%2i | buffer: %2i |\n", col1, col2, col3, col4);
}
