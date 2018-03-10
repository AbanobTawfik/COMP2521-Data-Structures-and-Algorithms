// queue.c ... simple Queue of Strings
// Written by John Shepherd, September 2015

/*
 * added in our own functions
 * we added list size as a Queuerep element
 * we added the functions to, store an array of strings into the queue, check if a string is in the queue, and return list size of queue
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "helper.h"
#include "queue.h"

typedef struct Node *Link;

typedef struct Node {
	char *val;
	Link  next;
} Node;

typedef struct QueueRep {
	Link  front;
	Link  back;
	int size;
} QueueRep;

// Function signatures

Queue newQueue();
void disposeQueue(Queue);
void enterQueue(Queue,char *);
char *leaveQueue(Queue);
int  emptyQueue(Queue);
void showQueue(Queue q);

static Link newNode(char *);
static void disposeNode(Link);


// newQueue()
// - create an initially empty Queue
Queue newQueue()
{
	Queue new = malloc(sizeof(QueueRep));
	assert(new != NULL);
	new->front = NULL;
	new->back = NULL;
	new->size = 0;
	return new;
}

// disposeQueue(Queue)
// - clean up memory associated with Queue
void disposeQueue(Queue q)
{
	if (q == NULL) return;
	Link next, curr = q->front;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);
		curr = next;
	}
	q->size = 0;
}

// enterQueue(Queue,Str)
// - add Str to back of Queue
void enterQueue(Queue q, char *str)
{
	Link new = newNode(str);
	if (q->front == NULL) {
		q->front = q->back = new;
		q->size++;
	}
	else {
		// add into list of elems
		q->back->next = new;
		q->back = new;
		q->size++;
	}
}

// leaveQueue(Queue)
// - return string at front of Queue
char *leaveQueue(Queue q)
{
	assert (q->front != NULL);
	char *str = q->front->val;
	Link old = q->front;
	q->front = old->next;
	q->size--;
	if (q->front == NULL) {
		q->back = NULL;
		q->size--;
	}
	free(old);
	return str;
}

// emptyQueue(Queue)
// - check whether Queue is empty
int emptyQueue(Queue q)
{
	return (q->front == NULL);
}

// showQueue(Queue)
// - display Queue (for debugging)
void showQueue(Queue q)
{
	Link curr;
	if (q->front == NULL)
		printf("Queue is empty\n");
	else {
		printf("Queue (front-to-back):\n");
		int id = 0;
		curr = q->front;
		while (curr != NULL) {
			printf("[%03d] %s\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode(char *str)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = str;
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr->val);
	free(curr);
}

/*
 * my own added functions to make this assignment significantlly easier
 */

/*
 * returns the size of the queue or number of nodes/words in the queue
 */
int queuesize(Queue q){
	return q->size;
}

/*
 * checks if a given string is inside the queue
 * boolean type true and false statement
 */
int inQueue(Queue q, char *check){
	Link curr = q->front;
	while(curr!=NULL){
		if(strcmp(curr->val,check)==0){			//scan through the queue
			return 1;							//if the string you are checking for is in the queue , return 1
		}
		curr = curr->next;
	}
	return 0;									//else return 0
}

//sthis stores the queue into a string array where the string array is unique i.e no duplicates
char **list(Queue q, char **store){
	Link curr = q->front;
	Queue seen = newQueue();					//seen queue to keep track of seen elements
	int i = 0;
	char *temp;									//buffer
	while(curr!=NULL){
		if(!inQueue(seen,curr->val)){			//if the element isn't in the seen queue, store it into the return string array
			store[i] = curr->val;
			i++;
		}
		temp = strdup(curr->val);				//add the word to our seen queue
		enterQueue(seen,temp);
		curr = curr->next;
	}
	disposeQueue(seen);							//free memory
	free(seen);
	return store;
}