#include "SPGame.h"
#include "SPMiniMax.h"

SPGame* spGameCreateDef(){
    SPGame* game = (SPGame *) malloc(sizeof(SPGame));
    if(game == NULL) return NULL;	//puts("Error: malloc has failed");
    game->currentPlayer = 1;
    game->history = spArrayListCreate(3);
    game->settings = settings_default_values(NULL);
    spSetNewBoard(game);
}

SPGame* spGameCreate (SPSettings* settings){
	SPGame* game = (SPGame *) malloc(sizeof(SPGame));
	if(game == NULL) return NULL;	//puts("Error: malloc has failed");
	game->currentPlayer = 1;
    game->history = spArrayListCreate(3);
    if(settings == NULL)    game->settings = settings_default_values(NULL);
    else game ->settings = settings;
    spSetNewBoard(game);
    return game;
}

SP_GAME_MESSAGE spSetNewBoard(SPGame* game) {
    if (game == NULL)
        return SP_GAME_INVALID_ARGUMENT;
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


SP_GAME_MESSAGE spGameSetMove(SPGame* src, int srcRow , int srcCol , int desRow, int desCol){
    if (src == NULL || srcCol < 0 || srcCol >= SP_GAME_COLUMNS || srcRow < 0 || srcRow >= SP_GAME_ROWS || desCol < 0 || desCol >= SP_GAME_COLUMNS || desRow < 0 || desRow >= SP_GAME_ROWS)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
    if(src ->gameBoard[srcRow][srcCol] == SP_GAME_EMPTY_ENTRY)
        return SP_GAME_EMPTY_ENTRY_MOVE;
	if (!spGameIsValidMove(src,srcRow,srcCol,desRow,desCol))
		return SP_GAME_INVALID_MOVE;
    src->gameBoard[desRow][desCol] = src->gameBoard[srcRow][srcCol];
	if (src->history->actualSize == src->history->maxSize)
		spArrayListRemoveFirst(src->history);
	spArrayListAddLast(src->history, srcRow*1000+srcCol*100+desRow*10+desCol); // need to think of a new way for saving history!
	return SP_GAME_SUCCESS;
}

bool spGameIsValidMove(SPGame* src, int srcRow , int srcCol , int desRow, int desCol) {
    if (src == NULL || srcCol < 0 || srcCol >= SP_GAME_COLUMNS || srcRow < 0 || srcRow >= SP_GAME_ROWS || desCol < 0 ||
        desCol >= SP_GAME_COLUMNS || desRow < 0 || desRow >= SP_GAME_ROWS)
        return false;
    if(srcCol == desCol && srcRow == desRow) return false;
    if (src->gameBoard[srcRow][srcCol] >= 'A' && src->gameBoard[srcRow][srcCol] <= 'Z') {
        if(src->gameBoard[desRow][desCol] >= 'A' && src->gameBoard[desRow][desCol] <= 'Z') return false;
        if ((src->settings->p1_color == SP_USER_COLOR_BLACK && src->currentPlayer == SP_GAME_PLAYER_1_SYMBOL)
            || (src->settings->p1_color == SP_USER_COLOR_WHITE && src->currentPlayer == SP_GAME_PLAYER_2_SYMBOL)) {
            return checkValidStep(src , srcRow , srcCol , desRow , desCol);
        }
    }
    else if (src->gameBoard[srcRow][srcCol] >= 'a' && src->gameBoard[srcRow][srcCol] <= 'z'){
        if(src->gameBoard[desRow][desCol] >= 'a' && src->gameBoard[desRow][desCol] <= 'z') return false;
        if( (src->settings->p1_color == SP_USER_COLOR_WHITE && src->currentPlayer == SP_GAME_PLAYER_1_SYMBOL)
            || (src->settings->p1_color == SP_USER_COLOR_BLACK && src->currentPlayer == SP_GAME_PLAYER_2_SYMBOL) ) {
            return checkValidStep(src , srcRow , srcCol , desRow , desCol);
        }
    }
    return false;
}

bool checkValidStep(SPGame* src, int srcRow , int srcCol , int desRow, int desCol){
    char piece = src->gameBoard[srcRow][srcCol];
    if(piece == 'P' || piece == 'p') return checkValidStepForP(src , srcRow , srcCol , desRow , desCol);
    if(piece == 'r' || piece == 'R') return checkValidStepForR(src , srcRow , srcCol , desRow , desCol);
    if(piece == 'b' || piece == 'B') return checkValidStepForB(src , srcRow , srcCol , desRow , desCol);
    if(piece == 'q' || piece == 'Q') return (checkValidStepForR(src , srcRow , srcCol , desRow , desCol)
                                             || checkValidStepForB(src , srcRow , srcCol , desRow , desCol));
    if(piece == 'N' || piece == 'n') return checkValidStepForN(srcRow , srcCol , desRow , desCol);
    if(piece == 'K' || piece == 'k') return checkValidStepForK(srcRow , srcCol , desRow , desCol);
    return false;
}
bool checkValidStepForP(SPGame* src, int srcRow , int srcCol , int desRow, int desCol){
    char piece = src->gameBoard[srcRow][srcCol];
    if(piece == 'P' && srcCol == desCol &&
       (desRow - srcRow == -1
        || (desRow - srcRow == -2 && srcRow == 6 && src->gameBoard[srcRow-1][srcCol] == SP_GAME_EMPTY_ENTRY))) return true;
    if(piece == 'p' && srcCol == desCol &&
       (desRow - srcRow == 1
        || (desRow - srcRow == 2 && srcRow == 1 && src->gameBoard[srcRow+1][srcCol] == SP_GAME_EMPTY_ENTRY))) return true;
    return false;
}
bool checkValidStepForR(SPGame* src, int srcRow , int srcCol , int desRow, int desCol){
    if (desRow == srcRow) { // going left or right
        if (srcCol > desCol) { //going left
            for (int i = desCol + 1; i < srcCol; i++) {
                if (src->gameBoard[srcRow][i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        } else { // going right
            for (int i = srcCol + 1; i < desCol; i++) {
                if (src->gameBoard[srcRow][i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
    } else if (desCol == srcCol) { //going up or down
        if (srcRow > desRow) { // going down
            for (int i = desRow + 1; i < srcRow; i++) {
                if (src->gameBoard[i][srcCol] != SP_GAME_EMPTY_ENTRY) return false;
            }
        } else { //going up
            for (int i = srcRow + 1; i < desRow; i++) {
                if (src->gameBoard[i][srcCol] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
    } else return false;
    return true;

}
bool checkValidStepForB(SPGame* src, int srcRow , int srcCol , int desRow, int desCol){
    if(desRow - srcRow == desCol - srcCol){ // going positive slope-up\down
        if(desRow > srcRow){ //going positive slope up
            for(int i = 1; i< desRow-srcRow-1; i++){
                if(src->gameBoard[srcRow+i][srcCol+i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
        else{ // going positive slope down
            for(int i = 1; i< srcRow-desRow-1; i++){
                if(src->gameBoard[desRow+i][desCol+i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
    }
    else if(desRow - srcRow == -(desCol - srcCol) ){ // going negative slope-up\down
        if(desRow > srcRow){ //going negative slope up
            for(int i = 1; i< desRow-srcRow-1; i++){
                if(src->gameBoard[srcRow+i][srcCol-i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
        else{ // going positive slope down
            for(int i = 1; i< srcRow-desRow-1; i++){
                if(src->gameBoard[desRow-i][desCol+i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
    }
    else return false;
    return true;
}

bool checkValidStepForN(int srcRow , int srcCol , int desRow, int desCol){
    int diffRow = abs(srcRow - desRow) , diffCol = abs(srcCol - desCol);
    if((diffRow == 2 && diffCol == 1) || (diffRow == 1 && diffCol == 2)) return true;
    return false;
}


bool checkValidStepForK(int srcRow , int srcCol , int desRow, int desCol){
    int diffRow = abs(srcRow - desRow) , diffCol = abs(srcCol - desCol);
    if((diffRow == 1 && diffCol == 1) || (diffRow == 0 && diffCol == 1) || (diffRow == 1 && diffCol == 0)) return true;
    return false;
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
    // copy settings
	return cpy;
}

void spGameDestroy(SPGame* src){
	if(src != NULL){
		if(src->history != NULL)
			spArrayListDestroy(src->history);
        if(src->settings != NULL)
            spSettingsDestroy(src->settings);
		free(src);
	}
}

SP_GAME_MESSAGE spGameUndoPrevMove(SPGame* src) {
	if (src == NULL)
		return SP_GAME_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(src->history))
		return SP_GAME_HISTORY;
	int history = spArrayListGetLast(src->history);
	spArrayListRemoveLast(src->history);
	return SP_GAME_SUCCESS;
}
