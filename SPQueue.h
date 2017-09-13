//
// Created by hoshri on 9/11/2017.
//

#ifndef UNTITLED1_SPQUEUE_H
#define UNTITLED1_SPQUEUE_H

#endif //UNTITLED1_SPQUEUE_H

/* a link in the queue, holds the info and point to the next Node*/

typedef struct sp_Node_t {
    char** data;
    struct sp_Node_t* prev;
} spNODE;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
    spNODE* head;
    spNODE* tail;
    int actualSize;
    int maxSize;
} Queue;

Queue* spCreateQueue(int maxSize);
void spDestroyQueue(Queue* queue);
int pushQueue(Queue* myQ, spNODE* item);
spNODE* popQueue(Queue* myQ);
int isEmpty(Queue* myQ);