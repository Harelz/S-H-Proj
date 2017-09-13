//
// Created by hoshri on 9/11/2017.
//

#ifndef UNTITLED1_SPQUEUE_H
#define UNTITLED1_SPQUEUE_H

#endif //UNTITLED1_SPQUEUE_H

#define SP_GAMEBOARD_SIZE 8

/* a link in the queue, holds the info and point to the next Node*/

typedef struct sp_Node_t {
    char data[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE];
    struct sp_Node_t* prev;
} SPNode;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
    SPNode* head;
    SPNode* tail;
    int actualSize;
    int maxSize;
} Queue;

Queue* spQueueCreate(int maxSize);
void spQueueDestroy(Queue* queue);
int spQueuePush(Queue *myQ, char data[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE]);
SPNode* spQueuePop(Queue* myQ);
int spQueueIsEmpty(Queue* myQ);