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
	res->numOfBtns = NUM_OF_LOAD_BUTTONS;
	const char* activeImages[NUM_OF_LOAD_BUTTONS] = { ACT_IMG(slot0), ACT_IMG(
			slot1), ACT_IMG(slot2), ACT_IMG(slot3), ACT_IMG(slot4), ACT_IMG(
			back), ACT_IMG(load) };
	const char* inactiveImages[NUM_OF_LOAD_BUTTONS] = { INACT_IMG(slot0),
			INACT_IMG(slot1), INACT_IMG(slot2), INACT_IMG(slot3), INACT_IMG(
					slot4), INACT_IMG(back), INACT_IMG(load) };
	int xBtns[NUM_OF_LOAD_BUTTONS] = { 230, 230, 230, 230, 230, 50, 400 };
	int yBtns[NUM_OF_LOAD_BUTTONS] = { 50, 130, 210, 290, 370, 490, 490 };
	bool visible[NUM_OF_LOAD_BUTTONS] = { false, false, false, false, false, true, true };
	int numOfSlots = countSavedFiles();
	for (int i = 0; i < numOfSlots; i++)
		visible[i] = true;
	bool active[NUM_OF_LOAD_BUTTONS] = { true, true, true, true, true, true, false };
	SPGUI_BUTTON_TYPE types[NUM_OF_LOAD_BUTTONS] = { BUTTON_LOAD_SLOT0,
			BUTTON_LOAD_SLOT1, BUTTON_LOAD_SLOT2, BUTTON_LOAD_SLOT3,
			BUTTON_LOAD_SLOT4, BUTTON_LOAD_BACK, BUTTON_LOAD_LOAD };
	res->btns = createButtons(res->loadRenderer, activeImages, inactiveImages,
			xBtns, yBtns, visible, active, types, res->numOfBtns);
	if (res->btns == NULL) {
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
	if (src->btns != NULL)
		destroyButtons(src->btns, src->numOfBtns);
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
	for (int i = 0; i < src->numOfBtns; i++)
		drawButton(src->btns[i]);
	SDL_RenderPresent(src->loadRenderer);
}
SPGUI_LOAD_EVENT spLoadWindowEventHandler(SPGUILoadWindow *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_LOAD_INVALID_ARGUMENT;
	for (int i = 0; i < countSavedFiles(); i++)
		src->btns[i]->visible = true;
	SPGUI_BUTTON_TYPE btn = NO_BUTTON;
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		btn = getButtonClicked(src->btns, src->numOfBtns, event, false);
		if (btn == BUTTON_LOAD_BACK)
			return SPGUI_LOAD_BACK;
		if (btn >= BUTTON_LOAD_SLOT0 && btn <= BUTTON_LOAD_SLOT4) {
			for(int i = 0; i<NUM_OF_SAVES; i++) //de-activate all slots
				src->btns[i]->active = false;
			src->slotPicked = btn - (int)BUTTON_LOAD_SLOT0;
			src->btns[src->slotPicked]->active = true; //active chosen slot
			src->btns[6]->active = true; //active load btn
			return SPGUI_LOAD_SLOT;
		}
		if (btn == BUTTON_LOAD_LOAD && src->slotPicked >= 0 && src->slotPicked <= 4) {
			SPGame* loaded = spGameCreateDef();
			const char* savePaths[] = {SAVE1 , SAVE2 , SAVE3 , SAVE4 , SAVE5};
			loadGame(loaded, (char *)savePaths[src->slotPicked]);
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