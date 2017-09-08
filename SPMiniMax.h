#ifndef SPMINIMAX_H_
#define SPMINIMAX_H_
#include "SPGame.h"
#include <limits.h>
#include "SPMiniMaxNode.h"
#include <stdio.h>
#define WEIGHT_VECTOR_SIZE 8
#define MIN(a,b)(((a)<(b))?(a):(b)) //returns Minimum of 2
#define MAX(a,b)(((a)>(b))?(a):(b)) //returns Maximum of 2

/**
* Given a game state, this function evaluates the best move according to
* the current player. The function initiates a MiniMax algorithm up to a
* specified length given by maxDepth. The current game state doesn't change
* by this function including the history of previous moves.
*
* @param currentGame - The current game state
* @param maxDepth - The maximum depth of the miniMax algorithm
* @return
* -1 if either currentGame is NULL or maxDepth <= 0.
* On success the function returns a number between [0,SP_GAME_COLUMNS -1]
* which is the best move for the current player.
*/
int spMinimaxSuggestMove(SPGame* currentGame,
	unsigned int maxDepth);

/**
* Given a cell, this function evaluates its value:
* 1 if it contains player 1 disc, -1 if it contains player 2 disc, 0 if empty.
*
* @param cell - cell content
* @return
* the value of the cell to the Scoring Function as described above
*/
int checkCell(char cell);

/**
* Given a game state, this function evaluates the score of the state
* using the scoring function described on the PDF.
*
* @param game - The game state
* @param count - count array as described on PDF
* @param row - current row to check 4's from
* @param col current collumn to check 4's from
* @return
* void
*/
void updateCounter(SPGame* game, int count[8], int row, int col);


/**
* Given a game state, this function evaluates the score of the state
* using the scoring function described on the PDF.
*
* @param currentGame - The current game state
* @return
* -1 if either currentGame is NULL
* On success the function returns a number between [0,SP_GAME_COLUMNS -1]
* which is the best move for the current player.
*/
int spMinimaxScoring(SPGame* currentGame);


#endif
