#include "SPGUIMainWindow.h"

SPGUIMainWindow* spMainWindowCreate() {
	SPGUIMainWindow* res = NULL;
	res = (SPGUIMainWindow*) calloc(sizeof(SPGUIMainWindow), sizeof(char));
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
	const char* activeImages[NUM_OF_MAIN_BUTTONS] = { ACTIVE_BMP_PATH(new_game), ACTIVE_BMP_PATH(
			load), ACTIVE_BMP_PATH(exit) };

	const char* inactiveImages[NUM_OF_MAIN_BUTTONS] = {
			INACTIVE_BMP_PATH(new_game), INACTIVE_BMP_PATH(load),
			INACTIVE_BMP_PATH(exit) };

	int xVals[NUM_OF_MAIN_BUTTONS] = { 125, 125, 125 };
	int yVals[NUM_OF_MAIN_BUTTONS] = { 63, 176, 289 };
	bool visible[NUM_OF_MAIN_BUTTONS] = { true, true, true };
	bool active[NUM_OF_MAIN_BUTTONS] = { true, true, true };
	SPGUI_BUTTON_TYPE types[NUM_OF_MAIN_BUTTONS] = { BUTTON_MAIN_NEW_GAME,
			BUTTON_MAIN_LOAD, BUTTON_MAIN_EXIT };

	res->buttons = createButtons(res->mainRenderer, activeImages, inactiveImages,
			xVals, yVals, visible, active, types, NUM_OF_MAIN_BUTTONS);

	if (res->buttons == NULL) {
		SDL_DestroyRenderer(res->mainRenderer);
		SDL_DestroyWindow(res->mainWindow);
		free(res);
	}
	return res;
}

void spMainWindowDestroy(SPGUIMainWindow* src) {
	if (!src)
		return;

	if (src->buttons != NULL)
		destroyButtons(src->buttons, NUM_OF_MAIN_BUTTONS);

	if (src->mainRenderer != NULL)
		SDL_DestroyRenderer(src->mainRenderer);

	if (src->mainWindow != NULL)
		SDL_DestroyWindow(src->mainWindow);

	free(src);
}

void spMainWindowDraw(SPGUIMainWindow* src) {
	if (!src)
		return;

	SDL_SetRenderDrawColor(src->mainRenderer, 40, 40, 40, 0);
	SDL_RenderClear(src->mainRenderer);
	for (int i = 0; i < NUM_OF_MAIN_BUTTONS; i++)
		drawButton(src->buttons[i]);
	SDL_RenderPresent(src->mainRenderer);
}

SPGUI_MAIN_EVENT spMainWindowEventHandler(SPGUIMainWindow *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_MAIN_INVALID_ARGUMENT;
	SPGUI_BUTTON_TYPE button = NO_BUTTON;
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		button = getClickedButtonType(src->buttons, NUM_OF_MAIN_BUTTONS, event, false);
		if (button == BUTTON_MAIN_NEW_GAME)
			return SPGUI_MAIN_NEW_GAME;
		else if (button == BUTTON_MAIN_LOAD)
			return SPGUI_MAIN_LOAD;
		else if (button == BUTTON_MAIN_EXIT)
			return SPGUI_MAIN_EXIT;
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPGUI_MAIN_QUIT;
		break;
	default:
		return SPGUI_MAIN_NONE;
	}
	return SPGUI_MAIN_NONE;
}

