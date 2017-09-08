#include "SPGame.h"
#include "SPMiniMax.h"


SPGame* spGameCreate(){
	SPGame* game = (SPGame *) malloc(sizeof(SPGame));
	if(game == NULL) return NULL;	//puts("Error: malloc has failed");
	game->currentPlayer = 1;
	game->gameBoard[0][0] = game->gameBoard[7][0] = game->gameBoard[0][7] = game->gameBoard[7][7] = 'R';
	game->gameBoard[0][1] = game->gameBoard[7][1] = game->gameBoard[0][6] = game->gameBoard[7][6] = 'N';
	game->gameBoard[0][2] = game->gameBoard[7][2] = game->gameBoard[0][5] = game->gameBoard[7][5] = 'B';
	game->gameBoard[0][4] = game->gameBoard[7][4] = 'K';
	game->gameBoard[0][3] = game->gameBoard[7][3] = 'Q';
	for(int i = 0; i < SP_GAME_ROWS; i++) {
		game->gameBoard[6][i] = 'M';
		game->gameBoard[1][i] = 'm';
		game->gameBoard[0][i] += 32;
		for(int j = 2; j < 6; j++){
			game->gameBoard[j][i] = SP_GAME_EMPTY_ENTRY;
		}
	}
	return game;
}

SP_GAME_MESSAGE spGamePrintBoard(SPGame* src){
	if (src==NULL)
		return SP_GAME_INVALID_ARGUMENT;
	int i, j;
	for (i = 7; i >= 0; i--){
		printf("%d|",i);
		for (j = 0; j < 8; j++){
			printf(" %c", src->gameBoard[i][j]);
		}
		printf(" |\n");
	}
	printf("  -----------------\n  ");
	for (i = 65; i < 73; i++){
		printf(" %c", i);
	}
	printf("\n");
	return SP_GAME_SUCCESS;
}


SP_GAME_MESSAGE spGameSetMove(SPGame* src, int col){
	if (src == NULL || col < 0 || col >= SP_GAME_COLUMNS)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (!spGameIsValidMove(src,col))
		return SP_GAME_INVALID_MOVE;
	if (src->currentPlayer == SP_GAME_PLAYER_1_SYMBOL){
		src->gameBoard[src->tops[col]][col] = SP_GAME_PLAYER_1_SYMBOL;
		src->currentPlayer = SP_GAME_PLAYER_2_SYMBOL;
	}
	else {
		src->gameBoard[src->tops[col]][col] = SP_GAME_PLAYER_2_SYMBOL;
		src->currentPlayer = SP_GAME_PLAYER_1_SYMBOL;
	}
	src->tops[col]++;
	if (src->history->actualSize == src->history->maxSize)
		spArrayListRemoveFirst(src->history);
	spArrayListAddLast(src->history, col);
	return SP_GAME_SUCCESS;
}

char spGameGetCurrentPlayer(SPGame* src){
	if(src == NULL) return SP_GAME_EMPTY_ENTRY;
	if(src->currentPlayer == SP_GAME_PLAYER_1_SYMBOL || src->currentPlayer == SP_GAME_PLAYER_2_SYMBOL)
		return src->currentPlayer;
	return SP_GAME_EMPTY_ENTRY;
}

char spCheckWinner(SPGame* src) {
	if (src != NULL) {
		int score = spMinimaxScoring(src);
		if (score == INT_MIN) //computer win
			return SP_GAME_PLAYER_2_SYMBOL;
		if (score == INT_MAX) //user win
			return SP_GAME_PLAYER_1_SYMBOL;
		if (fullBoard(src))
			return SP_GAME_TIE_SYMBOL;
	}
	return '\0';
}

SPGame* spGameCopy(SPGame* src){
	if(src == NULL) return NULL;
	SPGame* cpy = (SPGame *) malloc(sizeof(SPGame));
	if(cpy == NULL) return NULL;
	cpy->history = spArrayListCopy(src->history);
	if(cpy->history == NULL){
		free(cpy);
		return NULL;
	}
	cpy->currentPlayer = src->currentPlayer;
	for(int i = 0; i < SP_GAME_ROWS; i++)
		for(int j = 0; j < SP_GAME_COLUMNS; j++)
			cpy->gameBoard[i][j] = src->gameBoard[i][j];
	for(int i = 0; i < SP_GAME_COLUMNS; i++)
		cpy->tops[i] = src->tops[i];
	return cpy;
}

void spGameDestroy(SPGame* src){
	if(src != NULL){
		if(src->history != NULL)
			spArrayListDestroy(src->history);
		free(src);
	}
}


bool spGameIsValidMove(SPGame* src, int col){
	if(src == NULL || col < 0 || col >= SP_GAME_COLUMNS)
		return false;
	if(src->tops[col] < SP_GAME_ROWS)
		return true;
	return false;
}

SP_GAME_MESSAGE spGameUndoPrevMove(SPGame* src) {
	if (src == NULL)
		return SP_GAME_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(src->history)) //actual size less than 2
		return SP_GAMEO_HISTORY;
	int col = spArrayListGetLast(src->history);
	src->tops[col]--;
	int row = src->tops[col];
	src->gameBoard[row][col] = SP_GAME_EMPTY_ENTRY;
	spArrayListRemoveLast(src->history);
	if (src->currentPlayer == SP_GAME_PLAYER_1_SYMBOL) //change player turn
		src->currentPlayer = SP_GAME_PLAYER_2_SYMBOL;
	else
		src->currentPlayer = SP_GAME_PLAYER_1_SYMBOL;
	return SP_GAME_SUCCESS;
}
bool fullBoard(SPGame* src){
	return 1;
}
