#include "pse.h"
#include <stdio.h>
 
#define n_phil 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + n_phil - 1) % n_phil
#define RIGHT (phnum + 1) % n_phil
 
int state[n_phil];
int phil[n_phil];


sem_t mutex;
sem_t S[n_phil];
 

void test(int phnum)
{
    if (state[phnum] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {
        // state that eating
        state[phnum] = EATING;
 
        usleep(2);
 
        	//printf("Philosopher %d takes fork %d and %d\n",
                      //phnum + 1, LEFT + 1, phnum + 1);
 
        printf("Philosopher %d is Eating\n", phnum + 1);
 
        // sem_post(&S[phnum]) has no effect
        // during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&S[phnum]);
    }
}
 
// take up chopsticks
void take_fork(int phnum)
{
 
    sem_wait(&mutex);
 
    // state that hungry
    state[phnum] = HUNGRY;
 
    printf("Philosopher %d is Hungry\n", phnum + 1);
 
    // eat if neighbours are not eating
    test(phnum);
 
    sem_post(&mutex);
 
    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);
 
    usleep(1);
}
 
// put down chopsticks
void put_fork(int phnum)
{
 
    sem_wait(&mutex);
 
    // state that thinking
    state[phnum] = THINKING;
 
    	//printf("Philosopher %d putting fork %d and %d down\n",
           //phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);
 
    test(LEFT);
    test(RIGHT);
 
    sem_post(&mutex);
}
 
void* philosopher(void* num)
{
 
    while (1) {
 
        int* i = num;
 
        usleep(1);
 
        take_fork(*i);
 
        usleep(0);
 
        put_fork(*i);
    }
}
 
int main(int argc, char *argv[])
{
 
    int i;
    //if(argc==1) n_phil = argv[0];
    for(int indice = 0; indice < n_phil; indice++)  phil[indice] = indice;
    pthread_t thread_id[n_phil];
 
    // initialize the semaphores
    sem_init(&mutex, 0, 1);


    for (i = 0; i < n_phil; i++)
 
        sem_init(&S[i], 0, 0);
 
    for (i = 0; i < n_phil; i++) {
 
        // create philosopher processes
        pthread_create(&thread_id[i], NULL,
                       philosopher, &phil[i]);
 
        	//printf("Philosopher %d is thinking\n", i + 1);
    }
 
    for (i = 0; i < n_phil; i++)
 
        pthread_join(thread_id[i], NULL);
}














