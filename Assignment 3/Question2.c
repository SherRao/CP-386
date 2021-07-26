/*
 -------------------------------------
 File:    q2.c
 Project: A03Q2
 -------------------------------------
 Author:  Nausher Rao
 ID:      190906250
 Email:   raox6250@mylaurier.ca
 Version  2021-06-19
 -------------------------------------
 */

#include "Question2.h"

/**
 * 
 * Main entry function.
 * 
 */
int main(int arg_count, char *args[]) {
    if (arg_count < 2)
        err("Input file name missing...exiting with error code -1\n");

    sem_init(&running, 0, 1);
    sem_init(&even, 0, 0);
    sem_init(&odd, 0, 0);

    Thread *threads = NULL;
    int thread_count = read_file(args[1], &threads);
    program_clock = time(NULL);

    while (threads_left(threads, thread_count) > 0) {
        int i = 0;
        while ((i = get_next_thread_to_start(threads, thread_count)) > -1) {
            // Allows the first thread to go first - this sets the semaphores for the rest.
            if (run_first_thread) {
                run_first_thread = false;
                if (threads[i].tid[2] % 2 == 0)
                    sem_post(&even);

                else
                    sem_post(&odd);
            }
            // ---------------------------------->

            threads[i].state = 1;
            threads[i].retVal = pthread_create(&(threads[i].handle), NULL, thread_run, &threads[i]);
        }

        // ---------------------------------->
        if (!threads_started) {
            int ready = true;
            for (int i = 0; i < thread_count; i++)
                if (threads[i].state == 0)
                    ready = false;

            if (ready)
                threads_started = true;
        }
        // ---------------------------------->
    }

    sem_destroy(&running);
    sem_destroy(&even);
    sem_destroy(&odd);
    return 0;
}

/**
 * 
 * Populates an array of threads with thread information provided
 * from a file. Returns the amount of threads.
 * 
 */
int read_file(char *fileName, Thread **threads) {
    FILE *in = fopen(fileName, "r");
    if (!in) {
        printf(
            "Child A: Error in opening input file...exiting with error code -1\n");
        return -1;
    }

    struct stat st;
    fstat(fileno(in), &st);
    char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
    fileContent[0] = '\0';
    while (!feof(in)) {
        char line[100];
        if (fgets(line, 100, in) != NULL) {
            strncat(fileContent, line, strlen(line));
        }
    }
    fclose(in);

    char *command = NULL;
    int threadCount = 0;
    char *fileCopy = (char *)malloc((strlen(fileContent) + 1) * sizeof(char));
    strcpy(fileCopy, fileContent);
    command = strtok(fileCopy, "\r\n");
    while (command != NULL) {
        threadCount++;
        command = strtok(NULL, "\r\n");
    }
    *threads = (Thread *)malloc(sizeof(Thread) * threadCount);

    char *lines[threadCount];
    command = NULL;
    int i = 0;
    command = strtok(fileContent, "\r\n");
    while (command != NULL) {
        lines[i] = malloc(sizeof(command) * sizeof(char));
        strcpy(lines[i], command);
        i++;
        command = strtok(NULL, "\r\n");
    }

    for (int k = 0; k < threadCount; k++) {
        char *token = NULL;
        int j = 0;
        token = strtok(lines[k], ";");
        while (token != NULL) {
            //if you have extended the Thread struct then here
            //you can do initialization of those additional members
            //or any other action on the Thread members
            (*threads)[k].state = 0;
            if (j == 0)
                strcpy((*threads)[k].tid, token);
            if (j == 1)
                (*threads)[k].start_time = atoi(token);
            j++;
            token = strtok(NULL, ";");
        }
    }

    return threadCount;
}

/**
 * 
 * Returns the amount of threads that haven't been executed yet. 
 * 
 */
int threads_left(Thread *threads, int threadCount) {
    int remainingThreads = 0;
    for (int k = 0; k < threadCount; k++) {
        if (threads[k].state > -1)
            remainingThreads++;
    }

    return remainingThreads;
}

/**
 *
 * Returns the index of the next thread that should be started. 
 * 
 */
int get_next_thread_to_start(Thread *threads, int threadCount) {
    for (int k = 0; k < threadCount; k++) {
        if (threads[k].state == 0 && threads[k].start_time == get_current_time())
            return k;
    }

    return -1;
}

/**
 * 
 * Function that should be executed synchronously by each thread.
 * 
 */
void *thread_run(void *t) {
    Thread *thread = (Thread *)t;
    log_start(((Thread *)t)->tid);

    /// Start critical section lock
    if (!threads_started) {
        if (thread->tid[2] % 2)
            sem_wait(&odd);

        else
            sem_wait(&even);

        sem_wait(&running);
    }
    // ---------------------------------->

    printf("[%ld] Thread %s is in its wcritical section\n", get_current_time(), ((Thread *)t)->tid);

    /// Release critical section lock
    if (!threads_started) {
        if (thread->tid[2] % 2)
            sem_post(&even);

        else
            sem_post(&odd);

        sem_post(&running);

    } else {
        int value;
        sem_getvalue(&even, &value);
        if (value == 0)
            sem_post(&even);

        sem_getvalue(&odd, &value);
        if (value == 0)
            sem_post(&odd);

        sem_post(&running);
    }
    // ---------------------------------->

    log_finish(((Thread *)t)->tid);
    ((Thread *)t)->state = -1;
    pthread_exit(0);
}

/**
 * 
 * Helper function to get the current time.
 * 
 */
long get_current_time() {
    time_t now;
    now = time(NULL);

    return now - program_clock;
}

/**
 * 
 * Helper function to log a message whenever a thread is finished.
 * 
 */
void log_start(char *tID) {
    printf("[%ld] New Thread with ID %s is started.\n", get_current_time(), tID);
}

/**
 * 
 * Helper function to log a message whenever a thread is finished.
 * 
 */
void log_finish(char *tID) {
    printf("[%ld] Thread with ID %s is finished.\n", get_current_time(), tID);
}

/**
 * 
 * Helper function to make error messages nicer to print.
 * 
 */
void err(char *message) {
    printf("%s", message);
    exit(-1);
}