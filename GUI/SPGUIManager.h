#ifndef GRAPHICS_SPCHESSGUIMANAGER_H_
#define GRAPHICS_SPCHESSGUIMANAGER_H_

#include "SPGUIMainWindow.h"
#include "SPGUISetWindow.h"
#include "SPGUIGameWindow.h"
#include "SPGUILoadWindow.h"

/*
 * enum represents windows - in order
 * for the manager which functions to activate.
 */
typedef enum {
	SPGUI_MAIN_WINDOW,
	SPGUI_GAME_WINDOW,
	SPGUI_LOAD_WINDOW,
	SPGUI_SET_WINDOW,
	SPGUI_NO_WINDOW
} SPGUI_WINDOW;

/*
 * main events of the gui
 */
typedef enum {
	SPGUI_MANAGER_EVENT_QUIT,
    SPGUI_MANAGER_EVENT_NONE,
} SPGUI_MANAGER_EVENT;

/*
 * structure represents the manager - it has a param for each window and enums
 * represent the active and last windows.
 */
typedef struct spchessguimanager_t {
	SPGUIMainWindow* mainWindow;
	SPGUISetWindow* setWindow;
	SPGUIGameWindow* gameWindow;
	SPGUILoadWindow* loadWindow;
	SPGUI_WINDOW activeWindow;
	SPGUI_WINDOW prevWindow;
} SPGUIManager;

/*
 * standard manager functions: create, destroy, draw and handle events.
 */
SPGUIManager* spManagerCreate();
void spManagerDestroy(SPGUIManager* src);
void spManagerDraw(SPGUIManager* src, SDL_Event* event);
SPGUI_MANAGER_EVENT spManagerEventHandler(SPGUIManager *src, SDL_Event *event);

/*
 * functions handle the manager after a specific window
 * event (switch windows, close and create windows etc.)
 */
SPGUI_MANAGER_EVENT spManagerHandleMainEvent(SPGUIManager *src, SPGUI_MAIN_EVENT event);
SPGUI_MANAGER_EVENT spManagerHandleSetEvent(SPGUIManager *src, SPGUI_SET_EVENT event);
SPGUI_MANAGER_EVENT spManagerHandleGameEvent(SPGUIManager *src, SPGUI_GAME_EVENT event);
SPGUI_MANAGER_EVENT spManagerHandleLoadEvent(SPGUIManager *src, SPGUI_LOAD_EVENT event);

// Main Gui Handler
int MainGUIManager();

#endif /* GRAPHICS_SPCHESSGUIMANAGER_H_ */
