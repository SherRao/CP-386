/*
 -------------------------------------
 File:    q2.h
 Project: A03Q2
 -------------------------------------
 Author:  Nausher Rao
 ID:      190906250
 Email:   raox6250@mylaurier.ca
 Version  2021-06-19
 -------------------------------------
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef struct thread {
    char tid[4];  //ID of the thread as read from file
    unsigned int start_time;
    int state;
    pthread_t handle;
    int retVal;

} Thread;

bool run_first_thread = true;
bool threads_started = false;

time_t program_clock;

pthread_mutex_t mutex;
sem_t running;
sem_t even;
sem_t odd;

/**
 * 
 * Main entry function.
 * 
 */
int main(int arg_count, char *args[]);

/**
 * 
 * Populates an array of threads with thread information provided
 * from a file. Returns the amount of threads.
 * 
 */
int read_file(char *fileName, Thread **threads);

/**
 * 
 * Returns the amount of threads that haven't been executed yet. 
 * 
 */
int threads_left(Thread *threads, int threadCount);

/**
 *
 * Returns the index of the next thread that should be started. 
 * 
 */
int get_next_thread_to_start(Thread *threads, int threadCount);

/**
 * 
 * Function that should be executed synchronously by each thread.
 * 
 */
void *thread_run(void *t);

/**
 * 
 * Helper function to get the current time.
 * 
 */
long get_current_time();

/**
 * 
 * Helper function to log a message whenever a thread is finished.
 * 
 */
void log_start(char *tID);

/**
 * 
 * Helper function to log a message whenever a thread is finished.
 * 
 */
void log_finish(char *tID);

/**
 * 
 * Helper function to make error messages nicer to print.
 * 
 */
void err(char *message);