#ifndef UNTITLED1_SPQUEUE_H
#define UNTITLED1_SPQUEUE_H

#define SP_GAMEBOARD_SIZE 8

/* a link in the queue, holds the info and point to the next Node*/
typedef struct sp_Node_t {
    char data[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE];
    char msg[sizeof("Undo move for player black : <x,y> -> <w,z>\nUndo move for player black : <x,y> -> <w,z>\n")];
    struct sp_Node_t* prev;
    struct sp_Node_t* next;
} SPNode;

/* the struct of the Queue, holds the queue's head , tail , actual-size and the maximum size available*/
typedef struct Queue {
    SPNode* head;
    SPNode* tail;
    int actualSize;
    int maxSize;
} Queue;


/**
 * Creates a new queue with maxSize as given.
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new Queue instant is returned.
 */
Queue* spQueueCreate(int maxSize);

/**
 * free the memory allocated by the queue given
 */
void spQueueDestroy(Queue* queue);

/**
 * adds the data given to the end of the Queue
 *
 * @return
 * 0 if either a memory allocation failure or a bad input.
 * Otherwise, on success, returns 1.
 */
int spQueuePush(Queue *myQ, char data[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE]);

/**
 * adds the given data with its massage to the end of the Queue
 *
 * @return
 * 0 if either a memory allocation failure or a bad input.
 * Otherwise, on success, returns 1.
 */
int spQueueFullPush(Queue *myQ, char data[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], char* msg);

/**
 * removes the head of the queue given
 *
 * @return
 * 0 if a bad input given.
 * Otherwise, on success, returns 1.
 */
SPNode* spQueuePop(Queue* myQ);

/**
 * removes the tail of the queue given
 *
 * @return
 * 0 if a bad input given.
 * Otherwise, on success, returns 1.
 */
SPNode* spStackPop(Queue* myQ);

/**
 * returns 0 or 1 according to if the queue is empty or not
 *
 * @return
 * 0 if a bad input given or the queue is not empty.
 * 1 if the queue is empty.
 */
int spQueueIsEmpty(Queue* myQ);

/**
 * copies the queue given to a new location in memory
 * and returns a pointer to the head of the copied queue
 *
 * @return
 * Empty Queue with maxSize = 3 if the queue given is either empty or NULL.
 * NULL if a memory allocation failure.
 * Otherwise, on success, returns a copied queue.
 */
Queue* spQueueCopy(Queue* myQ);

#endif //UNTITLED1_SPQUEUE_H