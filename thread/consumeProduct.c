//
// Created by zjyan on 2025/8/16.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t full;
pthread_cond_t empty;

void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100;
        pthread_mutex_lock(&mutex);
        while (count==BUFFER_SIZE) {
            pthread_cond_wait(&full, &mutex);
        }
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        printf("Produced item: %d\n", item);
        usleep(500000); // 模拟生产时间
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count==0) {
            pthread_cond_wait(&empty, &mutex);
        }
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        printf("Consumed item: %d\n", item);
        usleep(500000); // 模拟消耗时间
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    return 0;

}
