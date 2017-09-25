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
    char msg[sizeof("Undo move for player black : <x,y> -> <w,z>\nUndo move for player black : <x,y> -> <w,z>\n")];
    struct sp_Node_t* prev;
    struct sp_Node_t* next;
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
int spQueueFullPush(Queue *myQ, char data[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], char* msg);
SPNode* spQueuePop(Queue* myQ);
SPNode* spStackPop(Queue* myQ);
int spQueueIsEmpty(Queue* myQ);
Queue* spQueueCopy(Queue* myQ);