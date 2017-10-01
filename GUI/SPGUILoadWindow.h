#ifndef GRAPHICS_SPCHESSGUILOADWIN_H_
#define GRAPHICS_SPCHESSGUILOADWIN_H_

#include "SPGUIBasics.h"
#define NUM_OF_LOAD_BUTTONS 7

// enum which represents events in load window
typedef enum {
	SPGUI_LOAD_SLOT,
	SPGUI_LOAD_BACK,
	SPGUI_LOAD_INVALID_ARGUMENT,
	SPGUI_LOAD_NONE,
	SPGUI_LOAD_LOAD,
	SPGUI_LOAD_QUIT
} SPGUI_LOAD_EVENT;

// Load window struct
typedef struct spguiloadwindow_t {
	SDL_Window* loadWindow;
	SDL_Renderer* loadRenderer;
	Button** buttons;
	int slotPicked;
	SPGame* game;
} SPGUILoadWindow;


/**
 * Creates a new load window
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new load window is returned.
 */
SPGUILoadWindow* spLoadWindowCreate();

/**
 * Draws the given load window
 */
void spLoadWindowDestroy(SPGUILoadWindow* src);

/**
 * free the memory located by the given load window
 */
void spLoadWindowDraw(SPGUILoadWindow* src);

/**
 * The function handles an event that accured in the
 * load window, and returns the event that happened
 * for further handling (by the controller)
 *
 * @return
 * SPGUI_LOAD_SLOT - if the one of the save slot buttons have been pressed
 * SPGUI_LOAD_BACK - if the back button have been pressed
 * SPGUI_LOAD_INVALID_ARGUMENT - if an invalid argument have been given to the handler
 * SPGUI_LOAD_LOAD - if the load button have been pressed
 * SPGUI_LOAD_QUIT - if a quit have been requested
 * SPGUI_LOAD_NONE - if none of the above happened
 */
SPGUI_LOAD_EVENT spLoadWindowEventHandler(SPGUILoadWindow *src, SDL_Event *event);

#endif /* GRAPHICS_SPCHESSGUILOADWIN_H_ */
