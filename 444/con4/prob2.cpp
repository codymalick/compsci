#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h> /*pthread.h include for pthreads*/
#include<time.h>
#include<semaphore.h>

#define CHAIRS 4
sem_t chair_limit;
sem_t barb_sleep;

pthread_mutex_t run_mutex;

/* structures */
struct customer {
	int id;
	int hair_length;
	struct chair *c;
	sem_t customer_s;
};
struct chair {
	struct customer *c;
};
struct queue {
	struct chair *chairs[4];
	int size;
};
struct barber_shop {
	struct chair *w_chairs[CHAIRS];
	struct chair *b_chair;
	struct queue *q;
};
struct c_args {
	struct customer *c;
	struct barber_shop *bs;
};
/* functions */
void queue_push(struct queue *q, struct chair *c);
void queue_pop(struct queue *q);

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
	for(int i = 0; i < 7; i++) {
		customers[i]->id = i+1;
		sem_init(&customers[i]->customer_s, 0, 0);
	}

	struct barber_shop *bs = new struct barber_shop;
	for(int i = 0; i < CHAIRS; i++)
		bs->w_chairs[i] = new struct chair;
	
	bs->b_chair = new struct chair;
	bs->q = new struct queue;
	bs->q->size = 0;

	sem_init(&chair_limit, 0, CHAIRS);
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
	delete bs->q;
	delete bs;

	return 0;
}

void queue_push(struct queue *q, struct chair *c) {
	q->chairs[q->size] = c;
	q->size++;
}

void queue_pop(struct queue *q) {
	for(int i = 1; i < q->size; i++) {
		q->chairs[i-1] = q->chairs[i];
	}
	q->size--;
}

void* barber_func(void *ptr) {
	struct barber_shop *bs = (struct barber_shop *)ptr;
	int chair_cut = 0;
	sem_t current_job;
	while(true) {
		/* will sleep until sem_post */
		sem_wait(&barb_sleep);
		
		current_job = bs->q->chairs[0]->c->customer_s;
		/* start next job */
		sem_post(&current_job);

		/* cut hair */
		cut_hair(bs->w_chairs[chair_cut]->c->hair_length);
	}

}
void* customer_func(void *ptr) {
	struct c_args *c_a = (struct c_args *) ptr;
	struct customer *c = c_a->c;
	struct barber_shop *bs = c_a->bs;
	sem_t c_s;
	while(true) {
		/* maximum hair length 8, min 2 */
		c->hair_length = rand() % 8 + 2;
		//printf("Customer %i, length %i\n", c->id, c->hair_length);

		/* try and sit in the waiting room */
		if(sem_trywait(&chair_limit)) {
			printf("Customer %i left!\n", c->id);
			/* sleep for some time before resetting */
			sleep(5);
		} else {
			c_s = c->customer_s;
			sit_customer(bs, c);
			printf("Customer %i is sitting\n", c->id);
			
			/* if the barber is asleep, wake him, also customer counter */
			sem_post(&barb_sleep);
			wait(c_s);
			get_hair_cut(c->hair_length, c->id);
						

		}
	}
}
/* Finds an empty chair in the waiting room, and sits customer */
void sit_customer(struct barber_shop *bs, struct customer *c) {
	for(int i = 0; i < CHAIRS; i++) {
		if(bs->w_chairs[i]->c == NULL) {
			bs->w_chairs[i]->c = c;
			c->c = bs->w_chairs[i];
			queue_push(bs->q, c->c);
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
