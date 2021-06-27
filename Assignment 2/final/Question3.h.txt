/*
 -------------------------------------
 File:    q3.h
 Project: 190906250_190765210_a02_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-09
 -------------------------------------
 */
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/**
 * 
 * Strucuture that represents a single thread.
 * 
 */
typedef struct thread {
    char tid[4];     // 4 digit ID
    int start_time;  // Seconds
    int lifetime;    // Seconds
    bool started;    // Has this thread been started?

    pthread_t thread_id;  // The actual Thread ID

} Thread;

/**
 * 
 * Main entry function.
 * 
 */
int main(int arg_count, char* args[]);

/**
 * 
 * Populates an array of threads with thread information provided
 * from a file. Returns the amount of threads.
 * 
 */
int read_file(char* file_name, Thread** threads);

/**
 * 
 * Goes through every Thread in the given list, and starts
 * a new thread with the provided ID, start time, and lifetime.
 * 
 */
void service_threads(Thread* threads, int thread_count);

/**
 * 
 * Creates a new thread with the provided thread information
 * and returns the thread ID.
 * 
 */
pthread_t create_thread(Thread* thread);

/**
 * 
 * Function ran by a thread to time itself out after
 * its lifetime has passed.
 * 
 */
void* thread_run(Thread* thread);

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
void log_start(char* tID);

/**
 * 
 * Helper function to log a message whenever a thread is finished.
 * 
 */
void log_finish(char* tID);

/**
 * 
 * Helper function to make error messages nicer to print.
 * 
 */
void err(char* message);