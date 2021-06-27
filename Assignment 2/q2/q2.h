/*
 -------------------------------------
 File:    q2.h
 Project: 190906250_190765210_a02_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-09
 -------------------------------------
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct number_list {
    int length;
    int *numbers;
};

void *execute_thread(void *function_pointer, struct number_list *list);

/**
 * 
 * Calculates the average value of the given array of numbers.
 * 
 */
void *calculate_average(struct number_list *list);
/**
 * 
 * Calculates the maximum value of the given array of numbers.
 * 
 */
void *calculate_maximum(struct number_list *list);

/**
 * 
 * Calculates the minimum value of the given array of numbers.
 * 
 */
void *calculate_minimum(struct number_list *list);

/**
 * 
 * Calculates the median value of the given array of numbers.
 * 
 */
void *calculate_median(struct number_list *list);

/**
 * 
 * Helper function that takes the command line arguments
 * and parses them as integers to be stored in an array.
 * 
 */
void get_input_numbers(int count, char *array[], int target_array[]);

/**
 * 
 * Helper function to make error messages nicer to print.
 * 
 */
void err(char *message);