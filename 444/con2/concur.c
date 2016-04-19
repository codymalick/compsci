#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h> /*pthread.h include for pthreads*/
#include<time.h>

/*mutex declaration*/
pthread_mutex_t mutex;

struct philosopher {
	char *name;
	struct fork *left_fork;
	struct fork *right_fork;
	pthread_mutex_t mutex;
};
struct fork {
    char *name;
	int used;
};
struct table {
	struct fork *fork1;
	struct fork *fork2;
	struct fork *fork3;
	struct fork *fork4;
	struct fork *fork5;	
};

void* start_eating(void *ptr);
void think(struct philosopher *philo);
void get_forks(struct philosopher *philo);
void eat(struct philosopher *philo);
void put_forks(struct philosopher *philo);
int rand_eat();
int rand_think();
void print_status();

int main(int argc, char *argv[])
{
	/* seed random number generator */
	srand(time(NULL));

	/* Philosopher threads */
	pthread_t locke, voltaire, demosthenes, plato, aristotle;

	/* pthread array to simplify initialization */
	pthread_t* philos[5] = {&locke, &voltaire, &demosthenes, &plato, &aristotle};

	/* Construct the table and forks */
	struct table *tab = (struct table *)malloc(sizeof(struct table));
	tab->fork1 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork1->used = 0;
    tab->fork1->name = "fork1";    

	tab->fork2 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork2->used = 0;
    tab->fork2->name = "fork2";

	tab->fork3 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork3->used = 0;
    tab->fork3->name = "fork3";

	tab->fork4 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork4->used = 0;
    tab->fork4->name = "fork4";

	tab->fork5 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork5->used = 0;
    tab->fork5->name = "fork5";

	/* all philosopher structs in order, l == locke, v == voltaire, etc */
	struct philosopher *l = (struct philosopher *)
		malloc(sizeof(struct philosopher));
	struct philosopher *v = (struct philosopher *)
		malloc(sizeof(struct philosopher));
	struct philosopher *d = (struct philosopher *)
		malloc(sizeof(struct philosopher));
	struct philosopher *p = (struct philosopher *)
		malloc(sizeof(struct philosopher));
	struct philosopher *a = (struct philosopher *)
		malloc(sizeof(struct philosopher));

	l->name = "locke";
	v->name = "voltaire";
	d->name = "demosthenes";
	p->name = "plato";
	a->name = "aristotle";

	l->left_fork = tab->fork1;
	l->right_fork = tab->fork2;

	v->left_fork = tab->fork2;
	v->right_fork = tab->fork3;

	d->left_fork = tab->fork3;
	d->right_fork = tab->fork4;

	p->left_fork = tab->fork4;
	p->right_fork = tab->fork5;

	a->left_fork = tab->fork5;
	a->right_fork = tab->fork1;

	struct philosopher *p_array[5] = {l, v, d, p, a};
	int err_check = 0;

	/* Loop through thread array, spawn philosopher threads */
	for(int i = 0; i < 5; i++) {
		err_check = pthread_create(philos[i], NULL, start_eating,
			       (void*)p_array[i]);

		if(err_check) {
			fprintf(stderr, "Error, a thread failed to create -"
				       	"exit: %i\n", err_check);
			exit(err_check);
		}
	}

	/* Join threads */
	pthread_join(locke, NULL);
	pthread_join(voltaire, NULL);
	pthread_join(demosthenes, NULL);
	pthread_join(plato, NULL);
	pthread_join(aristotle, NULL);
	
	/* free stuff */
	free(a);
	free(v);
	free(l);
	free(p);
	free(d);
	free(tab);	
	return 0;
}

int rand_eat() {
	return rand() % 9 + 2;
}

int rand_think() {
	return rand() % 20 + 1;
}
void* start_eating(void *ptr) {

	struct philosopher *philo = (struct philosopher *)ptr;
	
	pthread_mutex_init(&philo->mutex, NULL);

	while(1) {
		/* think() */
		think(philo);
		get_forks(philo);
		eat(philo);
		put_forks(philo);
	}
}

void think(struct philosopher *philo) {
	/*FIX, THINK 1-20 SECONDS*/
	printf("| %11s | %11s |\n", philo->name, "thinking");
	sleep(rand_think());
}


void get_forks(struct philosopher *philo) {
	printf("| %11s | %11s |\n", philo->name, "get forks");
	while(1) {
		if(!philo->left_fork->used &&
			       	!philo->right_fork->used) {
			pthread_mutex_lock(&philo->mutex);
			philo->right_fork->used = 1;
			philo->left_fork->used = 1;
			pthread_mutex_unlock(&philo->mutex);
            printf("| %11s | %11s | %s and %s |\n", philo->name,
                     "picked up", philo->left_fork->name,
                     philo->right_fork->name);
			break;
		}
	}	
}

void eat(struct philosopher *philo) {
	printf("| %11s | %11s |\n", philo->name, "eating");
	/* eat for 2-9 seconds */
	sleep(rand_eat());
}

void put_forks(struct philosopher *philo) {
	printf("| %11s | %11s |\n", philo->name, "put forks");
	pthread_mutex_lock(&philo->mutex);
	philo->right_fork->used = 0;
	philo->left_fork->used = 0;
	pthread_mutex_unlock(&philo->mutex);
    printf("| %11s | %11s | %s and %s |\n", philo->name,
                     "put down", philo->left_fork->name,
                     philo->right_fork->name);
}
