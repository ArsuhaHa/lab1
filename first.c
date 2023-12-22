#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

int ready = 0;


void producer_up() {
    pthread_mutex_lock(&g_mutex);
    if (ready == 1) {
        pthread_mutex_unlock(&g_mutex);
        return;
    }

    ready = 1;
    printf("producer_up\n");
    pthread_cond_signal(&g_cond);
    pthread_mutex_unlock(&g_mutex);
}


void consumer_up() {
    pthread_mutex_lock(&g_mutex);
    while (ready == 0)
    {
        pthread_cond_wait(&g_cond, &g_mutex);
        printf("Awoke\n");
    }

    ready = 0;
    printf("consumer_up\n");
    pthread_mutex_unlock(&g_mutex);
}


void* producer() {
    while (1) {
        producer_up();
    }
    return NULL;
}


void* consumer() {
    while (1) {
        consumer_up();
    }
    return NULL;
}


int main() {
    pthread_t producer_thread, consumer_thread;

    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        perror("Failed to create producer thread");
        return -1;
    }

    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        perror("Failed to create consumer thread");
        return -2;
    }

    return 0;
}