#include "SPMiniMax.h"
/**
* Given a game state, this function evaluates the score of the state
* using the scoring function described on the PDF.
*
* @param currentGame - The current game state
* @return
* -1 if either currentGame is NULL
* On success the function returns a number between [0,SP_GAMEBOARD_SIZE -1]
* which is the best move for the current player.
**/
int spMinimaxScoring(char board[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], SP_USER_COLOR color) {
    char c;
    int i,j, whiteScore = 0, blackScore = 0, boardScore = 0;
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

int spMinimaxRecCalc(SPGame* game, int alphaScore, int betaScore, int isMaxi, int diff, SPMove *bestMove) {
    int bestScore, nodeScore,i,j,k;
    SPMovesList* moveLst;
    bestScore = isMaxi ? alphaScore : betaScore;
    bool cutFlag = false;
    if (diff == 0)
        return spMinimaxScoring(game->gameBoard, BLACK);
    for (i = 0; i < SP_GAMEBOARD_SIZE && !cutFlag; i++) {
        for (j = 0; j < SP_GAMEBOARD_SIZE && !cutFlag; j++) {
            if (getColor(game->gameBoard[i][j])==game->settings->curr_turn && game->gameBoard[i][j] != SP_GAME_EMPTY_ENTRY) {
                moveLst = spGameGetMoves(game, i, j);
                for (k = 0; k < moveLst->actualSize && !cutFlag; k++) {
                    SP_GAME_MESSAGE msg = spGameSetMove(game, spMovesListGetAt(moveLst,k));
                    if (msg == SP_GAME_INVALID_MOVE)
                        continue;
                    changeColor(game);
                    if (!(msg == SP_GAME_SUCCESS_MATED))
                        nodeScore = spMinimaxRecCalc(game, isMax(bestScore, alphaScore), isMax(betaScore, bestScore),
                                                     isMax(BLACK, WHITE), diff - 1, bestMove);
                    else{
                        cutFlag = true; nodeScore = isMax(INT_MAX, INT_MIN); }
                    free(spGameUndoHandler(game));
                    changeColor(game);
                    if (!isMaxi &&(defMove || bestScore > nodeScore)) {
                        UPDATE_SCORE();
                    }
                    if (isMaxi && (defMove || bestScore < nodeScore)) {
                        UPDATE_SCORE();
                    }
                }
                spMovesListDestroy(moveLst);
            }
            if (isMax(betaScore,bestScore) <= isMax(bestScore,alphaScore))
                cutFlag = true;
        } }
    return bestScore;
}

int spMinimaxSuggestMove(SPGame *game, SPMove *bestMove) {
    SPGame* game_cpy = spGameCopy(game);
    if (!game_cpy)
        return -1;
    HARD_SIZE();
    int stat;
    if (game->settings->curr_turn == WHITE) {
        stat = spMinimaxRecCalc(GAME_PACK_WHITE);
        spGameDestroy(game_cpy);
        return stat;
    }
    else{
        stat = spMinimaxRecCalc(GAME_PACK_BLACK);
        spGameDestroy(game_cpy);
        return stat;
    }
}
