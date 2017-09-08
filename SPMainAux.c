#include "SPMainAux.h"

SPGame* StartNewGame(){
	SPGame* game = spGameCreate(SIZE_OF_HISTORY);
	if (game == NULL){
		printf("Error: malloc has failed\n");
		return NULL;
	}
	spGamePrintBoard(game);
	printf("Please make the next move:\n");
	return game;
}

int handleIfOver(SPGame* game){
	char ch = spCheckWinner(game);
	if(ch != '\0'){
		spGamePrintBoard(game);
		if(ch == SP_GAME_PLAYER_1_SYMBOL)
			printf("Game over: you win\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
		else if(ch == SP_GAME_PLAYER_2_SYMBOL)
			printf("Game over: computer wins\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
		else if(ch == SP_GAME_TIE_SYMBOL)
			printf("Game over: it\'s a tie\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
		else printf("Error");
		return 1;
	}
	return 0;
}
SPGame* ExecuteCmd(SPGame* game , SPCommand cmd , int difficulty){
	int last = -1 , flag = spCheckWinner(game);
	switch(cmd.cmd){
	case SP_QUIT:
		printf("Exiting...\n");
		spGameDestroy(game);
		return NULL;
	case SP_INVALID_LINE:
		printf("Error: invalid command\n");
		return game;
	case SP_RESTART:
		return game;
	case SP_UNDO_MOVE:
		last = spArrayListGetLast(game->history) + 1;
		if(spGameUndoPrevMove(game) != SP_GAME_SUCCESS)
			printf("Error: cannot undo previous move!\n");
		else{
			printf("Remove disc: remove computer's disc at column %d\n" , last);
			last = spArrayListGetLast(game->history) + 1;
			spGameUndoPrevMove(game);
			printf("Remove disc: remove user's disc at column %d\n" , last);
			spGamePrintBoard(game);
			printf("Please make the next move:\n");
		}
		return game;
	case SP_SUGGEST_MOVE:
		if(flag != '\0'){
			printf("Error: the game is over\n");
			return game;
		}
		printf("Suggested move: drop a disc to column %d\n", spMinimaxSuggestMove(game , difficulty) + 1);
		return game;
	case SP_ADD_DISC:
		if(flag != '\0'){
			printf("Error: the game is over\n");
			return game;
		}
		if(cmd.validArg == true && cmd.arg <=7 && cmd.arg >=1){
			if (spGameSetMove(game, cmd.arg - 1) == SP_GAME_INVALID_MOVE)
				printf("Error: column %d is full\n", cmd.arg);
			else if(!handleIfOver(game)){
				int suggestMove = spMinimaxSuggestMove(game, difficulty);
				spGameSetMove(game, suggestMove);
				printf("Computer move: add disc to column %d\n", suggestMove + 1);
				if(!handleIfOver(game)){
					spGamePrintBoard(game);
					printf("Please make the next move:\n");
				}
			}
		}
		else
			printf("Error: column number must be in range 1-7\n");
		return game;
	}
	return 0;
}
