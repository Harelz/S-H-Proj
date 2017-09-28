#ifndef GRAPHICS_SPCHESSGUIMAINWIN_H_
#define GRAPHICS_SPCHESSGUIMAINWIN_H_

#include "SPGUICommon.h"
#define NUM_OF_MAIN_BUTTONS 3
/*
 * enum represents main window events.
 */
typedef enum {
	SPGUI_MAIN_EXIT,
	SPGUI_MAIN_NEW_GAME,
	SPGUI_MAIN_INVALID_ARGUMENT,
	SPGUI_MAIN_LOAD,
	SPGUI_MAIN_NONE,
	SPGUI_MAIN_QUIT
} SPGUI_MAIN_EVENT;

/*
 * structure represents main window.
 */
typedef struct spchessmainwin_t {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	int numOfButtons;
	Button** buttons;

} SPGUIMainWindow;

/*
 * standard main window functions: create destroy, draw,
 * handle events, hide and show.
 */
SPGUIMainWindow* spMainWindowCreate();
void spMainWindowDraw(SPGUIMainWindow* src);
void spMainWindowDestroy(SPGUIMainWindow* src);
SPGUI_MAIN_EVENT spMainWindowEventHandler(SPGUIMainWindow *src, SDL_Event *event);


#endif /* GRAPHICS_SPCHESSGUIMAINWIN_H_ */
