#ifndef GRAPHICS_SPCHESSGUIMAINWIN_H_
#define GRAPHICS_SPCHESSGUIMAINWIN_H_

#include "SPCHESSGUICommon.h"
#define NUM_OF_MAIN_BUTTONS 3
/*
 * enum represents main window events.
 */
typedef enum {
	SPCHESS_MAIN_EXIT,
	SPCHESS_MAIN_NEW_GAME,
	SPCHESS_MAIN_INVALID_ARGUMENT,
	SPCHESS_MAIN_LOAD,
	SPCHESS_MAIN_NONE,
	SPCHESS_MAIN_QUIT
} SPCHESS_MAIN_EVENT;

/*
 * structure represents main window.
 */
typedef struct spchessmainwin_t {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	int numOfBtns;
	Button** btns;

} SPCHESSMainWin;

/*
 * standard main window functions: create destroy, draw,
 * handle events, hide and show.
 */
SPCHESSMainWin* spMainWindowCreate();
void spMainWindowDraw(SPCHESSMainWin* src);
void spMainWindowDestroy(SPCHESSMainWin* src);
void spMainWindowHide(SPCHESSMainWin* src);
void spMainWindowShow(SPCHESSMainWin* src);
SPCHESS_MAIN_EVENT spMainWindowHandleEvent(SPCHESSMainWin* src, SDL_Event* event);


#endif /* GRAPHICS_SPCHESSGUIMAINWIN_H_ */
