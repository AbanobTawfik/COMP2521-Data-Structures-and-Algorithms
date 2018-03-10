// queue.h ... interface to simple Queue of Strings
// Written by John Shepherd, September 2015
 
#ifndef SEARCHENGINE_QUEUE_H
#define SEARCHENGINE_QUEUE_H

typedef struct QueueRep *Queue;

// Function signatures

Queue newQueue();
void disposeQueue(Queue);
void enterQueue(Queue,char *);
char *leaveQueue(Queue);
int  emptyQueue(Queue);
void showQueue(Queue q);
int queuesize(Queue q);
int inQueue(Queue q, char*);
char **list(Queue q, char **store);

#endif //SEARCHENGINE_QUEUE_H
