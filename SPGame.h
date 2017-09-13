#ifndef SPGAME_H_
#define SPGAME_H_
#include <stdbool.h>
#include <math.h>
#include "SPSettings.h"
#include "SPQueue.h"
#include "SPMoves.h"
#include <ctype.h>
/**
 * SPGame Summary:
 *
 * A container that represents a classic connect-4 game, a two players 6 by 7
 * board game (rows X columns). The container supports the following functions.
 *
 * spGameCreate           - Creates a new game board
 * spGameCopy             - Copies a game board
 * spGameDestroy          - Frees all memory resources associated with a game
 * spGameSetMove          - Sets a move on a game board
 * spGameIsValidMove      - Checks if a move is valid
 * spGameUndoPrevMove     - Undoes previous move made by the last player
 * spGamePrintBoard       - Prints the current board
 * spGameGetCurrentPlayer - Returns the current player
 *
 */

//Definitions
#define SP_GAMEBOARD_SIZE 8
#define SP_GAME_PLAYER_1_SYMBOL 'X'
#define SP_GAME_PLAYER_2_SYMBOL 'Y'
#define SP_GAME_TIE_SYMBOL '-'
#define SP_GAME_EMPTY_ENTRY '_'

//reference defines
#define WHITE 'w'
#define BLACK 's'
#define invColor(C) ((C)==WHITE? BLACK:WHITE)
#define getColor(P) (islower(P)? WHITE:BLACK)

#define W_PAWN 'm'
#define B_PAWN 'M'
#define W_BISHOP 'b'
#define B_BISHOP 'B'
#define W_ROOK 'r'
#define B_ROOK 'R'
#define W_KNIGHT 'n'
#define B_KNIGHT 'N'
#define W_QUEEN 'q'
#define B_QUEEN 'Q'
#define W_KING 'k'
#define B_KING 'K'
/* other reference defs
#define EMPTY ' '
#define CASTLE 'z' /*special mark for castling move*/
#define NORM '\0' /*mark for non-special moves*/

#define CONTINUE 'c'
#define TIE 't'
#define CHECK 'C'

#define inBoard(C,R) (0<=(C) && 0<=(R) && (C)<SP_GAMEBOARD_SIZE && (R)<SP_GAMEBOARD_SIZE)*/

typedef struct sp_game_t {
	char gameBoard[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE];
	int currentPlayer;
    Queue* history;
    SPSettings* settings;
} SPGame;

/**
 * Type used for returning error codes from game functions
 */
typedef enum sp_game_message_t {
	SP_GAME_INVALID_MOVE,
    SP_GAME_EMPTY_ENTRY_MOVE,
	SP_GAME_INVALID_ARGUMENT,
	SP_GAME_HISTORY,
	SP_GAME_SUCCESS,
//You may add any message you like
} SP_GAME_MESSAGE;

SPGame* spSetNewBoard(SPGame* src);
SPGame* spGameCreateDef();


/**
 * Creates a new game with a specified settings.
 * @settings - The game seetings.
 * @return
 * NULL if either a memory allocation failure occurs.
 * default settings new game if settings == NULL.
 * Otherwise, a new game instant is returned.
 */
SPGame* spGameCreate(SPSettings* settings);

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
SPGame* spGameCopy(SPGame* src);

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spGameDestroy(SPGame* src);

/**
 * Sets the next move in a given game by specifying column index. The
 * columns are 0-based and in the range [0,SP_GAMEBOARD_SIZE -1].
 *
 * @param src - The target game
 * @param col - The target column, the columns are 0-based
 * @return
 * SP_GAME_INVALID_ARGUMENT - if src is NULL or col is out-of-range
 * SP_GAME_INVALID_MOVE - if the given column is full.
 * SP_GAME_SUCCESS - otherwise
 */
SP_GAME_MESSAGE spGameSetMove(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);

/**
 * Checks if a disk can be put in the specified column.
 *
 * @param src - The source game
 * @param col - The specified column
 * @return
 * true  - if the a disc can be put in the target column
 * false - otherwise.
 */
bool spGameIsValidMove(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);

/**
 * Removes a disc that was put in the previous move and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * SP_GAME_INVALID_ARGUMENT - if src == NULL
 * SP_GAMEO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * SP_GAME_SUCCESS          - on success. The last disc that was put on the
 *                                 board is removed and the current player is changed
 */
SP_GAME_MESSAGE spGameUndoPrevMove(SPGame* src);

/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. The characters 'X' and 'O' are used to represent
 * the discs of player 1 and player 2, respectively.
 *
 * @param src - the target game
 * @return
 * SP_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_GAME_SUCCESS - otherwise
 *
 */
SP_GAME_MESSAGE spGamePrintBoard(SPGame* src);

/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_GAME_PLAYER_1_SYMBOL - if it's player one's turn
 * SP_GAME_PLAYER_2_SYMBOL - if it's player two's turn
 * SP_GAME_EMPTY_ENTRY     - otherwise
 */
char spGameGetCurrentPlayer(SPGame* src);

/**
* Checks if there's a winner in the specified game status. The function returns either
* SP_GAME_PLAYER_1_SYMBOL or SP_GAME_PLAYER_2_SYMBOL in case there's a winner, where
* the value returned is the symbol of the winner. If the game is over and there's a tie
* then the value SP_GAME_TIE_SYMBOL is returned. in any other case the null characters
* is returned.
* @param src - the source game
* @return
* SP_GAME_PLAYER_1_SYMBOL - if player 1 won
* SP_GAME_PLAYER_2_SYMBOL - if player 2 won
* SP_GAME_TIE_SYMBOL - If the game is over and there's a tie
* null character - otherwise
*/
char spCheckWinner(SPGame* src);

/**
 * Checks if the board is full in the specified game status.
 * @param src - the source game
 * @return
 * true - if game board is full
 * false - if game board isn't full
 */
bool fullBoard(SPGame* src);

bool checkValidStepForP(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);
bool checkValidStepForR(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);
bool checkValidStepForB(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);
bool checkValidStepForN(int srcRow , int srcCol , int desRow, int desCol);
bool checkValidStepForK(int srcRow , int srcCol , int desRow, int desCol);
bool checkValidStep(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);

#endif
