/*
 -------------------------------------
 File:    Question1.c
 Project: A03Q1
 -------------------------------------
 Author:  Nausher Rao
 ID:      190906250
 Email:   raox6250@mylaurier.ca
 Version  2021-06-19
 -------------------------------------
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global Data variables.
int a = 5;
int b = 7;
pthread_mutex_t lock;

// Function that access the global data.
void* inc_dec(void* args) {
    pthread_mutex_lock(&lock);

    printf("Read value of 'a' global variable is: %d\n", a);
    printf("Read value of 'b' global variable is: %d\n", b);
    sleep(1);
    a = a + 1;
    b = b - 1;
    printf("Updated value of 'a' variable is: %d\n", a);
    printf("Updated value of 'b' variable is: %d\n", b);

    pthread_mutex_unlock(&lock);
    return 0;
}

int main() {
    // Creating the thread instances.
    pthread_t t1, t2, t3;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\nCouldn't initialise mutex!\n");
        return 1;
    }

    // Start all the threads.
    pthread_create(&t1, NULL, inc_dec, NULL);
    pthread_create(&t2, NULL, inc_dec, NULL);
    pthread_create(&t3, NULL, inc_dec, NULL);

    // Block until all threads are done.
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // Destroying the threads.
    pthread_exit((void*)t1);
    pthread_exit((void*)t2);
    pthread_exit((void*)t3);
    pthread_mutex_destroy(&lock);
    return 0;
}
