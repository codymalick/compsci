#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h> /*pthread.h include for pthreads*/
#include<time.h>
#include<semaphore.h>

sem_t semaphore;
pthread_mutex_t run_mutex;
int running = 0;

void thread_loop(void *ptr);

int main(int argc, char *argv[])
{
	/* Philosopher threads */
	pthread_t t_1, t_2, t_3, t_4, t_5, t_6;

	/* pthread arrays to simplify initialization */
	pthread_t* threads[6] = {&t_1, &t_2, &t_3, &t_4, &t_5, &t_6};
	sem_init(&semaphore, 0, 3);
	pthread_mutex_init(&run_mutex, NULL);

	int err_check = 0;
	/* Loop through thread array, spawn threads */
	for(int i = 0; i < 6; i++) {
		err_check = pthread_create(threads[i], NULL, thread_loop,
			       (void *)NULL);
		if(err_check) {
			fprintf(stderr, "Error, a thread failed to create -"
				       	"exit: %i\n", err_check);
			exit(err_check);
		}
	}
	
	/* Join threads */
	pthread_join(t_1, NULL);
	pthread_join(t_2, NULL);
	pthread_join(t_3, NULL);
	pthread_join(t_4, NULL);
	pthread_join(t_5, NULL);
	pthread_join(t_6, NULL);

	/* free stuff */
	return 0;
}

void thread_loop(void *ptr) {
	while(1) {
		//Try and decrement
		sem_wait(&semaphore);
		pthread_mutex_lock(&run_mutex);
		running++;
		pthread_mutex_unlock(&run_mutex);
	 	printf("Running: %i\n", running);
	 	sleep(1);
		pthread_mutex_lock(&run_mutex);
		running--;
		pthread_mutex_unlock(&run_mutex);
		if(running == 0) {
			printf("unlocking blocked threads\n");
			sem_post(&semaphore);
			sem_post(&semaphore);
			sem_post(&semaphore);
		}
	}
}
