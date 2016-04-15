#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h> /*pthread.h include for pthreads*/

/*mutex declaration*/
pthread_mutex_t mutex;

struct philosopher {
	char *name;
	struct fork *left_fork;
	struct fork *right_fork;
	pthread_mutex_t mutex;
};
struct fork {
	int used;
};
struct table {
	struct fork *fork1;
	struct fork *fork2;
	struct fork *fork3;
	struct fork *fork4;
	struct fork *fork5;	
};

void print_something(void *ptr);

int main(int argc, char *argv[])
{
	/* Philosopheri threads */
	pthread_t locke, voltaire, demosthenes, plato, aristotle;

	struct table *tab = (struct table *)malloc(sizeof(struct table));
	tab->fork1 = (struct fork *)malloc(sizeof(struct fork));	
	tab->fork2 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork3 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork4 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork5 = (struct fork *)malloc(sizeof(struct fork));
	
	/*
	 * think()
	 * get_forks()
	 * eat()
	 * put_forks()
	 */
	
		
	free(tab);	
	return 0;
}

void print_something(void *ptr) {
	//struct philosopher philo = (philosopher *)ptr;
	printf("wohoo!");
	return;
}
