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

    int average;
    int maximum;
    int minimum;
    int median;
};

void *execute_thread(void *function_pointer, struct number_list *list);

void *calculate_average(struct number_list *list);

void *calculate_maximum(struct number_list *list);

void *calculate_minimum(struct number_list *list);

void *calculate_median(struct number_list *list);

void get_input_numbers(int count, char *array[], int target_array[]);

void err(char *message);

int main(int arg_count, char *args[]) {
    int numbers[arg_count - 1];
    get_input_numbers(arg_count, args, numbers);

    struct number_list list;
    list.length = arg_count - 1;
    list.numbers = numbers;

    list.average = 0;
    list.maximum = 0;
    list.minimum = 0;
    list.median = 0;

    execute_thread(&calculate_average, &list);
    execute_thread(calculate_maximum, &list);
    execute_thread(calculate_minimum, &list);
    execute_thread(calculate_median, &list);

    printf("The average value calculated by the first thread is %d!\n", list.average);
    printf("The maximum value calculated by the second thread is %d!\n", list.maximum);
    printf("The minimum value calculated by the third thread is %d!\n", list.minimum);
    printf("The median value calculated by the fourth thread is %d!\n", list.median);
    return 0;
}

void *execute_thread(void *function_pointer, struct number_list *list) {
    pthread_t thread_id;
    pthread_attr_t thread_attrib;
    int status;

    status = pthread_attr_init(&thread_attrib);
    if (status != 0)
        err("Main >> Error while initialising a new thread attributes object!\n");

    status = pthread_create(&thread_id, &thread_attrib, function_pointer, list);
    if (status != 0)
        err("Main >> Error while creating a new thread!\n");

    printf("Main >> Finished executing a thread with ID: %lu!\n", thread_id);
}

void *calculate_average(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;

    int total = 0;
    for (int i = 0; i < length; i++) {
        total += array[i];
    }

    list->average = total / length;
}

void *calculate_maximum(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;

    int index = 0;
    for (int i = 0; i < length; i++)
        if (array[index] < array[i])
            index = i;

    list->maximum = array[index];
}

void *calculate_minimum(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;

    int index = 0;
    for (int i = 0; i < length; i++)
        if (array[index] > array[i])
            index = i;

    list->minimum = array[index];
}

void *calculate_median(struct number_list *list) {
    int length = list->length;
    int *array = list->numbers;
    int index = 0.5 * (length + 1);

    list->median = array[index];
}

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

void err(char *message) {
    printf("%s", message);
    exit(-1);
}