/*
 -------------------------------------
 File:    q1.c
 Project: 190906250_190765210_a02_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-09
 -------------------------------------
 */

#include "q1.h"

/**
 * 
 * Main function.
 * 
 */
int main() {
    pthread_t thread_id;
    pthread_attr_t thread_attrib;
    int status;

    // Populates our new thread attribute object.
    status = pthread_attr_init(&thread_attrib);
    if (status != 0)
        err("Main >> Error while initialising a new thread attributes object!\n");

    printf("Main >> Initialised a new thread attributes object!\n");

    // Creates a new thread.
    status = pthread_create(&thread_id, &thread_attrib, thread_function, NULL);
    if (status != 0)
        err("Main >> Error while creating a new thread!\n");

    printf("Main >> Created a new thread!\n");
    parent_function();
    return 0;
}

/**
 * 
 * Function to be executed by the child secondary thread.
 * 
 */
void *thread_function(void *args) {
    for (int i = 0; i < 5; i++) {
        printf("I am a Custom Thread Function Created By user.\n");
        sleep(1);
    }
}

/**
 * 
 * Function to be executed by the main parent thread.
 * 
 */
void parent_function() {
    for (int i = 0; i < 5; i++) {
        printf("I am the process thread created by the compiler by default.\n");
        sleep(1);
    }
}

/**
 * 
 * Helper function to make error messages nicer to print.
 * 
 */
void err(char *message) {
    printf(message);
    exit(-1);
}