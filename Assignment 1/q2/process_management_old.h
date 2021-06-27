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

int main(int argc, char *argv[]);

// Functions related to controlling memory.

int get_new_memory_id();

char *get_memory_pointer(int memory_id);

void close_memory(char *memory_pointer, int memory_id);

// Main functions for the program that spawn child processes.

void write_file_to_memory(char *memory_pointer, char *fileName, int length);

void exec_commands_from_memory(char *memory_pointer);

void execute_commands(char *pointer);