#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

/**
* Calculating a node in the Minimax Tree
*
* @param currentGame - The current game state
* @param nodeDepth - The depth from this node in the miniMax algorithm
* @param imax - boolean set to true if need to maximize children score, false to minimize
* @return
* score of node in the Minmax Tree
*/
int spMinimaxNodeCalc(SPFiarGame* currentGame,
	unsigned int nodeDepth, bool imax);


#endif