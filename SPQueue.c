//
// Created by hoshri on 9/11/2017.
//

#include "SPQueue.h"

#include <stdlib.h>
#include <stdio.h>
#include "string.h"

Queue* spQueueCreate(int maxSize) {
    Queue* queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL) {
        return NULL;
    }
    if (maxSize <= 0) {
        maxSize = 10;
    }
    queue->maxSize = maxSize;
    queue->actualSize = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void spQueueDestroy(Queue* queue) {
    SPNode* trashNode;
    while ((trashNode = spQueuePop(queue)) != NULL) {
        free(trashNode);
    }
    free(queue);
}

int spQueuePush(Queue *myQ, char data[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE]) {
    /* Bad parameter */
    if ((myQ == NULL) || (data == NULL)) {
        return 0;
    }
    // if(myQ->maxSize != 0)
    if (myQ->actualSize >= myQ->maxSize) {
        return 0;
    }
    /*the queue is empty*/
    SPNode* item = (SPNode*) malloc(sizeof(SPNode));
    item->prev = NULL;
    memcpy(item->data , data , sizeof(char[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE]));
    if (myQ->actualSize == 0) {
        myQ->head = item;
        myQ->tail = item;

    } else {
        /*adding item to the end of the queue*/
        myQ->tail->prev = item;
        myQ->tail->prev->next = myQ->tail; // to impliment stack like pop
        myQ->tail = item;
    }
    myQ->actualSize++;
    return 1;
}

SPNode* spQueuePop(Queue* myQ) {
    /*the queue is empty or bad param*/
    SPNode* item;
    if (spQueueIsEmpty(myQ))
        return NULL;
    item = myQ->head;
    myQ->head = (myQ->head)->prev;
    myQ->actualSize--;
    return item;
}

SPNode* spStackPop(Queue* myQ) {
    /*the queue is empty or bad param*/
    SPNode* item;
    if (spQueueIsEmpty(myQ))
        return NULL;
    item = myQ->tail;
    myQ->tail = (myQ->tail)->next;
    myQ->actualSize--;
    return item;
}

int spQueueIsEmpty(Queue* myQ) {
    if (myQ == NULL) {
        return 0;
    }
    if (myQ->actualSize == 0) {
        return 1;
    } else {
        return 0;
    }
}

Queue* spQueueCopy(Queue* myQ){
    Queue* queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->maxSize = myQ->maxSize;
    queue->actualSize = myQ->actualSize;
    queue->head = myQ->head;
    queue->tail = myQ->tail;
    return queue;
}