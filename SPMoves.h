#ifndef UNTITLED1_SPMOVES_H
#define UNTITLED1_SPMOVES_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct sp_tile {
    int row;
    int coloumn;
} SPTile;

typedef struct sp_move {
    SPTile* src;
    SPTile* dest;
} SPMove;

typedef struct sp_array_list_t {
    SPMove** moves;
    int actualSize;
    int maxSize;
} SPMovesList;

typedef enum sp_array_list_message_t {
    SP_MOVES_LIST_SUCCESS,
    SP_MOVES_LIST_INVALID_ARGUMENT,
    SP_MOVES_LIST_FULL,
    SP_MOVES_LIST_EMPTY
} SP_MOVES_LIST_MESSAGE;


SPTile* spCreateTile(int row , int col);
SPMove* spCreateMove(int srcRow , int srcCol ,int desRow , int desCol);
int spDestroyMove(SPMove* move);
SPMove* spCreateMoveFromTile(SPTile* srcTile, SPTile* destTile);
SPMovesList* spMovesListCreate(int maxSize);
SPMovesList* spMovesListCopy(SPMovesList* src);
void spMovesListDestroy(SPMovesList* src);
SP_MOVES_LIST_MESSAGE spMovesListClear(SPMovesList* src);
SP_MOVES_LIST_MESSAGE spMovesListAddAt(SPMovesList* src, SPMove* move, int index);
SP_MOVES_LIST_MESSAGE spMovesListAddFirst(SPMovesList* src, SPMove* move);
SP_MOVES_LIST_MESSAGE spMovesListAddLast(SPMovesList* src, SPMove* move);
SP_MOVES_LIST_MESSAGE spMovesListRemoveAt(SPMovesList* src, int index);
SP_MOVES_LIST_MESSAGE spMovesListRemoveFirst(SPMovesList* src);
SP_MOVES_LIST_MESSAGE spMovesListRemoveLast(SPMovesList* src);
SPMove* spMovesListGetAt(SPMovesList* src, int index);
SPMove* spMovesListGetFirst(SPMovesList* src);
SPMove* spMovesListGetLast(SPMovesList* src);
int spMovesListMaxCapacity(SPMovesList* src);
int spMovesListSize(SPMovesList* src);
bool spMovesListIsFull(SPMovesList* src);
bool spMovesListIsEmpty(SPMovesList* src);
int spMoveToMove(SPMove* toMove, SPMove* fromMove);

#endif //UNTITLED1_SPMOVES_H