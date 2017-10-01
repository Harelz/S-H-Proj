#ifndef GRAPHICS_SPCHESSGUIMANAGER_H_
#define GRAPHICS_SPCHESSGUIMANAGER_H_

#include "SPGUIMainWindow.h"
#include "SPGUISettingsWindow.h"
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
	SPGUI_SETTINGS_WINDOW,
	SPGUI_NO_WINDOW
} SPGUI_WINDOW;

/*
 * main events of the gui
 */
typedef enum {
	SPGUI_CONTROLLER_EVENT_QUIT,
    SPGUI_CONTROLLER_EVENT_NONE,
} SPGUI_CONTROLLER_EVENT;

/*
 * structure represents the manager - it has a param for each window and enums
 * represent the active and last windows.
 */
typedef struct spguicontroller_t {
	SPGUIMainWindow* mainWindow;
	SPGUISettingsWindow* settingsWindow;
	SPGUIGameWindow* gameWindow;
	SPGUILoadWindow* loadWindow;
	SPGUI_WINDOW activeWindow;
	SPGUI_WINDOW previousWindow;
} SPGUIController;

/*
 * standard controller functions: create, destroy, draw and handle events.
 */
SPGUIController* spControllerCreate();
void spControllerDestroy(SPGUIController *src);
void spControllerDraw(SPGUIController *src, SDL_Event *event);
SPGUI_CONTROLLER_EVENT spControllerEventHandler(SPGUIController *src, SDL_Event *event);

/*
 * functions handle the controller after a specific window
 * event (switch windows, close and create windows etc.)
 */
SPGUI_CONTROLLER_EVENT spControllerHandleMainEvent(SPGUIController *src, SPGUI_MAIN_EVENT event);
SPGUI_CONTROLLER_EVENT spControllerHandleSetEvent(SPGUIController *src, SPGUI_SETTINGS_EVENT event);
SPGUI_CONTROLLER_EVENT spControllerHandleGameEvent(SPGUIController *src, SPGUI_GAME_EVENT event);
SPGUI_CONTROLLER_EVENT spControllerHandleLoadEvent(SPGUIController *src, SPGUI_LOAD_EVENT event);

int MainGUIManager();

#endif /* GRAPHICS_SPCHESSGUIMANAGER_H_ */
