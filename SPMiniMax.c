#include "SPMiniMax.h"

/**
* Given a cell, this function evaluates its value:
* 1 if it contains player 1 disc, -1 if it contains player 2 disc, 0 if empty.
*
* @param cell - cell content
* @return
* the value of the cell to the Scoring Function as described above
*/ /*
int checkCell(char cell) {
	if (cell == SP_GAME_PLAYER_1_SYMBOL) {
		return 1;
	}
	if (cell == SP_GAME_PLAYER_2_SYMBOL) {
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
*/ /*
void setCounter(SPGame* game, int count[8], int row, int col) {
	int dirCount[4] = { 0,0,0,0 };
	for (int i = 0; i < 4; i++) {
		if (col <= SP_GAMEBOARD_SIZE - 4) {
			dirCount[0] += checkCell(game->gameBoard[row][col + i]);//right
		}
		if (row >= 3) {
			dirCount[1] += checkCell(game->gameBoard[row - i][col]);//down
		}
		if (row >= 3 && col <= SP_GAMEBOARD_SIZE - 4) {
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
* On success the function returns a number between [0,SP_GAMEBOARD_SIZE -1]
* which is the best move for the current player.
*//*
int spMinimaxScoring(char board[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], SP_USER_COLOR color) {
	char c;
	int i,j, whiteScore, blackScore, boardScore;
	//pawn = 1 , knight = 3 , bishop = 3 , rook = 5, queen = 9, king=100
	for (i = 0; i < SP_GAMEBOARD_SIZE; i++){
		for (j = 0; j < SP_GAMEBOARD_SIZE; j++){
			c = board[i][j];
			switch (c) {
				case W_PAWN:
					whiteScore+=1;
					break;
				case W_KNIGHT:
					whiteScore+=3;
					break;
				case W_BISHOP:
					whiteScore+=3;
					break;
				case W_ROOK:
					whiteScore+=5;
					break;
				case W_QUEEN:
					whiteScore+=9;
					break;
				case W_KING:
					whiteScore+=100;
					break;
				case B_PAWN:
					blackScore+=1;
					break;
				case B_KNIGHT:
					blackScore+=3;
					break;
				case B_BISHOP:
					blackScore+=3;
					break;
				case B_ROOK:
					blackScore+=5;
					break;
				case B_QUEEN:
					blackScore+=9;
					break;
				case B_KING:
					blackScore+=100;
					break;

			}
		}
	}
	boardScore = (color==WHITE) ? whiteScore-blackScore : blackScore-whiteScore;
	return boardScore;
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
* On success the function returns a number between [0,SP_GAMEBOARD_SIZE -1]
* which is the best move for the current player.
*/ /*
int spMinimaxSuggestMove(SPGame* currentGame,
	unsigned int maxDepth) {
	SPGame* game;
	int tempScore, finalScore, col = -1;
	bool imax = false, madeMove = false;
	if (maxDepth <= 0 || currentGame == NULL)
	{
		return -1;
	}
	imax = (spGameGetCurrentPlayer(currentGame) == SP_GAME_PLAYER_1_SYMBOL);
	finalScore = INT_MAX ? imax : INT_MIN;
	for (int i = 0; i < SP_GAMEBOARD_SIZE; i++)
	{
		if (1)//spGameIsValidMove(currentGame, i))
		{
			if (!0){//(game = spGameCopy(currentGame))) {
				spGameDestroy(currentGame);
				return -1;
			}
			//spGameSetMove(game, i);
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
			spGameDestroy(game);
		}
	}
	/*if (col == -1) {
		for (int i = 0; i < SP_GAMEBOARD_SIZE; i++)
			if (currentGame->tops[i] != SP_GAMEBOARD_SIZE)
				return i;
	}*/ /*
	return col;
}
*/
