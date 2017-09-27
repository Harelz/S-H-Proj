#ifndef GRAPHICS_SPCHESSGUIGAMEWIN_H_
#define GRAPHICS_SPCHESSGUIGAMEWIN_H_

#include "SPCHESSGUICommon.h"
#include "../SPMiniMax.h"
#define NUM_OF_GAME_BUTTONS 6
#define NUM_OF_DIFF_PIECES 6
#define PANEL_OFFSET 200
#define GUI_BOARD_SIZE 650

/*
 * enum represents the diffrent game window events.
 */
typedef enum {
	SPCHESS_GAME_RESTART,
	SPCHESS_GAME_SAVE,
	SPCHESS_GAME_LOAD,
	SPCHESS_GAME_UNDO,
	SPCHESS_GAME_MAIN_MENU,
	SPCHESS_GAME_EXIT,
	SPCHESS_GAME_QUIT,
	SPCHESS_GAME_MOVE,
	SPCHESS_GAME_PLAYER_1_CHECK,
	SPCHESS_GAME_PLAYER_2_CHECK,
	SPCHESS_GAME_PLAYER_1_CHECKMATE,
	SPCHESS_GAME_PLAYER_2_CHECKMATE,
	SPCHESS_GAME_TIE,
	SPCHESS_GAME_NONE,
	SPCHESS_GAME_INVALID_ARG
} SPCHESS_GAME_EVENT;

/*
 * structure represents a game window. Beside regular params, it contains
 * flag indicates whether the game was saved,
 * and if there is a motion piece.
 */
typedef struct SPGamewin_t {
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	int numOfPanel;
	Button** panel;
	SDL_Texture* whitePieces[NUM_OF_DIFF_PIECES];
	SDL_Texture* blackPieces[NUM_OF_DIFF_PIECES];
	SDL_Texture* chessGrid;
	SPGame* game;
	bool isSaved;
	int chosenPiece[2];
} SPGameWin;

/*
 * Create game window functions.
 */
SPGameWin* spGameWindowCreate(SPGame* gameCopy);
/*
 * The function draw the relavent image in rect location according to the
 * piece at the i-th j-th cell on the game board.
 */
void drawPieceByEntry(SPGameWin* src, SDL_Rect rec, int i, int j);
/*
 * The functions shows a pop-up message box asking the user if he wants to save the game before exiting,
 * and returns the user choice.
 */
int popUpSave();
/*
 * The functions return true iff the current mouse location is on the chess board itself.
 */
bool isClickOnBoard(int x);

/*
 * The functions coverts pixel mouse location from
 * the gui game board to current position in the console mode.
 */
void computeLocFromGui(int loc[2]);

/*
 * standard window functions: draw, destroy, hide and show.
 */
void spGameWindowDraw(SPGameWin* src, SDL_Event* event);
void spGameWindowDestroy(SPGameWin* src);
void spGameWindowHide(SPGameWin* src);
void spGameWindowShow(SPGameWin* src);

/*
 * The handle events game window functions - act according to the event sent and the location.
 */
SPCHESS_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src,
		SDL_Event* event);
/*
 * The function check The game Status after a turn and return an event enum accordingly.
 */
SPCHESS_GAME_EVENT checkStatusForUserGui(SPGameWin* src);
/*
 * An helper function hadling only with panel events (restart, save, load etc.)
 */
SPCHESS_GAME_EVENT spPanelHandleEvent(SPGameWin* src, SDL_Event* event);
/*
 * The function shows the relavent pop-up (if needed) about the game state: check, checkmate or tie.
 */
SPCHESS_GAME_EVENT spStatusAfterMove(SPCHESS_GAME_EVENT msg,
		SPGameWin* src, SDL_Event* event);

#endif /* GRAPHICS_SPCHESSGUIGAMEWIN_H_ */
