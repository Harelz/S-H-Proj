//
// Created by hoshri on 9/11/2017.
//

#include "SPQueue.h"

#include <stdlib.h>
#include <stdio.h>


Queue* spCreateQueue(int maxSize) {
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

void spDestroyQueue(Queue* queue) {
    spNODE* trashNode;
    while (!isEmpty(queue)) {
        trashNode = popQueue(queue);
        free(trashNode);
    }
    free(queue);
}

int pushQueue(Queue* myQ, spNODE* item) {
    /* Bad parameter */
    if ((myQ == NULL) || (item == NULL)) {
        return 0;
    }
    // if(myQ->maxSize != 0)
    if (myQ->actualSize >= myQ->maxSize) {
        return 0;
    }
    /*the queue is empty*/
    item->prev = NULL;
    if (myQ->actualSize == 0) {
        myQ->head = item;
        myQ->tail = item;

    } else {
        /*adding item to the end of the queue*/
        myQ->tail->prev = item;
        myQ->tail = item;
    }
    myQ->actualSize++;
    return 1;
}

spNODE* popQueue(Queue* myQ) {
    /*the queue is empty or bad param*/
    spNODE* item;
    if (isEmpty(myQ))
        return NULL;
    item = myQ->head;
    myQ->head = (myQ->head)->prev;
    myQ->actualSize--;
    return item;
}

int isEmpty(Queue* myQ) {
    if (myQ == NULL) {
        return 0;
    }
    if (myQ->actualSize == 0) {
        return 1;
    } else {
        return 0;
    }
}