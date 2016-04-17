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

struct args {
	struct table tab;
	struct philosopher philo;
};

void* start_eating(void *ptr);
void think(struct philosopher *philo);
void get_forks(struct philosopher *philo);
void eat(struct philosopher *philo);
void put_forks(struct philosopher *philo);

int main(int argc, char *argv[])
{
	/* Philosopher threads */
	pthread_t locke, voltaire, demosthenes, plato, aristotle;

	/* pthread array to simplify initialization */
	pthread_t* philos[5] = {&locke, &voltaire, &demosthenes, &plato, &aristotle};

	/* Construct the table and forks */
	struct table *tab = (struct table *)malloc(sizeof(struct table));
	tab->fork1 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork1->used = 0;

	tab->fork2 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork2->used = 0;

	tab->fork3 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork3->used = 0;

	tab->fork4 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork4->used = 0;

	tab->fork5 = (struct fork *)malloc(sizeof(struct fork));
	tab->fork5->used = 0;

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

	struct args *arg1 = (struct args *)malloc(sizeof(struct args));
	arg1->tab = *tab;
	arg1->philo = *l;
	
	struct args *arg2 = (struct args *)malloc(sizeof(struct args));
	arg2->tab = *tab;
	arg2->philo = *v;

	struct args *arg3 = (struct args *)malloc(sizeof(struct args));
	arg3->tab = *tab;
	arg3->philo = *d;

	struct args *arg4 = (struct args *)malloc(sizeof(struct args));
	arg4->tab = *tab;
	arg4->philo = *a;

	struct args *arg5 = (struct args *)malloc(sizeof(struct args));
	arg5->tab = *tab;
	arg5->philo = *p;

	struct args* args_array[5] = {arg1, arg2, arg3, arg4, arg5};
	int err_check = 0;

	/* Loop through thread array, spawn philosopher threads */
	for(int i = 0; i < 5; i++) {
		err_check = pthread_create(philos[i], NULL, start_eating,
			       (void*)args_array[i]);

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
	free(arg1);
	free(arg2);
	free(arg3);
	free(arg4);
	free(arg5);
	free(tab);	
	return 0;
}

void* start_eating(void *ptr) {
	struct args *arg = (struct args *)ptr;

	struct table *tab = &arg->tab;
	struct philosopher *philo = &arg->philo;
	
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
	sleep(2);
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
			break;
		}
	}	
}

void eat(struct philosopher *philo) {
	printf("| %11s | %11s |\n", philo->name, "eating");
	/* eat for 2-9 seconds */
	sleep(4);
}

void put_forks(struct philosopher *philo) {
	printf("| %11s | %11s |\n", philo->name, "put forks");
	pthread_mutex_lock(&philo->mutex);
	philo->right_fork->used = 0;
	philo->left_fork->used = 0;
	pthread_mutex_unlock(&philo->mutex);
}
