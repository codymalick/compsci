#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h> //pthread.h include for pthreads
#include<time.h>
#include<semaphore.h>

// Globals
bool is_paper = false;
bool is_tobacco = false;
bool is_match = false;

// Agent Semaphores
sem_t agent_sem;
sem_t tobacco;
sem_t paper;
sem_t match;
sem_t pusher_tobacco;
sem_t pusher_match;
sem_t pusher_paper;

pthread_mutex_t mutex;

void* agent_a(void *tmp);
void* agent_b(void *tmp);
void* agent_c(void *tmp);
void* tobacco_smoker(void *tmp);
void* paper_smoker(void *tmp);
void* match_smoker(void *tmp);
void* pusher_a(void *tmp);
void* pusher_b(void *tmp);
void* pusher_c(void *tmp);
void smoke(int input);

int main(int argc, char *argv[])
{
	/* threads */
	pthread_t t_1, t_2, t_3, t_4, t_5, t_6, t_7, t_8, t_9;

	// Semaphore init
	sem_init(&agent_sem, 0, 1);
	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&match, 0, 0);
	sem_init(&pusher_match, 0, 0);
	sem_init(&pusher_tobacco, 0, 0);
	sem_init(&pusher_paper, 0, 0);


	pthread_mutex_init(&mutex, NULL);

	int *useless;
	int err_check = 0;
	
	err_check = pthread_create(&t_1, NULL, tobacco_smoker, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}
	err_check = pthread_create(&t_2, NULL, match_smoker, (void*)useless);	
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}	
	err_check = pthread_create(&t_3, NULL, paper_smoker, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}	
	err_check = pthread_create(&t_4, NULL, agent_a, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}	
	err_check = pthread_create(&t_5, NULL, agent_b, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}	
	err_check = pthread_create(&t_6, NULL, agent_c, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}
	err_check = pthread_create(&t_7, NULL, pusher_a, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}
	err_check = pthread_create(&t_8, NULL, pusher_b, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}
	err_check = pthread_create(&t_9, NULL, pusher_c, (void*)useless);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create -"
			       	"exit: %i\n", err_check);
		exit(err_check);
	}
	
	




	/* Join threads */
	pthread_join(t_1, NULL);
	pthread_join(t_2, NULL);
	pthread_join(t_3, NULL);
	pthread_join(t_4, NULL);
	pthread_join(t_5, NULL);
	pthread_join(t_6, NULL);
	pthread_join(t_7, NULL);
	pthread_join(t_8, NULL);
	pthread_join(t_9, NULL);
	/* free stuff */
	return 0;
}

void* agent_a(void *tmp) {
	while(1) {
		printf("agent a waiting\n");
		sem_wait(&agent_sem);
		printf("agent a going\n");
		sem_post(&tobacco);
		printf("agent a - tobacco\n");
		sem_post(&paper);	
		printf("agent a - paper\n");	
	}
}

void* agent_b(void *tmp) {
	while(1) {
		printf("agent b waiting\n");
		sem_wait(&agent_sem);
		printf("agent b going\n");
		sem_post(&paper);
		printf("agent b - paper\n");
		sem_post(&match);
		printf("agent b - match\n");
	}
}

void* agent_c(void *tmp) {
	while(1) {
		printf("agent c waiting\n");
		sem_wait(&agent_sem);
		printf("agent c going\n");
		sem_post(&tobacco);
		printf("agent c - tobacco\n");
		sem_post(&match);
		printf("agent c - match\n");
	}
}

void* pusher_a(void *tmp) {
	while(1) {
		printf("pusher a waiting\n");
		sem_wait(&tobacco);
		printf("pusher a going\n");
                pthread_mutex_lock(&mutex);
		// If pusher B has run
		if(is_paper) {
			is_paper = false;
			sem_post(&pusher_match);
		} else if(is_match) {
			is_match = false;
			sem_post(&pusher_paper);
		} else {
			is_tobacco = true;
		}
		pthread_mutex_unlock(&mutex);
	}
}

void* pusher_b(void *tmp) {
	while(1) {
		printf("pusher b waiting\n");
		sem_wait(&paper);
		printf("pusher b going\n");
		pthread_mutex_lock(&mutex);
		// If pusher C has run
		if(is_match) {
			is_match = false;
			sem_post(&pusher_tobacco);
		} else if(is_tobacco) {
			is_tobacco = false;
			sem_post(&pusher_match);
		} else {
			is_paper = true;
		}
		pthread_mutex_unlock(&mutex);
	}
}

void* pusher_c(void *tmp) {
	while(1) {
		printf("pusher c waiting\n");
		sem_wait(&match);
		printf("pusher c going\n");
		pthread_mutex_lock(&mutex);
		// If pusher A has run
		if(is_tobacco) {
			is_tobacco = false;
			sem_post(&pusher_paper);
		} else if(is_paper) {
			is_paper = false;
			sem_post(&pusher_tobacco);
		} else {
			is_match = true;
		}
		pthread_mutex_unlock(&mutex);
	}
}

void* tobacco_smoker(void *tmp) {
	while(1) {
		sem_wait(&pusher_tobacco);	
		sem_post(&agent_sem);
		smoke(1);
	}
}

void* paper_smoker(void *tmp) {
	while(1) {
		sem_wait(&pusher_paper);
		sem_post(&agent_sem);
		smoke(2);
	}
}

void* match_smoker(void *tmp) {
	while(1) {
		sem_wait(&pusher_match);
		sem_post(&agent_sem);
		smoke(3);
	}
}

void smoke(int input) {
	printf("#%i is smoking\n", input);
	sleep(2);
}
