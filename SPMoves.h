#ifndef UNTITLED1_SPMOVES_H
#define UNTITLED1_SPMOVES_H

#include <stdlib.h>
#include <stdbool.h>

// a struct which represents a tile
typedef struct sp_tile {
    int row;
    int coloumn;
} SPTile;

// a struct which represents a move
typedef struct sp_move {
    SPTile* src;
    SPTile* dest;
} SPMove;

// a struct which represents a list of moves
typedef struct sp_array_list_t {
    SPMove** moves;
    int actualSize;
    int maxSize;
} SPMovesList;

// enum which represents massages from the moves list functions
typedef enum sp_array_list_message_t {
    SP_MOVES_LIST_SUCCESS,
    SP_MOVES_LIST_INVALID_ARGUMENT,
    SP_MOVES_LIST_FULL,
} SP_MOVES_LIST_MESSAGE;

/**
 * Creates a new tile with the row an column given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new tile is returned.
 */
SPTile* spCreateTile(int row , int col);

/**
 * Creates a new move with the source and destination rows and columns given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new move is returned.
 */
SPMove* spCreateMove(int srcRow , int srcCol ,int desRow , int desCol);

/**
 * free the memory allocated by the move given
 */
void spDestroyMove(SPMove* move);

/**
 * Creates a new move with the source and destination tiles given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new move is returned.
 */
SPMove* spCreateMoveFromTile(SPTile* srcTile, SPTile* destTile);


/**
 * Creates a new moves list with the maxSize given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new move's list is returned.
 */
SPMovesList* spMovesListCreate(int maxSize);


/**
 * free the memory allocated by the moves list given
 */
void spMovesListDestroy(SPMovesList* src);

/**
 * Adds a given move to the moves list in the specified location index
 *
 * @return
 * SP_MOVES_LIST_INVALID_ARGUMENT if either a memory allocation failure.
 * SP_MOVES_LIST_FULL if the list is already full.
 * Otherwise, SP_MOVES_LIST_SUCCESS is returned.
 */
SP_MOVES_LIST_MESSAGE spMovesListAddAt(SPMovesList* src, SPMove* move, int index);

/**
 * Adds a given move to the moves list in the end
 *
 * @return
 * SP_MOVES_LIST_INVALID_ARGUMENT if either a memory allocation failure.
 * SP_MOVES_LIST_FULL if the list is already full.
 * Otherwise, SP_MOVES_LIST_SUCCESS is returned.
 */
SP_MOVES_LIST_MESSAGE spMovesListAddLast(SPMovesList* src, SPMove* move);

/**
 * returns the move in the moves list on the mentioned index
 *
 * @return
 * NULL on bad input.
 * Otherwise, the move is returned.
 */
SPMove* spMovesListGetAt(SPMovesList* src, int index);

/**
 * Duplicates the fromMove values into the toMove given
 */
void spMoveToMove(SPMove* toMove, SPMove* fromMove);

#endif //UNTITLED1_SPMOVES_H