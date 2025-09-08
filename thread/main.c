//
// Created by zjyan on 2025/8/16.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 5

void *thread_function(void *arg) {
    int* num=(int*)arg;
    printf("Process thread function %d\n",*num);
    *num+=1;
    return num;
}

void *printThreadNumber(void *threadId) {
    long tid=(long)threadId;
    sleep(tid);
    printf("Thread number %ld\n",tid);
    pthread_exit(NULL);
}

int main0() {
    pthread_t threads[NUM_THREADS];
    int rc;
    for (long t = 0; t < NUM_THREADS; t++) {
        printf("In main: Creating thread %ld\n",t);
        rc=pthread_create(&threads[t], NULL, printThreadNumber, (void*)t);
        if (rc) {
            printf("Error creating thread %ld\n",t);
            return -1;
        }
    }

    // 等待所有线程完成
    for (long t = 0; t < NUM_THREADS; t++) {
        rc=pthread_join(threads[t], NULL);
        if (rc) {
            printf("Error joining thread %ld\n",t);
            return -1;
        }
        printf("Thread %ld\n",t);
    }

    printf("All threads exiting\n");
    return 0;
}

int sharedVariable = 0;
pthread_mutex_t mutex;
pthread_cond_t conditionVariable;

void* incrementThread(void* arg) {
    for(int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        sharedVariable++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* printThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (sharedVariable>5) {
            printf("Shared variable is greater than 5: %d\n",sharedVariable);
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main2() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&thread1, NULL, incrementThread, NULL);
    pthread_create(&thread2, NULL, printThread, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void* producerThread(void* arg) {
    for(int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        sharedVariable++;
        pthread_cond_signal(&conditionVariable);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* consumerThread(void* arg) {
    pthread_mutex_lock(&mutex);
    while (sharedVariable==0) {
        pthread_cond_wait(&conditionVariable, &mutex);
    }
    printf("Consumed: %d\n", sharedVariable);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t  producer, consumer;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conditionVariable, NULL);
    pthread_create(&producer, NULL, producerThread, NULL);
    pthread_create(&consumer, NULL, consumerThread, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditionVariable);
    return 0;
}