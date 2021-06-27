/*
 -------------------------------------
 File:    process_managment.h
 Project: 190906250_190765210_a01_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-02
 -------------------------------------
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Stores the index of the virtual argument that contains the input file name.
const int ARGUMENT_POSITION = 1;

// Variables to control the size of the shared memory.
const int SHARED_MEMORY_SIZE = 4096;

// Variables to control the name of the shared memory.
const char *SHARED_MEMORY_FILE_NAME = "PROC_SHM";

// The file name for the output file.
const char *OUTPUT_FILE_NAME = "output.txt";

/**
 * 
 * Main function controlled by the parent process.
 * 
 */
int main(int argc, char *argv[]);

/**
 *
 * Creates and returns the ID of a new shared memory segment.
 *
 */
int get_new_memory_id();

/**
 *
 * Returns a read/writeable pointer for a given shared memory ID.
 *
 */
char *get_memory_pointer(int memory_id);

/**
 *
 * Spawns the child process that reads each line from the input file
 * and writes it to shared memory.
 *
 */
void write_file_to_memory(char *memory_pointer, char *fileName, int length);

/**
 * 
 * Spawns a child process that reads each command from shared memory that
 * was written by the other child, and executes the command. The output
 * of each command is written to a file.
 * 
 */
void exec_commands_from_memory(char *memory_pointer);

/**
 * 
 * Takes an array of strings, each representing a command. 
 * Each entry in the array is executed and the result is stored 
 * in a file.
 * 
 */
void write_command_output_to_pipe(char *pointer, int pipe_id);

/**
 * 
 * Takes the bytes currently in the FIFO named pipe and writes it 
 * to a file.
 * 
 */
void pipe_to_file(int pipe_id);

/**
 * 
 * Takes the memory ID and pointer. Unmaps memory address from this process. 
 * Closes the shared memory segment, and removes the shared memory from the
 * file system.
 * 
 */
void close_memory(char *memory_pointer, int memory_id);

/**
 * 
 * Helper function for replacing a character in a string with another string.
 * 
 */
char *replace_char(char *str, char find, char replace);
