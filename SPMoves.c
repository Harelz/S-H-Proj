//
// Created by hoshri on 9/11/2017.
//

#include "SPMoves.h"

SPTile* spCreateTile(int row , int col){
    SPTile* instance = (SPTile *) malloc(sizeof(SPTile));
    if(instance == NULL)
        return NULL;
    instance -> row = row;
    instance ->coloumn = col;
    return instance;
}

SPMove* spCreateMove(int srcRow , int srcCol ,int desRow , int desCol){
    SPMove* instance = (SPMove *) malloc(sizeof(SPMove));
    if(instance == NULL)
        return NULL;
    instance -> src = spCreateTile(srcRow,srcCol);
    instance -> dest = spCreateTile(desRow,desCol);
    return instance;
}

int spDestroyMove(SPMove* move){
    if (move != NULL){
        if (move->dest != NULL)
            free(move->dest);
        if (move->src != NULL)
            free(move->src);
    }
    free(move);
    return 1;
}

SPMove* spCreateMoveFromTile(SPTile* srcTile, SPTile* destTile){
    SPMove* instance = (SPMove *) malloc(sizeof(SPMove));
    if(instance == NULL)
        return NULL;
    instance -> src = srcTile;
    instance -> dest = destTile;
    return instance;
}

SPMovesList* spMovesListCreate(int maxSize) {
    if (maxSize <= 0)
        return NULL;
    SPMovesList* instance = (SPMovesList *) malloc(sizeof(SPMovesList));
    if (instance == NULL) return NULL;
    instance -> maxSize = maxSize;
    instance -> actualSize = 0;
    instance -> moves = (SPMove**) malloc(maxSize * sizeof(SPMove*));
    if (instance->moves == NULL) {
        free(instance);
        return NULL;
    }
    return instance;
}


SPMovesList* spMovesListCopy(SPMovesList* src) {
    if (src == NULL) return NULL;
    SPMovesList* cpy = spMovesListCreate(src->maxSize);
    if (cpy == NULL) return NULL;
    for(int i = 0; i < src->maxSize; i++)
        cpy->moves[i] = src->moves[i];
    cpy->maxSize = src->maxSize;
    cpy->actualSize = src->actualSize;
    return cpy;
}

void spMovesListDestroy(SPMovesList* src) {
    if(src != NULL){
        free(src->moves);
        free(src);
    }
}


SP_MOVES_LIST_MESSAGE spMovesListClear(SPMovesList* src) {
    if (src == NULL)
        return SP_MOVES_LIST_INVALID_ARGUMENT;
    src ->actualSize = 0;
    return SP_MOVES_LIST_SUCCESS;
}


SP_MOVES_LIST_MESSAGE spMovesListAddAt(SPMovesList* src, SPMove* move, int index) {
    if (src == NULL || src->actualSize < index || index < 0)
        return SP_MOVES_LIST_INVALID_ARGUMENT;
    if (src->actualSize >= src->maxSize)
        return SP_MOVES_LIST_FULL;
    for (int i = src ->actualSize; i > index; i--)
        src->moves[i] = src->moves[i - 1];
    src -> actualSize++;
    src->moves[index] = move;
    return SP_MOVES_LIST_SUCCESS;
}


SP_MOVES_LIST_MESSAGE spMovesListAddFirst(SPMovesList* src, SPMove* move) {
    if(src == NULL) return SP_MOVES_LIST_INVALID_ARGUMENT;
    return spMovesListAddAt(src, move, 0);
}

SP_MOVES_LIST_MESSAGE spMovesListAddLast(SPMovesList* src, SPMove* move) {
    if(src == NULL) return SP_MOVES_LIST_INVALID_ARGUMENT;
    return spMovesListAddAt(src, move, src->actualSize);
}

SP_MOVES_LIST_MESSAGE spMovesListRemoveAt(SPMovesList* src, int index) {
    if (src == NULL || src->actualSize < index || index < 0)
        return SP_MOVES_LIST_INVALID_ARGUMENT;
    if(spMovesListIsEmpty(src))
        return SP_MOVES_LIST_EMPTY;
    src -> actualSize--;
    for (int i = index; i < src ->actualSize; ++i)
        src->moves[i] = src->moves[i + 1];
    return SP_MOVES_LIST_SUCCESS;
}

SP_MOVES_LIST_MESSAGE spMovesListRemoveFirst(SPMovesList* src) {
    if(src == NULL) return SP_MOVES_LIST_INVALID_ARGUMENT;
    return spMovesListRemoveAt(src, 0);
}

SP_MOVES_LIST_MESSAGE spMovesListRemoveLast(SPMovesList* src) {
    if(src == NULL) return SP_MOVES_LIST_INVALID_ARGUMENT;
    return spMovesListRemoveAt(src, src->actualSize);
}


SPMove* spMovesListGetAt(SPMovesList* src, int index) {
    if (src == NULL || src->actualSize <= index || src->actualSize == 0) {
        return NULL;
    }
    return src->moves[index];
}

SPMove* spMovesListGetFirst(SPMovesList* src) {
    return spMovesListGetAt(src, 0);
}

SPMove* spMovesListGetLast(SPMovesList* src) {
    return spMovesListGetAt(src, src->actualSize - 1);
}

int spMovesListMaxCapacity(SPMovesList* src) {
    if (src == NULL) {
        return 0;
    }
    return src->maxSize;
}

int spMovesListSize(SPMovesList* src) {
    if (src == NULL) {
        return 0;
    }
    return src->actualSize;
}

bool spMovesListIsFull(SPMovesList* src) {
    return !(src == NULL || src->maxSize > src->actualSize);
}

bool spMovesListIsEmpty(SPMovesList* src) {
    return !(src == NULL || src->actualSize != 0);
}

int spMoveToMove(SPMove* toMove, SPMove* fromMove){
    toMove->src->coloumn = fromMove->src->coloumn;
    toMove->src->row = fromMove->src->row;
    toMove->dest->coloumn = fromMove->dest->coloumn;
    toMove->dest->row = fromMove->dest->row;
    return 1;
}

