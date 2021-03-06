/*
 -------------------------------------
 File:    process_managment.c
 Project: 190906250_190765210_a01_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-02
 -------------------------------------
 */

#include "process_managment.h"

const int ARGUMENT_POSITION = 1;

// Array indexes that represent pipe ends.
const int READ = 0;
const int WRITE = 1;

// Variables to control the name and size of the shared memory.
const int SHARED_MEMORY_SIZE = 4096;
const char *SHARED_MEMORY_FILE_NAME = "PROC_SHM";

// The file name for the output file.
const char *OUTPUT_FILE_NAME = "output_file.txt";

/**
 *
 * Main function - parent process.
 *
 */
int main(int argument_count, char *arguments[]) {
    if (argument_count > 1) {
        int memory_id = get_new_memory_id();
        char *memory_pointer = get_memory_pointer(memory_id);

        char *fileName = arguments[ARGUMENT_POSITION];
        write_file_to_memory(memory_pointer, fileName, strlen(fileName));
        exec_commands_from_memory(memory_pointer);
        close_memory(memory_pointer, memory_id);
    }

    else {
        printf("\nmain: No file given!\n");
        exit(-1);
    }
}

/**
 *
 * Creates and returns the ID of a new shared memory segment.
 *
 */
int get_new_memory_id() {
    // Create the shared memory segment as if it was a file
    int shared_mem_id =
        shm_open(SHARED_MEMORY_FILE_NAME, O_CREAT | O_RDWR, 0666);
    if (shared_mem_id == -1) {
        printf("\nget_new_memory_id: Shared memory failed: %s\n",
               strerror(errno));
        exit(-1);
    }

    // configure the size of the shared memory segment
    ftruncate(shared_mem_id, SHARED_MEMORY_SIZE);
    return shared_mem_id;
}

/**
 *
 * Returns a read/writeable pointer for a given shared memory ID.
 *
 */
char *get_memory_pointer(int shared_mem_id) {
    // map the shared memory segment to the address space of the process
    char *pointer = mmap(0, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE,
                         MAP_SHARED, shared_mem_id, 0);
    if (pointer == MAP_FAILED) {
        printf("\nget_memory_pointer: Map failed: %s\n", strerror(errno));
        exit(-1);
    }

    return pointer;
}

/**
 *
 * Spawns the child process that reads each line from the input file
 * and writes it to shared memory.
 *
 */
void write_file_to_memory(char *memory_pointer, char *fileName, int length) {
    char *pointer = memory_pointer;
    pid_t pid = fork();

    // Child process
    if (pid == 0) {
        // int shared_mem_id = get_writeable_memory_id();
        // char *memory_pointer = get_writeable_memory_pointer(shared_mem_id);

        FILE *file = fopen(fileName, "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        // Writes each line from input to shared memory.
        while ((read = getline(&line, &len, file)) != -1) {
            pointer += sprintf(pointer, "%s", line);
        }

        // close_writeable_memory(memory_pointer, shared_mem_id);
        exit(0);
    }

    // Parent process
    else if (pid > 0) {
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == -1) {
            perror("\nwrite_file_to_memory: Error while waiting\n");
            exit(-1);
        }

        printf("\nwrite_file_to_memory: Done waitin\n");
    }

    else {
        printf("\nwrite_file_to_memory: Error while forking!\n");
        exit(-1);
    }
}

/**
 * 
 * Takes the memory ID and pointer. Unmaps memory address from this process. 
 * Closes the shared memory segment, and removes the shared memory from the
 * file system.
 * 
 */
void close_memory(char *memory_pointer, int memory_id) {
    // remove the mapped memory segment from the address space of the process
    if (munmap(memory_pointer, SHARED_MEMORY_SIZE) == -1) {
        printf("\nclose_writeable_memory: Unmap failed: %s\n", strerror(errno));
        exit(-1);
    }

    // close the shared memory segment as if it was a file
    if (close(memory_id) == -1) {
        printf("\nclose_writeable_memory: Close failed: %s\n", strerror(errno));
        exit(-1);
    }

    // remove the shared memory segment from the file system
    if (shm_unlink(SHARED_MEMORY_FILE_NAME) == -1) {
        printf("\nclose_readable_memory: Error removing %s: %s\n",
               SHARED_MEMORY_FILE_NAME, strerror(errno));
        exit(-1);
    }
}

/**
 * 
 * Spawns a child process that reads each command from shared memory that
 * was written by the other child, and executes the command. The output
 * of each command is written to a file.
 * 
 */
void exec_commands_from_memory(char *memory_pointer) {
    char *pointer = memory_pointer;
    pid_t pid = fork();

    // Child process
    if (pid == 0) {
        // Loop stores entire contents of memory in one string, separated by \n
        char *result = malloc(SHARED_MEMORY_SIZE);
        for (int i = 0; i < 64; i += 1) {
            char data = (char)pointer[i];
            strncat(result, &data, 1);
        }

        // Splits the string by \n
        char *result_pointer = strtok(result, "\r\n");
        execute_commands(result_pointer);
        exit(0);
    }

    // Parent process
    else if (pid > 0) {
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == -1) {
            perror("\nexec_commands_from_memory: Error while waiting\n");
            exit(-1);
        }
    }

    else {
        printf("\nexec_commands_from_memory: Error while forking!\n");
        exit(-1);
    }
}

/**
 * 
 * Takes an array of strings, each representing a command. 
 * Each entry in the array is executed and the result is stored 
 * in a file.
 * 
 */
void execute_commands(char *pointer) {
    FILE *output_file = fopen(OUTPUT_FILE_NAME, "w");
    char *line;

    while (pointer) {
        FILE *virtual_file = popen(pointer, "r");
        char output[1035];
        if (virtual_file) {
            fprintf(output_file, "The output of: %s : is\n>>>>>>>>>>>>>>>\n", pointer);
            while (fgets(output, sizeof(output), virtual_file) != NULL) {
                fprintf(output_file, "%s", output);
            }

            fprintf(output_file, "<<<<<<<<<<<<<<<\n\n");
        } else {
            printf("\nexecute_commands: Error while executing '%s'!\n",
                   pointer);
            exit(-1);
        }

        fclose(virtual_file);
        pointer = strtok(NULL, "\r\n");
    }

    fclose(output_file);
}

/**
 *
 * UNUSED CODE BELOW PLEASE IGNORE
 *
 */

/**
 *
 * Creates a new shared memory segment that can be read from and returns its ID.
 *
 */
// int get_readable_memory_id()
// {
//     int shared_mem_id = shm_open(SHARED_MEMORY_FILE_NAME, O_RDONLY, 0666);
//     if (shared_mem_id == -1)
//     {
//         printf("\nget_readable_memory_id: Shared memory failed: %s\n",
//         strerror(errno)); exit(-1);
//     }

//     return shared_mem_id;
// }

/**
 *
 * Returns a readable pointer for a given shared memory ID.
 *
 */
// char *get_readable_memory_pointer(int shared_mem_id)
// {
//     char *memory_pointer = mmap(0, SHARED_MEMORY_SIZE, PROT_READ, MAP_SHARED,
//     shared_mem_id, 0); if (memory_pointer == MAP_FAILED)
//     {
//         printf("\nget_readable_memory_pointer: Map failed: %s\n",
//         strerror(errno)); exit(-1);
//     }

//     return memory_pointer;
// }

/**
 *
 * Closes the pointer for the writeable memory segment.
 *
 */
// void close_writeable_memory(char *memory_pointer, int memory_segment_id)
// {
//     // remove the mapped memory segment from the address space of the process
//     if (munmap(memory_pointer, SHARED_MEMORY_SIZE) == -1)
//     {
//         printf("\nclose_writeable_memory: Unmap failed: %s\n",
//         strerror(errno)); exit(-1);
//     }

//     // close the shared memory segment as if it was a file
//     if (close(memory_segment_id) == -1)
//     {
//         printf("\nclose_writeable_memory: Close failed: %s\n",
//         strerror(errno)); exit(-1);
//     }
// }

/**
 *
 * Closes the pointer for the readable memory segment.
 *
 */
// void close_readable_memory(char *memory_pointer, int memory_segment_id)
// {
//     /* close the shared memory segment as if it was a file */
//     if (close(memory_pointer) == -1)
//     {
//         printf("\nclose_readable_memory: Close failed: %s\n",
//         strerror(errno)); exit(-1);
//     }

//     /* remove the shared memory segment from the file system */
//     if (shm_unlink(SHARED_MEMORY_FILE_NAME) == -1)
//     {
//         printf("\nclose_readable_memory: Error removing %s: %s\n",
//         SHARED_MEMORY_FILE_NAME, strerror(errno)); exit(-1);
//     }
// }