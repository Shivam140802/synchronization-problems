#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5  
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num + 4) % N
#define RIGHT (ph_num + 1) % N

int state[N];                 
int phil_num[N] = {0, 1, 2, 3, 4};  

pthread_mutex_t mutex;         
pthread_mutex_t S[N];      

void test(int ph_num) {
    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[ph_num] = EATING;

        sleep(2); 

        printf("Philosopher %d takes fork %d and %d\n", ph_num + 1, LEFT + 1, ph_num + 1);
        printf("Philosopher %d is eating\n", ph_num + 1);

        pthread_mutex_unlock(&S[ph_num]);
    }
}

void take_fork(int ph_num) {
    pthread_mutex_lock(&mutex);  
    state[ph_num] = HUNGRY;
    printf("Philosopher %d is hungry\n", ph_num + 1);

    test(ph_num);

    pthread_mutex_unlock(&mutex); 
    pthread_mutex_lock(&S[ph_num]);
    sleep(1);
}

void put_fork(int ph_num) {
    pthread_mutex_lock(&mutex); 
    state[ph_num] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n", ph_num + 1, LEFT + 1, ph_num + 1);
    printf("Philosopher %d is thinking\n", ph_num + 1);

    test(LEFT);
    test(RIGHT);

    pthread_mutex_unlock(&mutex);
}

void* philosopher(void* num) {
    while (1) {
        int* ph_num = num;

        sleep(1);  
        take_fork(*ph_num);  
        sleep(0);  
        put_fork(*ph_num);
    }
}

int main() {
    int i;
    pthread_t thread_id[N];  
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < N; i++) {
        pthread_mutex_init(&S[i], NULL);
        state[i] = THINKING;
    }

    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil_num[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
