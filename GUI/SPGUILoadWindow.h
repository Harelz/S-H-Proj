#ifndef GRAPHICS_SPCHESSGUILOADWIN_H_
#define GRAPHICS_SPCHESSGUILOADWIN_H_

#include "SPGUIBasics.h"
#define NUM_OF_LOAD_BUTTONS 7

/*
 * enum represents the diffrent load window events.
 */
typedef enum {
	SPGUI_LOAD_SLOT,
	SPGUI_LOAD_BACK,
	SPGUI_LOAD_INVALID_ARGUMENT,
	SPGUI_LOAD_NONE,
	SPGUI_LOAD_LOAD,
	SPGUI_LOAD_QUIT
} SPGUI_LOAD_EVENT;

/*
 * structur represents a lad window
 */
typedef struct spguiloadwindow_t {
	SDL_Window* loadWindow;
	SDL_Renderer* loadRenderer;
	Button** buttons;
	int slotPicked;
	SPGame* game;
} SPGUILoadWindow;


/*
 * standard load functions: createm destroy, draw, handle events, hide and show.
 */
SPGUILoadWindow* spLoadWindowCreate();
void spLoadWindowDestroy(SPGUILoadWindow* src);
void spLoadWindowDraw(SPGUILoadWindow* src);
SPGUI_LOAD_EVENT spLoadWindowEventHandler(SPGUILoadWindow *src, SDL_Event *event);

#endif /* GRAPHICS_SPCHESSGUILOADWIN_H_ */
