#include "q2.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

float average;
float max;
float min;
float median;

int cmpfunc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
    struct inputArray array;
    array.length = argc - 1;
    int nums[argc - 1];
    for (int i = 0; i < argc - 1; i++) {
        char *c;
        int number = strtol(argv[i + 1], &c, 10);
        nums[i] = number;
    }

    qsort(nums, argc - 1, sizeof(int), cmpfunc);
    array.data = nums;

    average = 0.0;
    min = 0.0;
    max = 0.0;
    median = 0.0;

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    pthread_attr_t tatt1;
    pthread_attr_t tatt2;
    pthread_attr_t tatt3;
    pthread_attr_t tatt4;

    pthread_attr_init(&tatt1);
    pthread_attr_init(&tatt2);
    pthread_attr_init(&tatt3);
    pthread_attr_init(&tatt4);

    pthread_create(&tid1, &tatt1, (void *)getAverage, &array);
    pthread_create(&tid2, &tatt2, (void *)getMax, &array);
    pthread_create(&tid3, &tatt3, (void *)getMin, &array);
    pthread_create(&tid4, &tatt4, (void *)getMedian, &array);

    printf("The average value calculated by the first thread is %.2f!\n", average);
    printf("The maximum value calculated by the first thread is %.2f!\n", max);
    printf("The minimum value calculated by the first thread is %.2f!\n", min);
    printf("The median value calculated by the first thread is %.2f!\n", median);
    return 0;
}

void *getAverage(struct inputArray *array) {
    int length = array->length;
    int *numbers = array->data;
    int count = 0;

    for (int i = 0; i < length; i++) {
        count += numbers[i];
    }
    average = count / length;
    return NULL;
}

void *getMax(struct inputArray *array) {
    int *numbers = array->data;
    int length = array->length;
    max = numbers[length - 1];
    return NULL;
}

void *getMin(struct inputArray *array) {
    int *numbers = array->data;
    min = numbers[0];
    return NULL;
}

void *getMedian(struct inputArray *array) {
    int length = array->length;
    int *numbers = array->data;

    int location = (length + 1) * 0.5;

    median = numbers[location];
    return NULL;
}
