#include<stdio.h>
#include<time.h>
#include<stdlib.h>
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

int buff_push(struct buffer *buff) {
    
    return 0;
}

struct message buff_pop() {
    
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
        //printf("Size: %i, comp: %i\n", buff->size,(int)(sizeof(buff->b)/sizeof(struct message)));
        if(buff->size < (sizeof(buff->b)/sizeof(struct message))) {
            
            //initialize new message
            item.number = rand_gen();
            item.wait_period = rand_gen();

            printf("%i:%i:%i:%i\n",item.number, item.wait_period, buff->size,
                (int)(sizeof(buff->b)/sizeof(struct message)));
        }
    }
   
}

void *message_out2(void *ptr) {
    printf("%s\n", "print 2");
}

int main(int argc, char *argv[]) {

    //thread declaration
    pthread_t factory, consumer;

    struct buffer *buff;
    buff = (struct buffer *)malloc(sizeof(struct buffer));
    buff->size = 0;
    const char *char1 = "thread 1";
    const char *char2 = "thread 2";

    int t_1, t_2;

    printf("initial buff: %i\n", buff->size);
    //Create new thread
    //pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    //  void *(*start_routine) (void *), void *arg);
    t_1 = pthread_create(&factory, NULL, factory_function, (void*)buff);
    if(t_1) {
        fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_1);
        exit(t_1);
    }

    //t_2 = pthread_create(&consumer, NULL, message_out2, (void*)char2);
    //if(t_2) {
        //fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_2);
        //exit(t_2);
    //}


    printf("pthread_create for thread 1 returns: %i\n", t_1);
    //printf("pthread_create for thread 2 returns: %i\n", t_2);
    
    //wait for threads to complete, barrier
    pthread_join(factory, NULL);
    pthread_join(consumer, NULL);

    free(buff);
        exit(0);
}
