/*
 -------------------------------------
 File:    q1.h
 Project: 190906250_190765210_a02_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-09
 -------------------------------------
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * 
 * Function to be executed by the child secondary thread.
 * 
 */
void *thread_function(void *args);

/**
 * 
 * Function to be executed by the main parent thread.
 * 
 */
void parent_function();

/**
 * 
 * Helper function to make error messages nicer to print.
 * 
 */
void err(char *message);