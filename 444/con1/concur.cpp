#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
//pthread.h include for pthreads (incase it wasn't obvious)
#include<pthread.h>

//mutex declaration
pthread_mutex_t mutex;

struct message {
    int number;
    int wait_period;
};

struct buffer {
    struct message b[32];
    int size;  
};

void buff_push(struct buffer *buff, struct message m) {
    buff->b[buff->size] = m;
    buff->size++;
}

void buff_pop(struct buffer *buff) {
    //shift all items towards the front one space
    for(int i = 1; i < buff->size; i++) {
        buff->b[i-1] = buff->b[i];
    }
    buff->size--;
}

int rand_gen() {
    srand(time(NULL));
    return rand() % 10;
}

void *factory_function(void *ptr) {
    //cast void pointer to buffer pointer
    struct buffer *buff = (buffer *)ptr;
    struct message item;

    while(1) {
        
        //check if buffer is full
        if(buff->size < (sizeof(buff->b)/sizeof(struct message))) {
            
            //lock resources
            pthread_mutex_lock(&mutex); 
            
            //initialize new message
            item.number = rand_gen();
            item.wait_period = rand_gen();
           
            buff_push(buff, item); 
            
            //unlock resources
            pthread_mutex_unlock(&mutex);
            
            printf("%i:%i:%i:%i\n",item.number, item.wait_period, buff->size,
                (int)(sizeof(buff->b)/sizeof(struct message)));
        }
    }
}

void *consumer_function(void *ptr) {
    
    //cast void pointer to buffer pointer
    struct buffer *buff = (buffer*)ptr;
    struct message item;
    int wait_time = 0;
    while(1) {

        if(buff->size > 0) {
            
            //lock resources
            pthread_mutex_lock(&mutex);

            item = buff->b[0];
            buff_pop(buff);
                     
            //unlock resources
            pthread_mutex_unlock(&mutex);

            sleep(item.wait_period);
            printf("Number: %i, Wait: %i\n", item.number, item.wait_period);
        }
    }

    printf("%s\n", "print 2");
}

int main(int argc, char *argv[]) {

    //thread declaration
    pthread_t factory, consumer;

    //buffer struct init
    struct buffer *buff;
    buff = (struct buffer *)malloc(sizeof(struct buffer));
    buff->size = 0;
    
    //variables for error checking
    int t_1, t_2;

    //mutex init 
    pthread_mutex_init(&mutex, NULL);
    
    //Create new thread
    //pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    //  void *(*start_routine) (void *), void *arg);
    t_1 = pthread_create(&factory, NULL, factory_function, (void*)buff);
    if(t_1) {
        fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_1);
        exit(t_1);
    }

    t_2 = pthread_create(&consumer, NULL, consumer_function, (void*)buff);
    if(t_2) {
        fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_2);
        exit(t_2);
    }


    //printf("pthread_create for thread 1 returns: %i\n", t_1);
    //printf("pthread_create for thread 2 returns: %i\n", t_2);
    
    //wait for threads to complete, barrier
    pthread_join(factory, NULL);
    pthread_join(consumer, NULL);

    //free buffer struct
    free(buff);
    exit(0);
}
