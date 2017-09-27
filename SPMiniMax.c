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

int spMinimaxRecCalc(SPGame *game, int alphaScore, int betaScore, int isMaxi, int diff, SPMove *bestMove) {
    int bestScore, nodeScore,i,j,k;
    SPMovesList* moveLst;
    bestScore = isMaxi ? alphaScore : betaScore;
    bool cutFlag = false;
    if (diff == 0) /*stop condition*/
        return spMinimaxScoring(game->gameBoard, BLACK);// game->settings->difficulty % 2 == 0 ? BLACK : WHITE);
    for (i = 0; i < SP_GAMEBOARD_SIZE && !cutFlag; i++) { /*move over board*/
        for (j = 0; j < SP_GAMEBOARD_SIZE && !cutFlag; j++) {
            if (getColor(game->gameBoard[i][j])==isMax(BLACK,WHITE) && game->gameBoard[i][j] != SP_GAME_EMPTY_ENTRY) {
                moveLst = spGameGetMoves(game, i, j);
                for (k = 0; k < moveLst->actualSize && !cutFlag; k++) {
                    if (spGameSetMove(game, spMovesListGetAt(moveLst,k)) == SP_GAME_INVALID_MOVE)
                        continue;
                    if (!spGameIsMate(game)) //swap if else
                        nodeScore = spMinimaxRecCalc(game, isMax(bestScore, alphaScore), isMax(betaScore, bestScore),
                                                     isMax(BLACK, WHITE), diff - 1, bestMove);
                    else{
                        cutFlag = true; nodeScore = isMax(INT_MAX, INT_MIN); }
                    spGameUndoHandler(game);
                    if (!isMaxi &&((bestScore==INT_MAX && nodeScore == INT_MAX) || bestScore > nodeScore)) {
                        UPDATE_SCORE();
                    }
                    if (isMaxi && ((bestScore==INT_MIN && nodeScore == INT_MIN) || bestScore < nodeScore)) {
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
    if (game->settings->curr_turn == WHITE)
        return spMinimaxRecCalc(game, INT_MIN, INT_MAX, BLACK, game->settings->difficulty, bestMove);
    else
        return spMinimaxRecCalc(game, INT_MIN, INT_MAX, WHITE, game->settings->difficulty, bestMove);
}