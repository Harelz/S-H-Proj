
#ifndef GRAPHICS_SPCHESSGUIGAMEWIN_H_
#define GRAPHICS_SPCHESSGUIGAMEWIN_H_

#include "SPGUIBasics.h"
#define NUM_OF_GAME_BUTTONS 6
#define NUM_OF_DIFF_PIECES 6
#define PANEL_OFFSET 200
#define GUI_BOARD_SIZE 650

/*
 * enum represents the diffrent game window events.
 */
typedef enum {
	SPGUI_GAME_RESTART,
	SPGUI_GAME_SAVE,
	SPGUI_GAME_LOAD,
	SPGUI_GAME_UNDO,
	SPGUI_GAME_MAIN_MENU,
	SPGUI_GAME_EXIT,
	SPGUI_GAME_QUIT,
	SPGUI_GAME_MOVE,
	SPGUI_GAME_PLAYER_1_CHECK,
	SPGUI_GAME_PLAYER_2_CHECK,
	SPGUI_GAME_PLAYER_1_CHECKMATE,
	SPGUI_GAME_PLAYER_2_CHECKMATE,
	SPGUI_GAME_TIE,
	SPGUI_GAME_NONE,
	SPGUI_GAME_INVALID_ARG
} SPGUI_GAME_EVENT;

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
} SPGUIGameWindow;

/*
 * Create game window functions.
 */
SPGUIGameWindow* spGameWindowCreate(SPGame* gameCopy);
/*
 * The function draw the relavent image in rect location according to the
 * piece at the i-th j-th cell on the game board.
 */
void drawPieceByEntry(SPGUIGameWindow* src, SDL_Rect rec, int i, int j);
/*
 * The functions shows a pop-up message box asking the user if he wants to save the game before exiting,
 * and returns the user choice.
 */
int popUpSave();
/*
 * The functions return true iff the current mouse location is on the chess board itself.
 */

/*
 * The functions coverts pixel mouse location from
 * the gui game board to current position in the console mode.
 */
void computeLocFromGui(int loc[2]);

/*
 * standard window functions: draw, destroy, hide and show.
 */
void spGameWindowDraw(SPGUIGameWindow* src, SDL_Event* event);
void spGameWindowDestroy(SPGUIGameWindow* src);

/*
 * The handle events game window functions - act according to the event sent and the location.
 */
SPGUI_GAME_EVENT spGameWindowEventHandler(SPGUIGameWindow *src,
										  SDL_Event *event);
/*
 * The function check The game Status after a turn and return an event enum accordingly.
 */
SPGUI_GAME_EVENT checkStatusForUserGui(SPGUIGameWindow* src);
/*
 * An helper function hadling only with panel events (restart, save, load etc.)
 */
SPGUI_GAME_EVENT spPanelHandleEvent(SPGUIGameWindow* src, SDL_Event* event);
/*
 * The function shows the relavent pop-up (if needed) about the game state: check, checkmate or tie.
 */
SPGUI_GAME_EVENT spStatusAfterMove(SPGUI_GAME_EVENT msg,
		SPGUIGameWindow* src, SDL_Event* event);

#endif /* GRAPHICS_SPCHESSGUIGAMEWIN_H_ */
