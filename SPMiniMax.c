#include "SPMiniMax.h"

/**
* Given a cell, this function evaluates its value:
* 1 if it contains player 1 disc, -1 if it contains player 2 disc, 0 if empty.
*
* @param cell - cell content
* @return
* the value of the cell to the Scoring Function as described above
*/
int checkCell(char cell) {
	if (cell == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		return 1;
	}
	if (cell == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
		return -1;
	}
	return 0;
}

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
void setCounter(SPFiarGame* game, int count[8], int row, int col) {
	int dirCount[4] = { 0,0,0,0 };
	for (int i = 0; i < 4; i++) {
		if (col <= SP_FIAR_GAME_N_COLUMNS - 4) {
			dirCount[0] += checkCell(game->gameBoard[row][col + i]);//right
		}
		if (row >= 3) {
			dirCount[1] += checkCell(game->gameBoard[row - i][col]);//down
		}
		if (row >= 3 && col <= SP_FIAR_GAME_N_COLUMNS - 4) {
			dirCount[2] += checkCell(game->gameBoard[row - i][col + i]);//right down diagonal
		}
		if (row >= 3 && col >= 3) {
			dirCount[3] += checkCell(game->gameBoard[row - i][col - i]);//left down diagonal
		}
	}
	for (int i = 0; i < 4; i++) {
		if (dirCount[i] > 0) {
			count[dirCount[i] + 3]++;
		}
		if (dirCount[i] < 0) {
			count[dirCount[i] + 4]++;
		}
	}
	return;
}

/**
* Given a game state, this function evaluates the score of the state
* using the scoring function described on the PDF.
*
* @param currentGame - The current game state
* @return
* -1 if either currentGame is NULL
* On success the function returns a number between [0,SP_FIAR_GAME_N_COLUMNS -1]
* which is the best move for the current player.
*/
int spMinimaxScoring(SPFiarGame* currentGame) {
	if (currentGame == NULL)
	{
		return -1;
	}
	int weight[WEIGHT_VECTOR_SIZE] = { INT_MIN, -5, -2, -1, 1, 2, 5, INT_MAX };
	int count[WEIGHT_VECTOR_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int score = 0, i = 0;
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			setCounter(currentGame, count, i, j);
		}
	}
	if (count[0] > 0)
	{
		return weight[0];
	}
	if (count[WEIGHT_VECTOR_SIZE - 1] > 0)
	{
		return weight[WEIGHT_VECTOR_SIZE - 1];
	}
	for (i = 1; i < WEIGHT_VECTOR_SIZE - 1; i++)
	{
		score += count[i] * weight[i];
	}
	return score;
}

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
* On success the function returns a number between [0,SP_FIAR_GAME_N_COLUMNS -1]
* which is the best move for the current player.
*/
int spMinimaxSuggestMove(SPFiarGame* currentGame,
	unsigned int maxDepth) {
	SPFiarGame* game;
	int tempScore, finalScore, col = -1;
	bool imax = false, madeMove = false;
	if (maxDepth <= 0 || currentGame == NULL)
	{
		return -1;
	}
	imax = (spFiarGameGetCurrentPlayer(currentGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL);
	finalScore = INT_MAX ? imax : INT_MIN;
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
	{
		if (spFiarGameIsValidMove(currentGame, i))
		{
			if (!(game = spFiarGameCopy(currentGame))) {
				spFiarGameDestroy(currentGame);
				return -1;
			}
			spFiarGameSetMove(game, i);
			tempScore = spMinimaxNodeCalc(game, maxDepth - 1, !imax);
			if (!madeMove)
			{
				finalScore = tempScore;
				col = i;
				madeMove = !madeMove;
			}
			else if (imax && tempScore > finalScore) {
				col = i;
				finalScore = tempScore;
			}
			else if (!imax && tempScore < finalScore) {
				col = i;
				finalScore = tempScore;
			}
			spFiarGameDestroy(game);
		}
	}
	if (col == -1) {
		for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
			if (currentGame->tops[i] != SP_FIAR_GAME_N_ROWS)
				return i;
	}
	return col;
}
