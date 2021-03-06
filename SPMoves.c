
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

void spDestroyMove(SPMove* move){
    if (move){
        if (move->dest)
            free(move->dest);
        if (move->src)
            free(move->src);
        free(move);
    }
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

void spMovesListDestroy(SPMovesList* src) {
    if(src != NULL){
        for (int k = 0; k < src->actualSize; k++){
            spDestroyMove(spMovesListGetAt(src, k));
        }
        free(src->moves);
        free(src);
    }
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


SP_MOVES_LIST_MESSAGE spMovesListAddLast(SPMovesList* src, SPMove* move) {
    if(src == NULL) return SP_MOVES_LIST_INVALID_ARGUMENT;
    return spMovesListAddAt(src, move, src->actualSize);
}

SPMove* spMovesListGetAt(SPMovesList* src, int index) {
    if (src == NULL || src->actualSize <= index || src->actualSize == 0) {
        return NULL;
    }
    return src->moves[index];
}

void spMoveToMove(SPMove* toMove, SPMove* fromMove){
    toMove->src->coloumn = fromMove->src->coloumn;
    toMove->src->row = fromMove->src->row;
    toMove->dest->coloumn = fromMove->dest->coloumn;
    toMove->dest->row = fromMove->dest->row;
}

