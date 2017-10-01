#ifndef GRAPHICS_SPCHESSGUIMANAGER_H_
#define GRAPHICS_SPCHESSGUIMANAGER_H_

#include "SPGUIMainWindow.h"
#include "SPGUISettingsWindow.h"
#include "SPGUIGameWindow.h"
#include "SPGUILoadWindow.h"

// enum which represents all the gui windows
typedef enum {
	SPGUI_MAIN_WINDOW,
	SPGUI_GAME_WINDOW,
	SPGUI_LOAD_WINDOW,
	SPGUI_SETTINGS_WINDOW,
	SPGUI_NO_WINDOW
} SPGUI_WINDOW;

// enum which represents controller events
typedef enum {
	SPGUI_CONTROLLER_EVENT_QUIT,
    SPGUI_CONTROLLER_EVENT_NONE,
} SPGUI_CONTROLLER_EVENT;

// The Controller struct
typedef struct spguicontroller_t {
	SPGUIMainWindow* mainWindow;
	SPGUISettingsWindow* settingsWindow;
	SPGUIGameWindow* gameWindow;
	SPGUILoadWindow* loadWindow;
	SPGUI_WINDOW activeWindow;
	SPGUI_WINDOW previousWindow;
} SPGUIController;

/**
 * Creates a Controller with new main, load, settings, and game windows,
 * and sets the main window as the active one.
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new Controller is returned.
 */
SPGUIController* spControllerCreate();

/**
 * free the memory which located by the controller
 */
void spControllerDestroy(SPGUIController *src);

/**
 * Draws the controller's active window
 */
void spControllerDrawActiveWindow(SPGUIController *src, SDL_Event *event);

/**
 * handles the event given , according to the current's active window
 *
 * @return
 * SPGUI_CONTROLLER_EVENT_QUIT - if the handler decides that terminate is needed.
 * SPGUI_CONTROLLER_EVENT_NONE - otherwise.
 */
SPGUI_CONTROLLER_EVENT spControllerEventHandler(SPGUIController *src, SDL_Event *event);

/**
 * handles the event given , if the active window is main window
 *
 * @return
 * SPGUI_CONTROLLER_EVENT_QUIT - if the handler decides that terminate is needed.
 * SPGUI_CONTROLLER_EVENT_NONE - otherwise.
 */
SPGUI_CONTROLLER_EVENT spControllerHandleMainEvent(SPGUIController *src, SPGUI_MAIN_EVENT event);

/**
 * handles the event given , if the active window is settings window
 *
 * @return
 * SPGUI_CONTROLLER_EVENT_QUIT - if the handler decides that terminate is needed.
 * SPGUI_CONTROLLER_EVENT_NONE - otherwise.
 */
SPGUI_CONTROLLER_EVENT spControllerHandleSettingsEvent(SPGUIController *src, SPGUI_SETTINGS_EVENT event);

/**
 * handles the event given , if the active window is game window
 *
 * @return
 * SPGUI_CONTROLLER_EVENT_QUIT - if the handler decides that terminate is needed.
 * SPGUI_CONTROLLER_EVENT_NONE - otherwise.
 */
SPGUI_CONTROLLER_EVENT spControllerHandleGameEvent(SPGUIController *src, SPGUI_GAME_EVENT event);

/**
 * handles the event given , if the active window is load window
 *
 * @return
 * SPGUI_CONTROLLER_EVENT_QUIT - if the handler decides that terminate is needed.
 * SPGUI_CONTROLLER_EVENT_NONE - otherwise.
 */
SPGUI_CONTROLLER_EVENT spControllerHandleLoadEvent(SPGUIController *src, SPGUI_LOAD_EVENT event);

//Main gui program - basicly opens and closes SDL, and infinite looping with the controller's handler
int MainGUIController();

#endif /* GRAPHICS_SPCHESSGUIMANAGER_H_ */
