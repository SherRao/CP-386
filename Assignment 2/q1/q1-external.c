#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void *thread_function(void *args);
void parent_function();
void err(char *message);

int main() {
    pthread_t thread_id;
    pthread_attr_t thread_attrib;
    int status;

    status = pthread_attr_init(&thread_attrib);
    if (status != 0)
        err("Main >> Error while initialising a new thread attributes object!\n");

    printf("Main >> Initialised a new thread attributes object!\n");

    status = pthread_create(&thread_id, &thread_attrib, thread_function, NULL);
    if (status != 0)
        err("Main >> Error while creating a new thread!\n");

    printf("Main >> Created a new thread!\n");
    parent_function();
    return 0;
}

void *thread_function(void *args) {
    for (int i = 0; i < 5; i++) {
        printf("I am a Custom Thread Function Created By user.\n");
        sleep(1);
    }
}

void parent_function() {
    for (int i = 0; i < 5; i++) {
        printf("I am the process thread created by the compiler by default.\n");
        sleep(1);
    }
}

void err(char *message) {
    printf(message);
    exit(-1);
}