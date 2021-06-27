
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

struct inputArray {
	int length;
	int *data;
};

void *getAverage(struct inputArray *array);
void *getMin(struct inputArray *array);
void *getMax(struct inputArray *array);
void *getMedian(struct inputArray *array);

