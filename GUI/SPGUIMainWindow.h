#ifndef GRAPHICS_SPCHESSGUIMAINWIN_H_
#define GRAPHICS_SPCHESSGUIMAINWIN_H_

#include "SPGUIBasics.h"
#define NUM_OF_MAIN_BUTTONS 3

// enum which represents events in main window
typedef enum {
	SPGUI_MAIN_EXIT,
	SPGUI_MAIN_NEW_GAME,
	SPGUI_MAIN_INVALID_ARGUMENT,
	SPGUI_MAIN_LOAD,
	SPGUI_MAIN_NONE,
	SPGUI_MAIN_QUIT
} SPGUI_MAIN_EVENT;

// Main window struct
typedef struct spguimainwindow_t {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	Button** buttons;

} SPGUIMainWindow;

/**
 * Creates a new main window
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new main window is returned.
 */
SPGUIMainWindow* spMainWindowCreate();

/**
 * Draws the given main window
 */
void spMainWindowDraw(SPGUIMainWindow* src);

/**
 * free the memory located by the given main window
 */
void spMainWindowDestroy(SPGUIMainWindow* src);

/**
 * The function handles an event that accured in the
 * main window, and returns the event that happened
 * for further handling (by the controller)
 *
 * @return
 * SPGUI_MAIN_EXIT - if the exit button have been pressed
 * SPGUI_MAIN_NEW_GAME - if the new-game button have been pressed
 * SPGUI_MAIN_INVALID_ARGUMENT - if an invalid argument have been given to the handler
 * SPGUI_MAIN_LOAD - if the load button have been pressed
 * SPGUI_MAIN_QUIT - if a quit have been requested
 * SPGUI_MAIN_NONE - if none of the above happened
 */
SPGUI_MAIN_EVENT spMainWindowEventHandler(SPGUIMainWindow *src, SDL_Event *event);

#endif /* GRAPHICS_SPCHESSGUIMAINWIN_H_ */
