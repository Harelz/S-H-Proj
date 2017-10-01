#ifndef SPGAME_H_
#define SPGAME_H_

#include <math.h>
#include "SPSettings.h"
#include "SPQueue.h"

/** bool which sets to true if the console mode is running and false if the gui mode is running.
 * the functions are using this indicator to know if they should print massages to the console or not.
 */
bool isConsole;

//Definitions
#define SP_GAMEBOARD_SIZE 8
#define SP_GAME_EMPTY_ENTRY '_'
#define SP_GAME_COLOR_BOTH '&'

//reference defines
#define WHITE SP_USER_COLOR_WHITE
#define BLACK SP_USER_COLOR_BLACK
/**
 * Returns the opposite to the current player of the specified game.
 * @param C - a color
 * @return
 * WHITE - if it's black
 * BLACK - if it's white
 */
#define invColor(C) ((C)==WHITE? BLACK:WHITE)
/**
 * Returns the current player of the specified piece. assuming it is a game piece.
 * @param P - a color
 * @return
 * WHITE - if it's a white turn
 * BLACK - if it's a black turn
 */
#define getColor(P) (islower(P)? WHITE: BLACK)
/**
 * Inverts the current player of the specified game.
 * @param GAME - the source game
 * @return
 */
#define changeColor(GAME) ((GAME)->settings->curr_turn = invColor((GAME)->settings->curr_turn))

/**
 * Defines for all pieces, self explanatory.
 */
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

typedef struct sp_game_t {
	char gameBoard[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE];
	Queue* history;
	SPSettings* settings;
} SPGame;

/**
 * Type used for returning error codes from game functions
 */
typedef enum sp_game_message_t {
	SP_GAME_INVALID_MOVE,
	SP_GAME_INVALID_ARGUMENT,
	SP_GAME_SUCCESS,
	SP_GAME_SUCCESS_TIE,
	SP_GAME_SUCCESS_CHECKED,
	SP_GAME_SUCCESS_MATED,
} SP_GAME_MESSAGE;

/**
 * Handles a SPGameCommand given on the game
 *
 * @return
 * 0 when game restarting is needed according to the command.
 * -1 if terminating the program is needed
 * 1 if the handler made an undo proccess
 * 3 otherwise
 */
int spGameHandler(SPGame *game, SPGameCommand cmd);

/**
 * Handles the save proccess, save the current
 * game status into a new xml file in the path given.
 *
 * @return
 * 0 if the path cant be reached
 * 1 otherwise (success)
 */
int spGameSaveHandler(SPGame *game, char *fpath);

/**
 * Handles the load proccess, loads a game that have been saved
 * previously from a xml file in the path given, into the SPGame.
 *
 * @return
 * 0 if the path cant be reached
 * 1 otherwise (success)
 */
int spGameLoadHandler(SPGame *game, char *fpath);

/**
 * resets the give game's game board to the starting form
 */
void spSetNewBoard(SPGame* game);

/**
 * Creates a new game with default settingss values.
 * @return
 * NULL if either a memory allocation failure occurs.
 * default settings new game if settings == NULL.
 * Otherwise, a new game instant is returned.
 */
SPGame* spGameCreateDef();


/**
 * copies the game and stimulate the move given on the copied version.
 *
 * @return
 * NULL if either a memory allocation failure occurs.
 * Otherwise, returns the copied version after the move have been utilized.
 */
SPGame* spGameStimulateMove(SPGame* game , SPMove* move);

/**
 * Handles the undo proccess, tries to undo user's move.
 *
 * @return
 * returns a string which contains a massage which should
 * be printed according to the undo proccess success/failure.
 */
char* spGameUndoHandler(SPGame* game);

/**
 * Handles the set move proccess, tries to set the move given in the game,
 * and prints massage to the console accordingly
 *
 * @return
 * -1 if checkmate or tie have reached after the set move proccess
 * 1 if check have reached or if the set move proccess successed
 * 3 if the set move proccess wasn't successful
 */
int spGameMoveHandler(SPGame* game , SPMove* move);

/**
 * Handles the get moves proccess, on a given tile, the function
 * searches for an available moves from this tile, while taking
 * in count the specific piece the tile contains and if the piece
 * will 'eat' another piece or will be threatened by another one
 * if the move will be performed. the handler prints the results
 * of the calculation to the console
 */
void spGameGetMovesHandler(SPGame* game , SPTile* tile);

/**
 * checks if the game have reached to an checkmate status
 *
 * @return
 * true if checkmate reached
 * false otherwise
 */
bool spGameIsMate(SPGame *src);


/**
 * checks if the game have reached to an tie status
 *
 * @return
 * true if tie reached
 * false otherwise
 */
bool spGameIsTie(SPGame* src);


/**
 * checks if the game have reached to an check status
 *
 * @return
 * true if check reached
 * false otherwise
 */
char spGameIsCheck(SPGame* src);

/**
 * the function get the user's move (the have previously done) and
 * making a CPU move suggested by the Minimax algorithm for the game's
 * difficulty. the CPU's move and the user's move are saved in the
 * history queue as a massage. the function handles the console
 * printing needed after the CPU move.
 *
 * @return
 * 0 if either a memory allocation failure occurs.
 * -1 if checkmate or tie accured
 * 1 otherwise
 */
int spSetCPUMove (SPGame* game,SPMove* move);


/**
 * Creates a new game with a specified settings.
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
 */
void spGameDestroy(SPGame* src);

/**
 * Sets the next move in a given game by specifying a SPMove object.
 *
 * @return
 * SP_GAME_INVALID_MOVE - if the given move is invalid.
 * SP_GAME_SUCCESS - otherwise
 */
SP_GAME_MESSAGE spGameSetMove(SPGame* src, SPMove* move);

/**
 * Checks if the move '<srcRow,srcCol> to <desRow,desCol>' is a valid move
 *
 * @return
 * true  - if the move is valid
 * false - otherwise.
 */
bool spGameIsValidMove(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);

/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing.
 *
 * @return
 * SP_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_GAME_SUCCESS - otherwise
 *
 */
SP_GAME_MESSAGE spGamePrintBoard(SPGame* src);

/**
 * On success, the function returns list of all the valid moves for
 * the piece that sits in the <row,col> given
 *
 * @return SPMovesList as mentioned
 *
 */
SPMovesList* spGameGetMoves(SPGame* src , int row , int col);

/**
 * On success, the function returns list of all the valid
 * moves in the current game
 *
 * @return SPMovesList as mentioned
 */
SPMovesList* spGameGetAllMoves(SPGame* src);
// checks if moving from <srcRow,srcCol> to <desRow,desCol> is valid for a pawn
bool checkValidStepForM(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);

// checks if moving from <srcRow,srcCol> to <desRow,desCol> is valid for a rook
bool checkValidStepForR(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);

// checks if moving from <srcRow,srcCol> to <desRow,desCol> is valid for a bishop
bool checkValidStepForB(SPGame* src, int srcRow , int srcCol , int desRow, int desCol);

// checks if moving from <srcRow,srcCol> to <desRow,desCol> is valid for a knight
bool checkValidStepForN(int srcRow , int srcCol , int desRow, int desCol);

// checks if moving from <srcRow,srcCol> to <desRow,desCol> is valid for a king
bool checkValidStepForK(int srcRow , int srcCol , int desRow, int desCol);

// returns a string contains the specified piece's name
char* spGetPiecesName(char piece);

//sets the move given into the game board NAIVELY (only rewrites the pieces location)
void spGameSetNaiveMove(SPGame* src, SPMove* move);

/**
 * the function sets a CPU move suggested by the Minimax algorithm for the
 * game's difficulty. neither printing or history saving are handled.
 *
 * @return
 * 0 if either a memory allocation failure occurs.
 * -1 if checkmate or tie accured
 * 1 otherwise
 */
int spSetNaiveCPUMove(SPGame* game);

/**
 * stimulate the move given and checks if the piece that
 * moved is threatened by the enemy after making the move
 *
 * @return
 * true if its threatened
 * false otherwise
 */
bool spGameMoveIsThreatened(SPGame *game, SPMove *move);

#endif
