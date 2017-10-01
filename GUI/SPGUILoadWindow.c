#include "SPGUILoadWindow.h"

SPGUILoadWindow* spLoadWindowCreate() {
	SPGUILoadWindow* res = (SPGUILoadWindow*) calloc(sizeof(SPGUILoadWindow), sizeof(char));
	if (res == NULL) {
		printf("Couldn't create spChessLoadWin struct\n");
		return NULL;
	}
	res->loadWindow = SDL_CreateWindow("CHESS GAME: Load Window", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL);
	if (res->loadWindow == NULL) {
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->loadRenderer = SDL_CreateRenderer(res->loadWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->loadRenderer == NULL) {
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	const char* activeImages[NUM_OF_LOAD_BUTTONS] = { ACTIVE_BMP_PATH(slot0), ACTIVE_BMP_PATH(
			slot1), ACTIVE_BMP_PATH(slot2), ACTIVE_BMP_PATH(slot3), ACTIVE_BMP_PATH(slot4), ACTIVE_BMP_PATH(
			back), ACTIVE_BMP_PATH(load) };
	const char* inactiveImages[NUM_OF_LOAD_BUTTONS] = { INACTIVE_BMP_PATH(slot0),
			INACTIVE_BMP_PATH(slot1), INACTIVE_BMP_PATH(slot2), INACTIVE_BMP_PATH(slot3), INACTIVE_BMP_PATH(
					slot4), INACTIVE_BMP_PATH(back), INACTIVE_BMP_PATH(load) };
	int xVals[NUM_OF_LOAD_BUTTONS] = { 230, 230, 230, 230, 230, 50, 400 };
	int yVals[NUM_OF_LOAD_BUTTONS] = { 50, 130, 210, 290, 370, 490, 490 };
	bool visible[NUM_OF_LOAD_BUTTONS] = { false, false, false, false, false, true, true };
	int numOfSlots = countSavedFiles();
	for (int i = 0; i < numOfSlots; i++)
		visible[i] = true;
	bool active[NUM_OF_LOAD_BUTTONS] = { true, true, true, true, true, true, false };
	SPGUI_BUTTON_TYPE types[NUM_OF_LOAD_BUTTONS] = { BUTTON_LOAD_SLOT0,
			BUTTON_LOAD_SLOT1, BUTTON_LOAD_SLOT2, BUTTON_LOAD_SLOT3,
			BUTTON_LOAD_SLOT4, BUTTON_LOAD_BACK, BUTTON_LOAD_LOAD };
	res->buttons = createButtons(res->loadRenderer, activeImages, inactiveImages,
			xVals, yVals, visible, active, types, NUM_OF_LOAD_BUTTONS);
	if (res->buttons == NULL) {
		SDL_DestroyRenderer(res->loadRenderer);
		SDL_DestroyWindow(res->loadWindow);
		free(res);
	}
	res->slotPicked = -1;
	return res;
}

void spLoadWindowDestroy(SPGUILoadWindow* src) {
	if (!src)
		return;
	if (src->buttons != NULL)
		destroyButtons(src->buttons, NUM_OF_LOAD_BUTTONS);
	if (src->game != NULL)
		spGameDestroy(src->game);
	if (src->loadRenderer != NULL)
		SDL_DestroyRenderer(src->loadRenderer);
	if (src->loadWindow != NULL)
		SDL_DestroyWindow(src->loadWindow);
	free(src);
}

void spLoadWindowDraw(SPGUILoadWindow* src) {
	if (src == NULL)
		return;
	SDL_SetRenderDrawColor(src->loadRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->loadRenderer);
	for (int i = 0; i < NUM_OF_LOAD_BUTTONS; i++)
		drawButton(src->buttons[i]);
	SDL_RenderPresent(src->loadRenderer);
}
SPGUI_LOAD_EVENT spLoadWindowEventHandler(SPGUILoadWindow *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_LOAD_INVALID_ARGUMENT;
	for (int i = 0; i < countSavedFiles(); i++)
		src->buttons[i]->visible = true;
	SPGUI_BUTTON_TYPE button = NO_BUTTON;
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		button = getClickedButtonType(src->buttons, NUM_OF_LOAD_BUTTONS, event, false);
		if (button == BUTTON_LOAD_BACK)
			return SPGUI_LOAD_BACK;
		if (button >= BUTTON_LOAD_SLOT0 && button <= BUTTON_LOAD_SLOT4) {
			for(int i = 0; i<NUM_OF_SAVES; i++)
				src->buttons[i]->active = true;
			src->slotPicked = button - (int)BUTTON_LOAD_SLOT0;
			src->buttons[src->slotPicked]->active = false;
			src->buttons[6]->active = true;
			return SPGUI_LOAD_SLOT;
		}
		if (button == BUTTON_LOAD_LOAD && src->slotPicked >= 0 && src->slotPicked <= 4) {
			for(int i = 0; i<NUM_OF_SAVES; i++)
				src->buttons[i]->active = true;
			SPGame* loaded = spGameCreateDef();
			const char* savePaths[] = {SAVE1 , SAVE2 , SAVE3 , SAVE4 , SAVE5};
            spGameLoadHandler(loaded, (char *) savePaths[src->slotPicked]);
			spGameDestroy(src->game);
			src->game = loaded;
			return SPGUI_LOAD_LOAD;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPGUI_LOAD_QUIT;
		break;
	default:
		return SPGUI_LOAD_NONE;
	}
	return SPGUI_LOAD_NONE;
}