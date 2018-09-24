#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include "queue.h"

struct Queue* createQueue(unsigned capacity)
{
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));	//set memory aside for the queue struct
	queue->capacity = capacity;						//set the size of the capacity
	queue->front = queue->size = 0;						//set front and size to zero for initilization
	queue->rear = capacity -1;						//set rear position becomes clear in enqueue
	queue->array = (char**) malloc(capacity * sizeof(char*));		//set memory for rows of array
	assert(queue->array != NULL);
	int i;
	for(i = 0; i <= capacity; i++)						//set memory for the columns of the array
	{	
		*(queue->array + i) = (char*) malloc(300 * sizeof(char));
		assert(*(queue->array + i) != NULL);
	}
	return queue;								
}


int isFull(struct Queue* queue)
{
	if(queue->size >= queue->capacity)
		return (1);
	else
		return (0);
}

int isEmpty(struct Queue* queue)
{
	if(queue-> size == 0)
		return (1);
	else
		return (0);
}


int enqueue(struct Queue* queue, char* string)
{
	if(isFull(queue))
	{
		printf("the queue is full and object has not been stored\n");
		return (1); 
	}
	queue->rear = (queue->rear + 1)%queue->capacity;
	int i = 0;
	strcpy(queue->array[queue->rear], string);
	queue->size = queue->size + 1;
	return (0);
}


char* dequeue(struct Queue* queue, char* string)
{
	if(isEmpty(queue))
	{
		printf("the queue is empty and no object can be retreived");
		return ('\0');
	}
	strcpy(string, queue->array[queue->front]);
	queue->front = (queue->front + 1)%queue->capacity;
	queue->size = queue->size -1;
	return;	
}

char* front(struct Queue* queue, char* string)
{
	if(isEmpty(queue))
	{
		printf("the queue is empty and no object can be retreived");
		return ('\0');
	}
	strcpy(string, queue->array[queue->front]);
	return;
}

char* rear(struct Queue* queue, char* string)
{
	if(isEmpty(queue))
	{
		printf("the queue is empty and no object can be retreived");
		return ('\0');
	}
	strcpy(string, queue->array[queue->front]);
	return;
}

