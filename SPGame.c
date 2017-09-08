#include "SPFIARGame.h"
#include "SPMiniMax.h"


SPFiarGame* spFiarGameCreate(int historySize){
	if(historySize<=0) return NULL;
	SPFiarGame* game = (SPFiarGame *) malloc(sizeof(SPFiarGame));
	if(game == NULL) return NULL;	//puts("Error: malloc has failed");
	game->history = spArrayListCreate(historySize);
	if(game->history == NULL){
		free(game);
		return NULL;
	}
	game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		game->tops[i] = 0;
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
			(game->gameBoard)[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
	return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	if(src == NULL) return NULL;
	SPFiarGame* cpy = (SPFiarGame *) malloc(sizeof(SPFiarGame));
	if(cpy == NULL) return NULL;
	cpy->history = spArrayListCopy(src->history);
	if(cpy->history == NULL){
		free(cpy);
		return NULL;
	}
	cpy->currentPlayer = src->currentPlayer;
	for(int i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
		for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
			cpy->gameBoard[i][j] = src->gameBoard[i][j];
	for(int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		cpy->tops[i] = src->tops[i];
	return cpy;
}

void spFiarGameDestroy(SPFiarGame* src){
	if(src != NULL){
		if(src->history != NULL)
			spArrayListDestroy(src->history);
	free(src);
	}
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if (src == NULL || col < 0 || col >= SP_FIAR_GAME_N_COLUMNS)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (!spFiarGameIsValidMove(src,col))
		return SP_FIAR_GAME_INVALID_MOVE;
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL){
		src->gameBoard[src->tops[col]][col] = SP_FIAR_GAME_PLAYER_1_SYMBOL;
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	else {
		src->gameBoard[src->tops[col]][col] = SP_FIAR_GAME_PLAYER_2_SYMBOL;
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	src->tops[col]++;
	if (src->history->actualSize == src->history->maxSize)
		spArrayListRemoveFirst(src->history);
	spArrayListAddLast(src->history, col);
	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	if(src == NULL || col < 0 || col >= SP_FIAR_GAME_N_COLUMNS)
			return false;
		if(src->tops[col] < SP_FIAR_GAME_N_ROWS)
			return true;
	return false;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	if (src == NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(src->history)) //actual size less than 2
		return SP_FIAR_GAME_NO_HISTORY;
	int col = spArrayListGetLast(src->history);
	src->tops[col]--;
	int row = src->tops[col];
	src->gameBoard[row][col] = SP_FIAR_GAME_EMPTY_ENTRY;
	spArrayListRemoveLast(src->history);
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) //change player turn
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	else
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if (src==NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	for(int r = SP_FIAR_GAME_N_ROWS-1; r>=0; r--){
		printf("| ");
		for(int c = 0; c<SP_FIAR_GAME_N_COLUMNS; c++)
			if(src->tops[c] > r)
				printf("%c ", src->gameBoard[r][c]);
			else
				printf("%c ", SP_FIAR_GAME_EMPTY_ENTRY);
		printf("|\n");
	}
	printf("-----------------\n");
	printf("  1 2 3 4 5 6 7  \n");
		return SP_FIAR_GAME_SUCCESS;
}

char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if(src == NULL) return SP_FIAR_GAME_EMPTY_ENTRY;
	if(src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL || src->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL)
		return src->currentPlayer;
	return SP_FIAR_GAME_EMPTY_ENTRY;
}

char spFiarCheckWinner(SPFiarGame* src) {
	if (src != NULL) {
		int score = spMinimaxScoring(src);
		if (score == INT_MIN) //computer win
			return SP_FIAR_GAME_PLAYER_2_SYMBOL;
		if (score == INT_MAX) //user win
			return SP_FIAR_GAME_PLAYER_1_SYMBOL;
		if (fullBoard(src))
			return SP_FIAR_GAME_TIE_SYMBOL;
	}
	return '\0';
}

bool fullBoard(SPFiarGame* src) {
	for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++)
		if (src->tops[col] < SP_FIAR_GAME_N_ROWS)
			return false;
	return true;
}
