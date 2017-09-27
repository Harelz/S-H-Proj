#ifndef GRAPHICS_SPCHESSGUIMANAGER_H_
#define GRAPHICS_SPCHESSGUIMANAGER_H_

#include "SPCHESSGUIMainWin.h"
#include "SPCHESSGUISetWin.h"
#include "SPCHESSGUIGameWin.h"
#include "SPCHESSGUILoadWin.h"

/*
 * enum represents the current active window - in order
 * for the manager which functions to activate.
 */
typedef enum {
	SPCHESS_MAIN_WINDOW_ACTIVE,
	SPCHESS_GAME_WINDOW_ACTIVE,
	SPCHESS_LOAD_WINDOW_ACTIVE,
	SPCHESS_SET_WINDOW_ACTIVE,
	SPCHESS_NO_WINDOW
} SPCHESS_ACTIVE_WINDOW;


/*
 * main events of the gui
 */
typedef enum {
	SPCHESS_MANAGER_QUIT, SPCHESS_MANAGER_NONE,
} SPCHESS_MANAGER_EVENT;


/*
 * structure represents the manager - it has a param for each window and enums
 * represent the active and last windows.
 */
typedef struct spchessguimanager_t {
	SPCHESSMainWin* mainWin;
	SPCHESSSetWin* setWin;
	SPGameWin* gameWin;
	SPCHESSLoadWin* loadWin;
	SPCHESS_ACTIVE_WINDOW activeWin;
	SPCHESS_ACTIVE_WINDOW prevWin;
} SPCHESSGuiManager;


/*
 * standard manager functions: create, destroy, draw and handle events.
 */
SPCHESSGuiManager* spManagerCreate();
void spManagerDestroy(SPCHESSGuiManager* src);
void spManagerDraw(SPCHESSGuiManager* src, SDL_Event* event);
SPCHESS_MANAGER_EVENT spManagerHandleEvent(SPCHESSGuiManager* src,
		SDL_Event* event);


/*
 * functions handle the manager after a specific window
 * event (switch windows, close and create windows etc.)
 */
SPCHESS_MANAGER_EVENT handleManagerDueToMainEvent(SPCHESSGuiManager* src, SPCHESS_MAIN_EVENT event);
SPCHESS_MANAGER_EVENT handleManagerDueToSetEvent(SPCHESSGuiManager* src, SPCHESS_SET_EVENT event);
SPCHESS_MANAGER_EVENT handleManagerDueToGameEvent(SPCHESSGuiManager* src, SPCHESS_GAME_EVENT event);
SPCHESS_MANAGER_EVENT handleManagerDueToLoadEvent(SPCHESSGuiManager* src, SPCHESS_LOAD_EVENT event);

#endif /* GRAPHICS_SPCHESSGUIMANAGER_H_ */
