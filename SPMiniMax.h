//
// Created by hoshri on 9/19/2017.
//

#ifndef CHESSPROG_MINIMAX_H
#define CHESSPROG_MINIMAX_H

#include "SPGame.h"
#include <limits.h>

#define isMax(a,b) (isMaxi ? (a) : (b))

int spMinimaxScoring(char board[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], SP_USER_COLOR color);

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm up to a
 * specified length given by maxDepth. The current game state doesn't change
 * The function maintains two values which are alpha and beta, and they symbolize the score
 * that maximize(for max player) and that minimize(for min player).
 * @param game - The current game
 * @param alphaScore - The value of the alpha, initially alpha is set to negative infinity
 * @param betaScore - The value of the beta, initially alpha is set to positive infinity
 * @param isMaxi - The number represent if we want to maximize the score or minimize the score
 * @param diff - The current depth of the recursion
 * @param bestMove - This struct will save the best move for the current player to make
 * @return - The function returns a score that symbolize the game state after the move will
 * be made, and update the param bestMove to the move the current plyaer need to make in his
 * next turn.
 */
int spMinimaxRecCalc(SPGame *game, int alphaScore, int betaScore, int isMaxi, int diff, SPMove *bestMove);

/**
 * The function calls the miniMaxRec function with the right values. That means in case of white player the
 * isMaxPlayer gets 0 cause we want to minimize score for the next player, and in the opposite case it's 1 because
 * we want to maximize the score. The scoreAlpha is set to INT_MIN, and scoreBeta is set to INT_MAX. We also pass
 * the depth - difficulty level which gets lower by 1 at each call of the miniMaxRec.
 * @param game - The source game
 * @param bestMove - The best move list
 * @return - The function calls the miniMaxRec function and the value is returned from there.
 */
int spMinimaxSuggestMove(SPGame *game, SPMove *bestMove);

#endif //CHESSPROG_MINIMAX_H