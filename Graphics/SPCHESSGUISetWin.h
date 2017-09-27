/*
 * SPCHESSGUISetWin.h
 *
 *  Created on: 12 בספט׳ 2017
 *      Author: uri
 */

#ifndef GRAPHICS_SPCHESSGUISETWIN_H_
#define GRAPHICS_SPCHESSGUISETWIN_H_

#include "SPCHESSGUICommon.h"
#define NUM_OF_SET_BUTTONS 13

/*
 * enum represents set window events.
 */
typedef enum {
	SPCHESS_SET_GAME_MODE,
	SPCHESS_SET_DIFF,
	SPCHESS_SET_COLOR,
	SPCHESS_SET_START,
	SPCHESS_SET_BACK,
	SPCHESS_SET_INVALID_ARGUMENT,
	SPCHESS_SET_NONE,
	SPCHESS_SET_QUIT
} SPCHESS_SET_EVENT;
/*
 * structure represents a set window.
 */
typedef struct spchesssetwin_t {
	SDL_Window* setWindow;
	SDL_Renderer* setRenderer;
	int numOfBtns;
	Button** btns;
	SPGame* game;

} SPCHESSSetWin;
/*
 * standard set window functions: create destroy, draw,
 * handle events, hide and show.
 */
SPCHESSSetWin* spSetWindowCreate();
void spSetWindowDraw(SPCHESSSetWin* src);
void spSetWindowDestroy(SPCHESSSetWin* src);
void spSetWindowHide(SPCHESSSetWin* src);
void spSetWindowShow(SPCHESSSetWin* src);
SPCHESS_SET_EVENT spSetWindowHandleEvent(SPCHESSSetWin* src,
		SDL_Event* event);

#endif /* GRAPHICS_SPCHESSGUISETWIN_H_ */
