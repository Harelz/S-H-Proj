#include "SPGame.h"
#include "SPParser.h"
#include "SPMiniMax.h"

int spGameHandler(SPGame *game, SPGameCommand cmd) {
    char* undo_msg;
    switch(cmd.cmd){
        case SP_RESET:
            printf("Restarting...\n");
            spGameDestroy(game);
            return 0; //indicates to run main all over again
        case SP_GQUIT:
            printf("Exiting...\n");
            return -1; //indicates to terminate
        case SP_UNDO:
            undo_msg = spGameUndoHandler(game);
            printf("%s", undo_msg);
            free(undo_msg);
            return 3;
        case SP_SAVE:
            IS_VALID(cmd);
            spGameSaveHandler(game, cmd.pathArg);
            return 3;
        case SP_MOVE:
            IS_VALID(cmd);
            SP_GAME_MESSAGE move_msg = spGameMoveHandler(game,cmd.move);
            spDestroyMove(cmd.move);
            return move_msg;
        case SP_GET_MOVES:
            IS_VALID(cmd);
            spGameGetMovesHandler(game,cmd.tile);
            spDestroyMove(cmd.move);
            return 3;
        case SP_GINVALID_LINE:
            PRINT_INVALID_COMMAND;
            return 3;
    }
    return -1;
}

char* spGameUndoHandler(SPGame* game){
    char* msg = (char*)malloc(sizeof("Undo move for player black : <x,y> -> <w,z>\nUndo move for player black : <x,y> -> <w,z>\n"));
    if (game->settings->game_mode == 2) {
        strcpy(msg, "Undo command not available in 2 players mode\n");
        return msg;
    }
    else if (spQueueIsEmpty(game->history)) {
        strcpy(msg, "Empty history, move cannot be undone\n");
        return msg;
    }
    else {
        SPNode* prevBoard = spStackPop(game->history);
        memcpy(game->gameBoard, prevBoard->data, sizeof(game->gameBoard));
        strcpy(msg, prevBoard->msg);
        free(prevBoard);
        return msg;
    }
    return msg;
}

void spGameGetMovesHandler(SPGame* game , SPTile* tile) {
    if (!(IN_RANGE(tile->row, 0,8) && IN_RANGE(tile->coloumn, 0,8)))
        printf("Invalid position on the board\n");
    else if (game->gameBoard[tile->row][tile->coloumn] == SP_GAME_EMPTY_ENTRY ||
             getColor(game->gameBoard[tile->row][tile->coloumn]) != game->settings->curr_turn)
        printf("The specified position does not contain your piece\n");
    else {
        SPMovesList* mlst = spGameGetMoves(game, tile->row, tile->coloumn);
        for (int i = 0; i < mlst->actualSize; i++) {
            SPMove *m = spMovesListGetAt(mlst, i);
            if (game->settings->difficulty == SP_DIFF_EASY || game->settings->difficulty == SP_DIFF_NOOB)
                printf("<%d,%c>%s%s\n", m->dest->row + 1, m->dest->coloumn + 'A',
                       spGameTileIsThreatened(game,m) ? "*" : "",
                       (game->gameBoard[m->dest->row][m->dest->coloumn] != SP_GAME_EMPTY_ENTRY
                        && getColor(game->gameBoard[m->dest->row][m->dest->coloumn]) != getColor(game->gameBoard[tile->row][tile->coloumn])) ? "^" : ""); // check thretened and eaten
            else
                printf("<%d,%c>\n", m->dest->row + 1, m->dest->coloumn + 'A');
        }
    }
}

int spGameMoveHandler(SPGame* game , SPMove* move){
    if (!IN_RANGE(move->src->coloumn, 0,8) || !IN_RANGE(move->dest->coloumn, 0,8)
        || !IN_RANGE(move->src->row, 0,8) || !IN_RANGE(move->dest->row, 0,8)) {
        printf("Invalid position on the board\n");
        return 3;
    }
    else if (game->gameBoard[move->src->row][move->src->coloumn] == SP_GAME_EMPTY_ENTRY ||
             getColor(game->gameBoard[move->src->row][move->src->coloumn]) != game->settings->curr_turn) {
        printf("The specified position does not contain your piece\n");
        return 3;
    }
    else{
        SP_GAME_MESSAGE msg = spGameSetMove(game, move);
        if (msg == SP_GAME_INVALID_MOVE){
            printf("Illegal move\n");
            return 3;
        }
        else if (msg == SP_GAME_SUCCESS_TIE){
            printf("The game is Tied\n");
            return -1;
        }
        else if(msg == SP_GAME_SUCCESS_MATED){
            printf("Checkmate! %s player wins the game\n", game->settings->curr_turn == WHITE ? "white" : "black" );
            return -1;
        }
        else if(msg == SP_GAME_SUCCESS_CHECKED){
            printf("Check: %s King is threatened!\n", game->settings->curr_turn == BLACK ? "white" : "black" );
            changeColor(game);
            return 1;
        }
        else if(msg == SP_GAME_SUCCESS){
            changeColor(game);
            if (game->settings->game_mode == SP_MODE_1P)
                return spSetCPUMove(game, move);
            else
                return 1;
        }
    }
    return 1;


}

char* spGetTilePiece (char piece){
    switch (piece){
        case W_PAWN:
        case B_PAWN:
            return "pawn";
        case W_BISHOP:
        case B_BISHOP:
            return "bishop";
        case W_KNIGHT:
        case B_KNIGHT:
            return "knight";
        case W_ROOK:
        case B_ROOK:
            return "rook";
        case W_QUEEN:
        case B_QUEEN:
            return "queen";
        case W_KING:
        case B_KING:
            return "king";
    }
    return NULL;
}
int spSetNaiveCPUMove(SPGame* game){
    SPMove* a = spCreateMove(0,0,0,0);
    SPGame* minmaxGame = spGameCopy(game);
    if (a==NULL || minmaxGame == NULL)
        return 0;
    spMinimaxSuggestMove(minmaxGame, a);
    spGameDestroy(minmaxGame);
    spGameSetNaiveMove(game, a);
    printf("Computer: move %s at <%c,%c> to <%c,%c>\n",
           spGetTilePiece(game->gameBoard[a->dest->row][a->dest->coloumn]),
           a->src->row+'1', a->src->coloumn+'A',a->dest->row+'1', a->dest->coloumn+'A');
    if(spGameIsCheck(game) == (signed int)game->settings->p1_color)
        printf("Check!\n");
    changeColor(game);
    spDestroyMove(a);
    return 1;
}
int spSetCPUMove (SPGame* game,SPMove* move){
    SPNode* lastBoard = spStackPop(game->history);
    SPMove* a = spCreateMove(0,0,0,0);
    SPGame* minmaxGame = spGameCopy(game);
    if (a==NULL || minmaxGame == NULL || lastBoard == NULL)
        return 0;
    spMinimaxSuggestMove(minmaxGame, a);
    spGameDestroy(minmaxGame);
    spGameSetMove(game, a);
    free(spStackPop(game->history));
    char msg[sizeof("Undo move for player black : <x,y> -> <w,z>\nUndo move for player black : <x,y> -> <w,z>\n")];
    sprintf(msg, "Undo move for player %s : <%c,%c> -> <%c,%c>\nUndo move for player %s : <%c,%c> -> <%c,%c>\n",
            game->settings->p1_color == WHITE ? "white" : "black", move->src->row+'1', move->src->coloumn+'A',move->dest->row+'1', move->dest->coloumn+'A',
            game->settings->p1_color == BLACK ? "white" : "black", a->src->row+'1', a->src->coloumn+'A',a->dest->row+'1', a->dest->coloumn+'A');
    spQueueFullPush(game->history, lastBoard->data, msg);
    spDestroyMove(a);
    printf("Computer: move %s at <%c,%c> to <%c,%c>\n",
           spGetTilePiece(game->gameBoard[a->dest->row][a->dest->coloumn]),
           a->src->row+'1', a->src->coloumn+'A',a->dest->row+'1', a->dest->coloumn+'A');
    if(spGameIsCheck(game) == (signed int)(game->settings->p1_color))
        printf("Check!\n");
    changeColor(game);
    free(lastBoard);
    return 1;
}

SPGame* spGameStimulateMove(SPGame* game , SPMove* move) {
    SPGame* movedGame = spGameCopy(game);
    spGameSetNaiveMove(movedGame,move);
    return movedGame;
}

bool spGameTileIsThreatened(SPGame* game , SPMove* move){
    SPGame* movedGame = spGameStimulateMove(game,move);
    SPMovesList* mlst = spGameGetAllMoves(movedGame);
    for(int i = 0; i<mlst->actualSize; i++){
        SPMove* atk = spMovesListGetAt(mlst,i);
        if(getColor(movedGame->gameBoard[atk->src->row][atk->src->coloumn])
           != getColor(movedGame->gameBoard[move->dest->row][move->dest->coloumn])
           && atk->dest->row == move->dest->row && atk->dest->coloumn == move->dest->coloumn){
            spMovesListDestroy(mlst);
            spGameDestroy(movedGame);
            return true;
        }
    }
    spMovesListDestroy(mlst);
    spGameDestroy(movedGame);
    return false;

}

int spGameSaveHandler(SPGame *game, char *fpath){
    FILE *fp;
    int i,j;
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
        return 4;
    }
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);
    game->settings->curr_turn = (SP_USER_COLOR)line[15]-'0';
    fgets(line, sizeof(line), fp);
    game->settings->game_mode = (SP_GAME_MODE)line[12]-'0';
    if (game->settings->game_mode == 1) {
        fgets(line, sizeof(line), fp);
        game->settings->difficulty = (SP_GAME_DIFFICULTY)line[13] - '0';
        fgets(line, sizeof(line), fp);
        game->settings->p1_color = (SP_USER_COLOR)line[13] - '0';
    }
    fgets(line, sizeof(line), fp);
    for (i = 8; i > 0; i--){
        fgets(line, sizeof(line), fp);
        for (j = 0; j < 8; j++)
            game->gameBoard[i-1][j] = line[9+j];
    }
    fclose(fp);
    return 4;
}

SPGame* spGameCreateDef(){
    SPGame* game = (SPGame *) malloc(sizeof(SPGame));
    if(game == NULL) return NULL;	//puts("Error: malloc has failed");
    game->history = spQueueCreate(3);
    game->settings = defaultSettings(NULL);
    game->settings->curr_turn = SP_USER_COLOR_WHITE;
    spSetNewBoard(game);
    return game;
}

SPGame* spGameCreate(SPSettings* settings){
    SPGame* game = (SPGame *) malloc(sizeof(SPGame));
    if(game == NULL) return NULL;	//puts("Error: malloc has failed");
    game->history = spQueueCreate(3);
    if(settings == NULL)    game->settings = defaultSettings(NULL);
    else game ->settings = settings;
    game->settings->curr_turn = WHITE;
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

SP_GAME_MESSAGE spGameSetNaiveMove(SPGame* src, SPMove* move){
    src->gameBoard[move->dest->row][move->dest->coloumn] = src->gameBoard[move->src->row][move->src->coloumn];
    src->gameBoard[move->src->row][move->src->coloumn] = SP_GAME_EMPTY_ENTRY;
    return SP_GAME_SUCCESS;
}
SP_GAME_MESSAGE spGameSetMove(SPGame* src, SPMove* move){
    if (!spGameIsValidMove(src, move->src->row, move->src->coloumn, move->dest->row, move->dest->coloumn))
        return SP_GAME_INVALID_MOVE;
    SPGame* statusGame = spGameStimulateMove(src, move);
    char statusAfter = spGameIsCheck(statusGame);
    spGameDestroy(statusGame);
    if(statusAfter == SP_GAME_COLOR_BOTH || statusAfter == (signed int)src->settings->curr_turn)  return SP_GAME_INVALID_MOVE;
    if (src->history->actualSize == src->history->maxSize) {
        free(spQueuePop(src->history));
        spQueuePush(src->history, src->gameBoard);
    }
    else
        spQueuePush(src->history, src->gameBoard);
    src->gameBoard[move->dest->row][move->dest->coloumn] = src->gameBoard[move->src->row][move->src->coloumn];
    src->gameBoard[move->src->row][move->src->coloumn] = SP_GAME_EMPTY_ENTRY;
    changeColor(src);
    if(statusAfter == (signed int)src->settings->curr_turn) {
        if(spGameIsMate(src)){
            changeColor(src);
            return SP_GAME_SUCCESS_MATED;
        }
        changeColor(src);
        return SP_GAME_SUCCESS_CHECKED;
    }
    if(spGameIsTie(src)) return SP_GAME_SUCCESS_TIE;
    changeColor(src);
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
            (desRow - srcRow == -1 ||
             (desRow - srcRow == -2 && srcRow == 6 && src->gameBoard[srcRow - 2][srcCol] == SP_GAME_EMPTY_ENTRY))) {
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
                SPMove* newMove = spCreateMove(0,0,0,0);
                spMoveToMove(newMove, spMovesListGetAt(temp,i));
                spMovesListAddLast(moves, newMove);
            }
            spMovesListDestroy(temp);
        }
    }
    return moves;
}

bool spGameIsTie(SPGame* src) {
    SPMovesList* moves = spGameGetAllMoves(src);
    for(int i = 0 ; i<moves->actualSize; i++) {
        SPMove *move = spMovesListGetAt(moves, i);
        if (getColor(src->gameBoard[move->src->row][move->src->coloumn]) == src->settings->curr_turn) {
            SPGame *statusGame = spGameStimulateMove(src, move);
            char statusAfter = spGameIsCheck(statusGame);
            spGameDestroy(statusGame);
            if (!(statusAfter == SP_GAME_COLOR_BOTH || statusAfter == (signed int) src->settings->curr_turn)) {
                spMovesListDestroy(moves);
                return false;
            }
        }
    }
    spMovesListDestroy(moves);
    return true;
}

char spGameIsCheck(SPGame *src){
    bool flagB = false , flagW = false;
    SPMovesList* moves = spGameGetAllMoves(src);
    for(int i = 0 ; i<moves->actualSize; i++){
        SPMove* move = spMovesListGetAt(moves , i);
        SPTile* t =  move -> dest;
        char target = src->gameBoard[t->row][t->coloumn];
        if(target == B_KING)   flagB = true;
        else if(target == W_KING) flagW = true;
    }
    spMovesListDestroy(moves);
    if(flagW && flagB) return SP_GAME_COLOR_BOTH;
    if(flagB) return BLACK;
    if(flagW) return WHITE;
    return SP_GAME_EMPTY_ENTRY;
}

bool spGameIsMate(SPGame *src){
    SPMovesList* moves = spGameGetAllMoves(src);
    for(int i = 0 ; i<moves->actualSize; i++) {
        SPMove *move = spMovesListGetAt(moves, i);
        if (getColor(src->gameBoard[move->src->row][move->src->coloumn]) == src->settings->curr_turn){
            SPGame* statusGame = spGameStimulateMove(src, move);
            char statusAfter = spGameIsCheck(statusGame);
            spGameDestroy(statusGame);
            if(!(statusAfter == SP_GAME_COLOR_BOTH || statusAfter == (signed int)src->settings->curr_turn)) {
                spMovesListDestroy(moves);
                return false;
            }
        }
    }
    spMovesListDestroy(moves);
    return true;
}

SPGame* spGameCopy(SPGame* src){
    SPGame* cGame = (SPGame *) malloc(sizeof(SPGame));
    if(cGame == NULL) return NULL;	//puts("Error: malloc has failed");
    //cGame->history = spQueueCopy(src->history);
    cGame->history = spQueueCreate(3);
    cGame->settings = spSettingsCopy(src->settings);
    cGame->settings->curr_turn = src->settings->curr_turn;
    memcpy(cGame->gameBoard , src->gameBoard, sizeof(char[8][8]));
    return cGame;
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

