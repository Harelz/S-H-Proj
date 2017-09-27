#include "SPCHESSGUIMainWin.h"

SPCHESSMainWin* spMainWindowCreate() {
	SPCHESSMainWin* res = NULL;
	res = (SPCHESSMainWin*) calloc(sizeof(SPCHESSMainWin), sizeof(char));
	if (res == NULL)
		return NULL;

	res->mainWindow = SDL_CreateWindow("CHESS GAME: Main Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);

	if (res->mainWindow == NULL) {
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL) {
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->numOfBtns = NUM_OF_MAIN_BUTTONS;
	const char* activeImages[NUM_OF_MAIN_BUTTONS] = { ACT_IMG(new_game), ACT_IMG(
			load), ACT_IMG(exit) };

	const char* inactiveImages[NUM_OF_MAIN_BUTTONS] = {
			INACT_IMG(new_game), INACT_IMG(load),
			INACT_IMG(exit) };

	int xBtns[NUM_OF_MAIN_BUTTONS] = { 125, 125, 125 };
	int yBtns[NUM_OF_MAIN_BUTTONS] = { 63, 176, 289 };
	bool visible[NUM_OF_MAIN_BUTTONS] = { true, true, true };
	bool active[NUM_OF_MAIN_BUTTONS] = { true, true, true };
	SPCHESS_BUTTON_TYPE types[NUM_OF_MAIN_BUTTONS] = { BUTTON_MAIN_NEW_GAME,
			BUTTON_MAIN_LOAD, BUTTON_MAIN_EXIT };

	res->btns = createButtons(res->mainRenderer, activeImages, inactiveImages,
			xBtns, yBtns, visible, active, types, res->numOfBtns);

	if (res->btns == NULL) {
		SDL_DestroyRenderer(res->mainRenderer);
		SDL_DestroyWindow(res->mainWindow);
		free(res);
	}
	return res;
}

void spMainWindowDestroy(SPCHESSMainWin* src) {
	if (!src)
		return;

	if (src->btns != NULL)
		destroyButtons(src->btns, src->numOfBtns);

	if (src->mainRenderer != NULL)
		SDL_DestroyRenderer(src->mainRenderer);

	if (src->mainWindow != NULL)
		SDL_DestroyWindow(src->mainWindow);

	free(src);
}

void spMainWindowDraw(SPCHESSMainWin* src) {
	if (!src)
		return;

	SDL_SetRenderDrawColor(src->mainRenderer, 40, 40, 40, 0);
	SDL_RenderClear(src->mainRenderer);
	for (int i = 0; i < src->numOfBtns; i++)
		drawButton(src->btns[i]);

	SDL_RenderPresent(src->mainRenderer);
}

SPCHESS_MAIN_EVENT spMainWindowHandleEvent(SPCHESSMainWin* src,
		SDL_Event* event) {
	if (!src || !event)
		return SPCHESS_MAIN_INVALID_ARGUMENT;

	SPCHESS_BUTTON_TYPE btn = NO_BUTTON;

	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		btn = getButtonClicked(src->btns, src->numOfBtns, event, false);
		if (btn == BUTTON_MAIN_NEW_GAME)
			return SPCHESS_MAIN_NEW_GAME;
		else if (btn == BUTTON_MAIN_LOAD)
			return SPCHESS_MAIN_LOAD;
		else if (btn == BUTTON_MAIN_EXIT)
			return SPCHESS_MAIN_EXIT;
		break;

	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPCHESS_MAIN_QUIT;
		break;
	default:
		return SPCHESS_MAIN_NONE;
	}
	return SPCHESS_MAIN_NONE;
}

void spMainWindowHide(SPCHESSMainWin* src) {
	SDL_HideWindow(src->mainWindow);
}

void spMainWindowShow(SPCHESSMainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}

