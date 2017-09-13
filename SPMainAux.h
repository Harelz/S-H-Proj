#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#define SIZE_OF_HISTORY 20

#include "SPParser.h" // include SPGame.h which includes SPArrayList
#include "SPMiniMax.h"


/**
 * Creates a new game, while handling malloc errors in the creation
 * and printing the starting massage.
 * @return NULL if malloc error occurred, else return a new game.
 */
SPGame* StartNewGame();

/**
 * check if the game is over. if so, the function prints the game's winner or a
 * tie massage
 * @param game - the game which will be checked
 * @return 1 - if the game is over , 0 - otherwise
 */
int handleIfOver(SPGame* game);


/**
 * Executes a given command on a game with a specified difficulty level.
 * @param
 * game - the game that the command will be executed on.
 * difficulty - the difficulty level of the game.
 * cmd - the command that will be executed on the game.
 * @return
 * NULL - if a the input was a 'quit' command.
 * otherwise, the function returns the game after executing the command.
 */
SPGame* ExecuteCmd(SPGame* game , SPSettingCommand cmd , int difficulty);

#endif
