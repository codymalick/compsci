#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h> /*pthread.h include for pthreads*/
#include<time.h>

/*mutex declaration*/
pthread_mutex_t read_mutex;
pthread_mutex_t write_mutex;
pthread_mutex_t delete_mutex;

struct node {
	int data;
	struct node *next;
};

struct list {
	struct node *head;
	int size;
};

void* read(void *ptr);
void* write(void *ptr);
void* del(void *ptr);

void print_status();
struct node* create_node();

int main(int argc, char *argv[])
{
	/* seed random number generator */
	srand(time(NULL));

	/* Philosopher threads */
	pthread_t readt, readt2, writet, writet2, delt, delt2;

	/* pthread arrays to simplify initialization */
	pthread_t* read_threads[2] = {&readt, &readt2};
	pthread_t* write_threads[2] = {&writet, &writet2};
	pthread_t* delete_threads[2] = {&delt, &delt2};

	struct list *l_list = (struct list *)malloc(sizeof(struct list));
	l_list->size = 0;

	int err_check = 0;

	/* Loop through thread array, spawn philosopher threads */
	for(int i = 0; i < 2; i++) {
		err_check = pthread_create(write_threads[i], NULL, write,
			       (void*)l_list);

		if(err_check) {
			fprintf(stderr, "Error, a thread failed to create -"
				       	"exit: %i\n", err_check);
			exit(err_check);
		}
	}

	for(int i = 0; i < 2; i++) {
		err_check = pthread_create(delete_threads[i], NULL, del,
						 (void*)l_list);

		if(err_check) {
			fprintf(stderr, "Error, a thread failed to create -"
								"exit: %i\n", err_check);
			exit(err_check);
		}
	}
	for(int i = 0; i < 2; i++) {
		err_check = pthread_create(read_threads[i], NULL, read,
			       (void*)l_list);

		if(err_check) {
			fprintf(stderr, "Error, a thread failed to create -"
				       	"exit: %i\n", err_check);
			exit(err_check);
		}
	}
	/* Join threads */
	pthread_join(delt, NULL);
	pthread_join(delt2, NULL);
	pthread_join(writet, NULL);
	pthread_join(writet2, NULL);
	pthread_join(readt, NULL);
	pthread_join(readt2, NULL);

	/* free stuff */
	free(l_list);
	return 0;
}

struct node* create_node() {
	struct node *node = (struct node *)malloc(sizeof(struct node));
	node->data = rand();
	node->next = NULL;
	return node;
}

void* read(void *ptr) {
	struct list *l_list = (struct list*)ptr;
	printf("read thread!\n");
	while(1) {

	}
}
void* write(void *ptr) {
	struct list *l_list = (struct list*)ptr;
	printf("write thread!\n");
	while(1) {
		pthread_mutex_lock(&write_mutex);
		struct node *new_node = create_node();
		if(l_list->size == 0) {
			l_list->head = new_node;
		} else {
			struct node *cur_node = l_list->head;
			while(cur_node->next != NULL) {
				cur_node = cur_node->next;
			}
			cur_node->next = new_node;
			printf("Added a node with value %i\n", cur_node->data);
		}
		l_list->size++;
		pthread_mutex_unlock(&write_mutex);

	}
}

void* del(void *ptr) {
	struct list *l_list = (struct list*)ptr;
	printf("delete thread!\n");
	while(1) {

	}
}
