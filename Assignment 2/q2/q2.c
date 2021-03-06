/*
 -------------------------------------
 File:    q2.c
 Project: 190906250_190765210_a02_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-09
 -------------------------------------
 */

#include "q2.h"

float average;
int maximum;
int minimum;
int median;

int main(int arg_count, char *args[]) {
    int numbers[arg_count - 1];
    get_input_numbers(arg_count, args, numbers);

    struct number_list list;
    list.length = arg_count - 1;
    list.numbers = numbers;

    average = 0;
    maximum = 0;
    minimum = 0;
    median = 0;

    execute_thread(&calculate_average, &list);
    execute_thread(calculate_maximum, &list);
    execute_thread(calculate_minimum, &list);
    execute_thread(calculate_median, &list);

    printf("The average value calculated by the first thread is %.2f!\n", average);
    printf("The maximum value calculated by the second thread is %d!\n", maximum);
    printf("The minimum value calculated by the third thread is %d!\n", minimum);
    printf("The median value calculated by the fourth thread is %d!\n", median);
    return 0;
}

void *execute_thread(void *function_pointer, struct number_list *list) {
    pthread_t thread_id;
    pthread_attr_t thread_attrib;
    int status;

    // Populates our new thread attribute object.
    status = pthread_attr_init(&thread_attrib);
    if (status != 0)
        err("Main >> Error while initialising a new thread attributes object!\n");

    // Creates a new thread.
    status = pthread_create(&thread_id, &thread_attrib, function_pointer, list);
    if (status != 0)
        err("Main >> Error while creating a new thread!\n");

    printf("Main >> Finished executing a thread with ID: %lu!\n", thread_id);
}

/**
 * 
 * Calculates the average value of the given array of numbers.
 * 
 */
void *calculate_average(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;

    int total = 0;
    for (int i = 0; i < length; i++) {
        total += array[i];
    }

    average = total / length;
}

/**
 * 
 * Calculates the maximum value of the given array of numbers.
 * 
 */
void *calculate_maximum(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;

    int index = 0;
    for (int i = 0; i < length; i++)
        if (array[index] < array[i])
            index = i;

    maximum = array[index];
}

/**
 * 
 * Calculates the minimum value of the given array of numbers.
 * 
 */
void *calculate_minimum(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;

    int index = 0;
    for (int i = 0; i < length; i++)
        if (array[index] > array[i])
            index = i;

    minimum = array[index];
}

/**
 * 
 * Calculates the median value of the given array of numbers.
 * 
 */
void *calculate_median(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;
    int index = 0.5 * (length + 1);

    median = array[index - 1];
}

/**
 * 
 * Helper function that takes the command line arguments
 * and parses them as integers to be stored in an array.
 * 
 */
void get_input_numbers(int count, char *array[], int target_array[]) {
    for (int i = 0; i < count - 1; i++) {
        char *p;
        int value = strtol(array[i + 1], &p, 10);
        if (p == array[i + 1])
            err("Main >> Couldn't convert the inputted numbers!\n");

        else
            target_array[i] = value;
    }
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