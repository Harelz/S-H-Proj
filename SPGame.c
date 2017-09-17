#include "SPGame.h"
#include "SPParser.h"


int gameHandler(SPGame* game, SPGameCommand cmd) {
    switch(cmd.cmd){
        case SP_RESET:
            printf("Restarting...\n");
            spGameDestroy(game);
            return 0; //indicates to run main all over again
        case SP_GQUIT:
            printf("Exiting...\n");
            return -1; //indicates to terminate
        case SP_UNDO:
            if (game->settings->game_mode == 2)
                printf("Empty history, move cannot be undone\n");
            else if (spQueueIsEmpty(game->history))
                printf("Can't Undo, no previous moves!\n");
            else {
                char** prevBoard = spStackPop(game->history)->data;
                memcpy(game->gameBoard, prevBoard, sizeof(game->gameBoard));
                printf("Undo move for player %s : <x,y> -> <w,z>\n", game->settings->p1_color == WHITE ? "white" : "black");
                printf("Undo move for player %s : <x,y> -> <w,z>\n", game->settings->p1_color == BLACK ? "white" : "black");
            }
            return 1;
        case SP_SAVE:
            IS_VALID(cmd);
            saveGame(game, cmd.pathArg);
            return 1;
        case SP_MOVE:
            IS_VALID(cmd);
            if (!IN_RANGE(cmd.move->src->coloumn, 0,8) || !IN_RANGE(cmd.move->dest->coloumn, 0,8)
                    || !IN_RANGE(cmd.move->src->row, 0,8) || !IN_RANGE(cmd.move->dest->row, 0,8))
                printf("Invalid position on the board\n");
            else if (game->gameBoard[cmd.move->src->row][cmd.move->src->coloumn] == SP_GAME_EMPTY_ENTRY ||
                    getColor(game->gameBoard[cmd.move->src->row][cmd.move->src->coloumn]) != game->settings->p1_color)
                printf("The specified position does not contain your piece\n");
            else if (spGameSetMove(game, cmd.move) == SP_GAME_INVALID_MOVE)
                printf("Illegal move\n");
            return 1;
        case SP_GET_MOVES:
            IS_VALID(cmd);
            if (!IN_RANGE(cmd.tile->coloumn, 0,8) || !IN_RANGE(cmd.tile->coloumn, 0,8))
                printf("Invalid position on the board\n");
            else if (game->gameBoard[cmd.tile->row][cmd.tile->coloumn] == SP_GAME_EMPTY_ENTRY ||
                     getColor(game->gameBoard[cmd.tile->row][cmd.tile->coloumn]) != game->settings->p1_color)
                printf("The specified position does not contain your piece\n");
            else {
                SPMovesList* mlst = spGameGetMoves(game, cmd.tile->row, cmd.tile->coloumn);
                for (int i = 0; i < mlst->actualSize; i++) {
                    SPTile *t = spMovesListGetAt(mlst, i)->dest;
                    if (game->settings->difficulty == SP_DIFF_EASY || game->settings->difficulty == SP_DIFF_NOOB)
                        printf("<%d,%c>%s%s\n", t->row + 1, t->coloumn + 'A',
                               spGameTileIsThreatened(game,t,getColor(game->gameBoard[cmd.tile->row][cmd.tile->coloumn])) ? "*" : "",
                               (game->gameBoard[t->row][t->coloumn] != SP_GAME_EMPTY_ENTRY
                                && getColor(game->gameBoard[t->row][t->coloumn]) != getColor(game->gameBoard[cmd.tile->row][cmd.tile->coloumn])) ? "^" : ""); // check thretened and eaten
                    else
                        printf("<%d,%c>\n", t->row + 1, t->coloumn + 'A');
                }
            }
            return 1;
        case SP_GINVALID_LINE:
            PRINT_INVALID_COMMAND;
            return 1;
    }
    return -1;
}

bool spGameTileIsThreatened(SPGame* game , SPTile* tile , int targetColor){
    SPMovesList* mlst = spGameGetAllMoves(game);
    for(int i = 0; i<mlst->actualSize; i++){
        SPMove* atk = spMovesListGetAt(mlst,i);
        if(getColor(game->gameBoard[atk->src->row][atk->src->coloumn]) != targetColor
           && atk->dest->row == tile->row && atk->dest->coloumn == tile->coloumn){
            return true;
        }
    }
    return false;

}


int saveGame(SPGame* game, char* fpath){
    FILE *fp;
    int i,j;
    char tile;
    fp = fopen(fpath, "w+");
    if (fp == NULL) {
        printf("File cannot be created or modified\n");
        return 0;
    }
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<game>\n");
    fprintf(fp, "\t<current_turn>%d</current_turn>\n", game->settings->curr_turn);
    fprintf(fp, "\t<game_mode>%d</game_mode>\n", game->settings->game_mode);
    if (game->settings->game_mode == 1) {
        fprintf(fp, "\t<difficulty>%d</difficulty>\n", game->settings->difficulty);
        fprintf(fp, "\t<user_color>%d</user_color>\n", game->settings->p1_color);
    }
    fprintf(fp, "\t<board>\n");
    for (i = 8; i > 0; i--){
        fprintf(fp, "\t\t<row_%d>",i);
        for (j = 0; j < 8; j++){
            fprintf(fp, "%c", game->gameBoard[i-1][j]);
        }
        fprintf(fp, "</row_%d>\n",i);
    }
    fprintf(fp, "\t</board>\n");
    fprintf(fp, "</game>\n");
    fclose(fp);
    return 1;
}

int loadGame(SPGame* game, char* fpath){
    FILE *fp;
    int i, j;
    char line[9999];
    fp = fopen(fpath, "r");
    if (fp == NULL) {
        printf("Error: File doesn’t exist or cannot be opened\n");
        return 0;
    }
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);
    game->settings->curr_turn = line[15]-'0';
    fgets(line, sizeof(line), fp);
    game->settings->game_mode = line[12]-'0';
    if (game->settings->game_mode == 1) {
        fgets(line, sizeof(line), fp);
        game->settings->difficulty = line[13] - '0';
        fgets(line, sizeof(line), fp);
        game->settings->p1_color = line[13] - '0';
    }
    fgets(line, sizeof(line), fp);
    for (i = 8; i > 0; i--){
        fgets(line, sizeof(line), fp);
        for (j = 0; j < 8; j++)
            game->gameBoard[i-1][j] = line[9+j];
    }
    fclose(fp);
    return 1;
}

SPGame* spGameCreateDef(){
    SPGame* game = (SPGame *) malloc(sizeof(SPGame));
    if(game == NULL) return NULL;	//puts("Error: malloc has failed");
    game->currentPlayer = SP_USER_COLOR_WHITE;
    game->history = spQueueCreate(3);
    game->settings = defaultSettings(NULL);
    spSetNewBoard(game);
}

SPGame* spGameCreate (SPSettings* settings){
	SPGame* game = (SPGame *) malloc(sizeof(SPGame));
	if(game == NULL) return NULL;	//puts("Error: malloc has failed");
	game->currentPlayer = SP_USER_COLOR_WHITE;
    game->history = spQueueCreate(3);
    if(settings == NULL)    game->settings = defaultSettings(NULL);
    else game ->settings = settings;
    spSetNewBoard(game);
    return game;
}

SPGame* spSetNewBoard(SPGame* game) {
    if (game == NULL)
        return game;
    game->gameBoard[0][0] = game->gameBoard[7][0] = game->gameBoard[0][7] = game->gameBoard[7][7] = B_ROOK;
    game->gameBoard[0][1] = game->gameBoard[7][1] = game->gameBoard[0][6] = game->gameBoard[7][6] = B_KNIGHT;
    game->gameBoard[0][2] = game->gameBoard[7][2] = game->gameBoard[0][5] = game->gameBoard[7][5] = B_BISHOP;
    game->gameBoard[0][4] = game->gameBoard[7][4] = B_KING;
    game->gameBoard[0][3] = game->gameBoard[7][3] = B_QUEEN;
    for(int i = 0; i < SP_GAMEBOARD_SIZE; i++) {
        game->gameBoard[6][i] = B_PAWN;
        game->gameBoard[1][i] = W_PAWN;
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
		printf("%d|",i+1);
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


SP_GAME_MESSAGE spGameSetMove(SPGame* src, SPMove* move){
	if (!spGameIsValidMove(src, move->src->row, move->src->coloumn, move->dest->row, move->dest->coloumn))
		return SP_GAME_INVALID_MOVE;
    if (src->history->actualSize == src->history->maxSize) {
        spQueuePop(src->history);
        spQueuePush(src->history, src->gameBoard);
    }
    else
        spQueuePush(src->history, src->gameBoard);
    src->gameBoard[move->dest->row][move->dest->coloumn] = src->gameBoard[move->src->row][move->src->coloumn];
    src->gameBoard[move->src->row][move->src->coloumn] = SP_GAME_EMPTY_ENTRY;
	return SP_GAME_SUCCESS;
}

bool spGameIsValidMove(SPGame* src, int srcRow , int srcCol , int desRow, int desCol){
    char piece = src->gameBoard[srcRow][srcCol];
    if(srcCol == desCol && srcRow == desRow) return false;
    if(src->gameBoard[desRow][desCol] != SP_GAME_EMPTY_ENTRY
       && getColor(src->gameBoard[desRow][desCol]) == getColor(src->gameBoard[srcRow][srcCol])) return false;
    if(piece == W_PAWN || piece == B_PAWN) return checkValidStepForM(src , srcRow , srcCol , desRow , desCol);
    if(piece == W_ROOK || piece == B_ROOK) return checkValidStepForR(src , srcRow , srcCol , desRow , desCol);
    if(piece == W_BISHOP || piece == B_BISHOP) return checkValidStepForB(src , srcRow , srcCol , desRow , desCol);
    if(piece == W_QUEEN || piece == B_QUEEN) return (checkValidStepForR(src , srcRow , srcCol , desRow , desCol)
                                             || checkValidStepForB(src , srcRow , srcCol , desRow , desCol));
    if(piece == B_KNIGHT || piece == W_KNIGHT) return checkValidStepForN(srcRow , srcCol , desRow , desCol);
    if(piece == B_KING || piece == W_KING) return checkValidStepForK(srcRow , srcCol , desRow , desCol);
    return false;
}

bool checkValidStepForM(SPGame* src, int srcRow , int srcCol , int desRow, int desCol){
    char piece = src->gameBoard[srcRow][srcCol];
    if(piece == B_PAWN && srcCol == desCol) { // go straight no killing
        if (src->gameBoard[srcRow - 1][srcCol] == SP_GAME_EMPTY_ENTRY &&
            (desRow - srcRow == -1 || (desRow - srcRow == -2 && srcRow == 6 && src->gameBoard[srcRow - 2][srcCol] == SP_GAME_EMPTY_ENTRY))) {
            return true;
        }
    }
    else if(piece == B_PAWN && abs(desCol - srcCol) == 1) { // go right or left with killing
        if (desRow - srcRow == -1 && src->gameBoard[desRow][desCol] != SP_GAME_EMPTY_ENTRY
            && getColor(src->gameBoard[desRow][desCol]) == WHITE) {
            return true;
        }
    }
    else if(piece == W_PAWN && srcCol == desCol) { // go straight no killing
        if (src->gameBoard[srcRow + 1][srcCol] == SP_GAME_EMPTY_ENTRY &&
            (desRow - srcRow == 1 || (desRow - srcRow == 2 && srcRow == 1 && src->gameBoard[srcRow + 2][srcCol] == SP_GAME_EMPTY_ENTRY))) {
            return true;
        }
    }
    else if(piece == W_PAWN && abs(desCol - srcCol) == 1) { // go right or left with killing
        if (desRow - srcRow == 1 && src->gameBoard[desRow][desCol] != SP_GAME_EMPTY_ENTRY
            && getColor(src->gameBoard[desRow][desCol]) == BLACK) {
            return true;
        }
    }
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
            for(int i = 1; i< desRow-srcRow; i++){
                if(src->gameBoard[srcRow+i][srcCol+i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
        else{ // going positive slope down
            for(int i = 1; i< srcRow-desRow; i++){
                if(src->gameBoard[srcRow-i][srcCol-i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
    }
    else if(desRow - srcRow == -(desCol - srcCol) ){ // going negative slope-up\down
        if(desRow > srcRow){ //going negative slope up
            for(int i = 1; i< desRow-srcRow; i++){
                if(src->gameBoard[srcRow+i][srcCol-i] != SP_GAME_EMPTY_ENTRY) return false;
            }
        }
        else{ // going negative slope down
            for(int i = 1; i< srcRow-desRow; i++){
                if(src->gameBoard[srcRow-i][srcCol+i] != SP_GAME_EMPTY_ENTRY) return false;
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

SPMovesList* spGameGetMoves(SPGame* src , int row , int col){
    SPMovesList* moves = spMovesListCreate(65);
    if(src->gameBoard[row][col] == SP_GAME_EMPTY_ENTRY) return moves;
    for(int r=0; r<SP_GAMEBOARD_SIZE;r++){
        for(int c=0; c<SP_GAMEBOARD_SIZE;c++){
            if(spGameIsValidMove(src,row,col,r,c)){
                spMovesListAddLast(moves, spCreateMove(row,col,r,c));
            }
        }
    }
    return moves;
}

SPMovesList* spGameGetAllMoves(SPGame* src) {
    SPMovesList* moves = spMovesListCreate(2049);
    for(int r=0; r<SP_GAMEBOARD_SIZE;r++) {
        for (int c = 0; c < SP_GAMEBOARD_SIZE; c++) {
            SPMovesList* temp = spGameGetMoves(src,r,c);
            for(int i =0; i<temp->actualSize; i++){
                spMovesListAddLast(moves,spMovesListGetAt(temp,i));
            }
        }
    }
    return moves;
}

char spGameIsCheck(SPGame* src){
    SPMovesList* moves = spGameGetAllMoves(src);
    for(int i = 0 ; i<moves->actualSize; i++){
        SPTile* t = spMovesListGetAt(moves , i) -> dest;
        char target = src->gameBoard[t->row][t->coloumn];
        if(target == 'K')   return BLACK;
        else if(target == 'k') return WHITE;
    }
    return NULL;
}

char spGameGetCurrentPlayer(SPGame* src){
	if(src == NULL) return SP_GAME_EMPTY_ENTRY;
	if(src->currentPlayer == SP_USER_COLOR_WHITE || src->currentPlayer == SP_USER_COLOR_BLACK)
		return src->currentPlayer;
	return SP_GAME_EMPTY_ENTRY;
}

void spGameDestroy(SPGame* src){
	if(src != NULL){
		if(src->history != NULL)
			spQueueDestroy(src->history);
        if(src->settings != NULL)
            spSettingsDestroy(src->settings);
		free(src);
	}
}



SP_GAME_MESSAGE spGameUndoPrevMove(SPGame* src) {
	/*if (src == NULL)
		return SP_GAME_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(src->history))
		return SP_GAME_HISTORY;
	int history = spArrayListGetLast(src->history);
	spArrayListRemoveLast(src->history);*/
	return SP_GAME_SUCCESS;
}
