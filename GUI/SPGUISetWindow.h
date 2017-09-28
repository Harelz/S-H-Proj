/*
 * SPCHESSGUISetWin.h
 *
 *  Created on: 12 בספט׳ 2017
 *      Author: uri
 */

#ifndef GRAPHICS_SPCHESSGUISETWIN_H_
#define GRAPHICS_SPCHESSGUISETWIN_H_

#include "SPGUICommon.h"
#define NUM_OF_SET_BUTTONS 13

/*
 * enum represents set window events.
 */
typedef enum {
	SPGUI_SET_GAME_MODE,
	SPGUI_SET_DIFF,
	SPGUI_SET_COLOR,
	SPGUI_SET_START,
	SPGUI_SET_BACK,
	SPGUI_SET_INVALID_ARGUMENT,
	SPGUI_SET_NONE,
	SPGUI_SET_QUIT
} SPGUI_SET_EVENT;
/*
 * structure represents a set window.
 */
typedef struct spchesssetwin_t {
	SDL_Window* setWindow;
	SDL_Renderer* setRenderer;
	int numOfBtns;
	Button** btns;
	SPGame* game;

} SPGUISetWindow;
/*
 * standard set window functions: create destroy, draw,
 * handle events, hide and show.
 */
SPGUISetWindow* spSetWindowCreate();
void spSetWindowDraw(SPGUISetWindow* src);
void spSetWindowDestroy(SPGUISetWindow* src);
void spSetWindowShow(SPGUISetWindow* src);
SPGUI_SET_EVENT spSetWindowEventHandler(SPGUISetWindow *src,
										SDL_Event *event);

#endif /* GRAPHICS_SPCHESSGUISETWIN_H_ */
