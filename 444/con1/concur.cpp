#include<stdio.h>
#include<stdlib.h>
//pthread.h include for pthreads (incase it wasn't obvious)
#include<pthread.h>

struct message {
    int number;
    int wait_period;
};

void *message_out(void *ptr) {
    printf("%s\n", "print 1");
}

void *message_out2(void *ptr) {
    printf("%s\n", "print 2");
}

int main(int argc, char *argv[]) {

    //thread declaration
    pthread_t factory, consumer;

    const char *char1 = "thread 1";
    const char *char2 = "thread 2";

    int t_1, t_2;

    //Create new thread
    //pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    //  void *(*start_routine) (void *), void *arg);
    t_1 = pthread_create(&factory, NULL, message_out, (void*)char1);
    if(t_1) {
        fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_1);
        exit(t_1);
    }

    t_2 = pthread_create(&consumer, NULL, message_out2, (void*)char2);
    if(t_2) {
        fprintf(stderr, "Error, pthread_create() - exit: %i\n", t_2);
        exit(t_2);
    }


    printf("pthread_create for thread 1 returns: %i\n", t_1);
    printf("pthread_create for thread 2 returns: %i\n", t_2);

    //wait for threads to complete, barrier
    pthread_join(factory, NULL);
    pthread_join(consumer, NULL);

    exit(0);
}
