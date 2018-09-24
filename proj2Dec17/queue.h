
#ifndef QUEUE_H
#define SORTER_H

//#include <stdio.h>
//#include <stdlib.h>
//#include <limits.h>
//#define SORTER_H

struct Queue
{
	int front, rear, size;
	unsigned capacity;
	char** array;
};

struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
int enqueue(struct Queue* queue, char* string);
char* dequeue(struct Queue* queue, char* string);
char* front(struct Queue* queue, char* string);
char* rear(struct Queue* queue, char* string);

#endif


