#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h> /*pthread.h include for pthreads*/
#include<time.h>
#include<semaphore.h>

#define CHAIRS 4
sem_t chair_limit;
sem_t barb_sleep;
sem_t current_job;

pthread_mutex_t run_mutex;

/* structures */
struct customer {
	int id;
	int hair_length;
};
struct chair {
	struct customer *c;
};
struct barber_shop {
	struct chair *w_chairs[CHAIRS];
	struct chair *b_chair;
};
struct c_args {
	struct customer *c;
	struct barber_shop *bs;
};
/* functions */
void* barber_func(void *ptr);
void* customer_func(void *ptr);

void get_hair_cut(int length, int id);
void cut_hair(int length);
void sit_customer(struct barber_shop *bs, struct customer *c);
void remove_customer(struct barber_shop *bs, int chair);

int main(int argc, char *argv[])
{
	srand(time(NULL));

	/* threads, barber is thread 8 */
	pthread_t t_1, t_2, t_3, t_4, t_5, t_6, t_7, t_8;

	/* Customer threads, seven customers */
	pthread_t* c_threads[7] = {&t_1, &t_2, &t_3, &t_4, &t_5, &t_6, &t_7};

	/* struct initialization */
	struct customer *c1 = new struct customer;
	struct customer *c2 = new struct customer;
	struct customer *c3 = new struct customer;
	struct customer *c4 = new struct customer;
	struct customer *c5 = new struct customer;
	struct customer *c6 = new struct customer;
	struct customer *c7 = new struct customer;

	struct customer *customers[7] = {c1, c2, c3, c4, c5, c6, c7};
	for(int i = 0; i < 7; i++)
		customers[i]->id = i+1;

	struct barber_shop *bs = new struct barber_shop;
	for(int i = 0; i < CHAIRS; i++)
		bs->w_chairs[i] = new struct chair;
	
	bs->b_chair = new struct chair;

	sem_init(&chair_limit, 0, CHAIRS);
	sem_init(&current_job, 0, 0);
	sem_init(&barb_sleep, 0, 0);

	pthread_mutex_init(&run_mutex, NULL);

	int err_check = 0;
	printf("There are 4 chairs, a barber shop chair, a barber, and 7 customers\n");
	err_check = pthread_create(&t_8, NULL, barber_func, (void *)bs);
	if(err_check) {
		fprintf(stderr, "Error, a thread failed to create - exit %i\n",
				err_check);
		exit(err_check);
	}
	
	/* Loop through thread array, spawn customer threads */
	for(int i = 0; i < 7; i++) {
		struct c_args *c_a = new struct c_args;
		c_a->c = customers[i];
		c_a->bs = bs;

		err_check = pthread_create(c_threads[i], NULL, customer_func,
			       (void*)c_a);
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
	pthread_join(t_7, NULL);
	pthread_join(t_8, NULL);

	/* free stuff */
	for(int i = 0; i < 7; i++) {
		delete customers[i];
	}
	for(int i = 0; i < CHAIRS; i++) {
		delete bs->w_chairs[i];
	}
	delete bs->b_chair;
	delete bs;

	return 0;
}

void* barber_func(void *ptr) {
	struct barber_shop *bs = (struct barber_shop *)ptr;
	int chair_cut = 0;
	while(true) {
		/* will sleep until sem_post */
		sem_wait(&barb_sleep);

		/* start next job */
		sem_post(&current_job);

		/* cut hair */
		cut_hair(bs->w_chairs[chair_cut]->c->hair_length);
		remove_customer(bs, chair_cut);

		/* iterate chair location */
		if(chair_cut == 3)
			chair_cut = 0;
		else
			chair_cut++;
	}

}
void* customer_func(void *ptr) {
	struct c_args *c_a = (struct c_args *) ptr;
	struct customer *c = c_a->c;
	struct barber_shop *bs = c_a->bs;
	while(true) {
		/* maximum hair length 8, min 2 */
		c->hair_length = rand() % 8 + 2;
		printf("Customer %i, length %i\n", c->id, c->hair_length);

		/* try and sit in the waiting room */
		if(sem_trywait(&chair_limit)) {
			printf("Customer %i left!\n", c->id);
			/* sleep for some time before resetting */
			sleep(5);
		} else {
			sit_customer(bs, c);
			printf("Customer %i is sitting\n", c->id);

			/* if the barber is asleep, wake him, also customer counter */
			sem_post(&barb_sleep);
			wait(current_job);
			get_hair_cut(c->hair_length, c->id);
				

		}
	}
}
/* Finds an empty chair in the waiting room, and sits customer */
void sit_customer(struct barber_shop *bs, struct customer *c) {
	for(int i = 0; i < CHAIRS; i++) {
		if(bs->w_chairs[i]->c == NULL) {
			bs->w_chairs[i]->c = c;
			break;
		}
	}
}

void remove_customer(struct barber_shop *bs, int chair) {
	bs->w_chairs[chair]->c = NULL;
	sem_post(&chair_limit);
}

void cut_hair(int length) {
	printf("The barber is cutting hair! - ");
	sleep(length);
}

void get_hair_cut(int length, int id) {
	printf("Customer %i is getting their hair cut!\n", id);
	sleep(length);
}
