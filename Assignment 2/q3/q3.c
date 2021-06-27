R/*
 -------------------------------------
 File:    q3.c
 Project: 190906250_190765210_a02_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-09
 -------------------------------------
 */

#include "q3.h"

// The global clock for the program.
time_t start_time;

/**
 * 
 * Main entry function.
 * 
 */
int main(int arg_count, char* args[]) {
    if (arg_count > 1) {
        char* file_name = args[1];

        // Grab threads from the file.
        Thread* threads;
        int thread_count = read_file(file_name, &threads);

        // Start the global program clock.
        service_threads(threads, thread_count);
        return 0;

    } else
        err("Input file name missing...exiting with error code -1\n");
}

/**
 * 
 * Populates an array of threads with thread information provided
 * from a file. Returns the amount of threads.
 * 
 */
int read_file(char* file_name, Thread** threads) {
    FILE* in = fopen(file_name, "r");
    if (in) {
        int thread_count = 0;
        struct stat file_attrib;
        fstat(fileno(in), &file_attrib);

        // Stores the entire contents of the file in one line.
        char* file_content = (char*)malloc(((int)file_attrib.st_size + 1) * sizeof(char));
        file_content[0] = '\0';

        // Grabs the entire file and throws it into "file_content".
        while (!feof(in)) {
            char line[100];
            if (fgets(line, 100, in) != NULL)
                strncat(file_content, line, strlen(line));
        }

        fclose(in);

        // Count how many newline characters there are to calculate thread count.
        char* file_copy = (char*)malloc((strlen(file_content) + 1) * sizeof(char));
        strcpy(file_copy, file_content);
        char* command = strtok(file_copy, "\r\n");
        while (command != NULL) {
            thread_count++;
            command = strtok(NULL, "\r\n");
        }

        *threads = (Thread*)malloc(sizeof(Thread) * thread_count);
        command = NULL;
        command = strtok(file_content, "\r\n");

        // Divides "file_content" into an array seperated by newline characters.
        int i = 0;
        char* lines[thread_count];
        while (command != NULL) {
            lines[i] = malloc(sizeof(command) * sizeof(char));
            strcpy(lines[i], command);

            i++;
            command = strtok(NULL, "\r\n");
        }

        // Initialises a new Thread struct for every entry in the array.
        Thread* thread_array = *threads;
        for (int k = 0; k < thread_count; k++) {
            char* token = strtok(lines[k], ";");
            char* tid = token;

            token = strtok(NULL, ";");
            char* start_time = token;

            token = strtok(NULL, ";");
            char* lifetime = token;

            Thread thread;
            strcpy(thread.tid, tid);
            thread.start_time = atoi(start_time);
            thread.lifetime = atoi(lifetime);
            thread.started = false;

            thread_array[k] = thread;
        }

        return thread_count;

    } else
        err("Child A: Error in opening input file...exiting with error code -1\n");
}

/**
 * 
 * Goes through every Thread in the given list, and starts
 * a new thread with the provided ID, start time, and lifetime.
 * 
 */
void service_threads(Thread* threads, int thread_count) {
    int completed_threads = 0;

    start_time = time(NULL);
    while (completed_threads < thread_count) {
        for (int i = 0; i < thread_count; i++) {
            Thread thread = threads[i];
            time_t now = time(NULL);

            if (!thread.started && get_current_time() == thread.start_time) {
                threads[i].started = true;
                completed_threads++;

                pthread_t thread_id = create_thread(&threads[i]);
                threads[i].thread_id = thread_id;
            }
        }
    }

    for (int i = 0; i < thread_count; i++)
        pthread_join(threads[i].thread_id, NULL);
}

/**
 * 
 * Creates a new thread with the provided thread information
 * and returns the thread ID.
 * 
 */
pthread_t create_thread(Thread* thread) {
    pthread_t thread_id;
    pthread_attr_t thread_attrib;
    int status;

    // Populates our new thread attribute object.
    status = pthread_attr_init(&thread_attrib);
    if (status != 0)
        err("Main >> Error while initialising a new thread attributes object!\n");

    // Creates a new thread.
    status = pthread_create(&thread_id, &thread_attrib, (void*)&thread_run, thread);
    if (status != 0)
        err("Main >> Error while creating a new thread!\n");

    return thread_id;
}

/**
 * 
 * Function ran by a thread to time itself out after
 * its lifetime has passed.
 * 
 */
void* thread_run(Thread* thread) {
    log_start(thread->tid);
    sleep(thread->lifetime);
    log_finish(thread->tid);
}

/**
 * 
 * Helper function to get the current time.
 * 
 */
long get_current_time() {
    time_t now = time(NULL);
    return abs(now - start_time);
}

/**
 * 
 * Helper function to log a message whenever a thread is finished.
 * 
 */
void log_start(char* tID) {
    printf("[%d] New Thread with ID %s is started.\n", abs(start_time - time(NULL)), tID);
}

/**
 * 
 * Helper function to log a message whenever a thread is finished.
 * 
 */
void log_finish(char* tID) {
    printf("[%d] Thread with ID %s is finished.\n", abs(start_time - time(NULL)), tID);
}

/**
 * 
 * Helper function to make error messages nicer to print.
 * 
 */
void err(char* message) {
    printf("%s", message);
    exit(-1);
}