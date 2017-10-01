
#ifndef GRAPHICS_SPCHESSGUIGAMEWIN_H_
#define GRAPHICS_SPCHESSGUIGAMEWIN_H_

#include "SPGUIBasics.h"
#define NUM_OF_GAME_BUTTONS 6
#define NUM_OF_DIFF_PIECES 6
#define PANEL_OFFSET 200
#define GUI_BOARD_SIZE 650

// enum which represents events in game window
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

// Game window struct
typedef struct spguigamewindow_t {
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	Button** buttons;
	SDL_Texture* whitePieces[NUM_OF_DIFF_PIECES];
	SDL_Texture* blackPieces[NUM_OF_DIFF_PIECES];
	SDL_Texture* grid;
	SPGame* game;
	bool isSaved;
	int chosenPiece[2];
} SPGUIGameWindow;


/**
 * Creates a new game window with the SPGame given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new game window is returned.
 */
SPGUIGameWindow* spGameWindowCreate(SPGame* gameCopy);

/**
 * Draws the given settings window
 */
void spGameWindowDraw(SPGUIGameWindow* src, SDL_Event* event);

/**
 * free the memory located by the given game window
 */
void spGameWindowDestroy(SPGUIGameWindow* src);


/*
 * Draws the piece that located in the <row,col> on the board, on the GUI's chess grid.
 */
void drawPieceByEntry(SPGUIGameWindow* src, SDL_Rect rec, int row, int col);
/*
 * Builds and shows a pop-up message box which asks the user if he
 * wants to save the game before he leaves.
 */
int popUpSave();

/*
 * Coverts the mouse location on the gui chess grid to the row and column on the board.
 */
void computeLocFromGui(int loc[2]);

/**
 * The function handles an event that accured in the
 * game window, and returns the event that happened
 * for further handling (by the controller)
 *
 * @return
 * SPGUI_GAME_UNDO - if the undo button have been pressed
 * SPGUI_GAME_RESTART - if the restart button have been pressed
 * SPGUI_GAME_MAIN_MENU - if the main-menu button have been pressed
 * SPGUI_GAME_SAVE - if the save button have been pressed
 * SPGUI_GAME_EXIT - if the exit button have been pressed
 * SPGUI_GAME_INVALID_ARG - if an invalid argument have been given to the handler
 * SPGUI_GAME_LOAD - if the load button have been pressed
 * SPGUI_GAME_QUIT - if a quit have been requested
 * SPGUI_GAME_NONE - if none of the above happened
 * SPGUI_GAME_MOVE - if a move have been done
 * SPGUI_GAME_TIE - if tie have reached
 * SPGUI_GAME_PLAYER_1_CHECKMATE if checkmate have reached for player 1
 * SPGUI_GAME_PLAYER_2_CHECKMATE if checkmate have reached for player 2
 * SPGUI_GAME_PLAYER_1_CHECK if check have reached for player 1
 * SPGUI_GAME_PLAYER_2_CHECK if check have reached for player 2
 */
SPGUI_GAME_EVENT spGameWindowEventHandler(SPGUIGameWindow *src, SDL_Event *event);
/*
 * The function check The game status (check\checkmate\tie\none)\
 */
SPGUI_GAME_EVENT checkStatusForUserGui(SPGUIGameWindow* src);
/*
 * An util function for the handler which handles the buttons events.
 */
SPGUI_GAME_EVENT spGameButtonHandleEvent(SPGUIGameWindow *src, SDL_Event *event);
/*
 * pop-ups massage of the game status (if needed).
 */
SPGUI_GAME_EVENT popUpStatusAfterMove(SPGUI_GAME_EVENT msg, SPGUIGameWindow *src, SDL_Event *event);

#endif /* GRAPHICS_SPCHESSGUIGAMEWIN_H_ */
