#include "SPCHESSGUILoadWin.h"

SPCHESSLoadWin* spLoadWindowCreate() {
	SPCHESSLoadWin* res = (SPCHESSLoadWin*) calloc(sizeof(SPCHESSLoadWin),
			sizeof(char));
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

	bool visible[NUM_OF_LOAD_BUTTONS] = { false, false, false, false, false,
	true, true };

	int numOfSlots = countSavedFiles();
	for (int i = 0; i < numOfSlots; i++)
		visible[i] = true;

	bool active[NUM_OF_LOAD_BUTTONS] = { true, true, true, true, true, true,
	false };
	SPCHESS_BUTTON_TYPE types[NUM_OF_LOAD_BUTTONS] = { BUTTON_LOAD_SLOT0,
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

void spLoadWindowDestroy(SPCHESSLoadWin* src) {
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

void spLoadWindowDraw(SPCHESSLoadWin* src) {
	if (src == NULL)
		return;

	SDL_SetRenderDrawColor(src->loadRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->loadRenderer);
	for (int i = 0; i < src->numOfBtns; i++)
		drawButton(src->btns[i]);

	SDL_RenderPresent(src->loadRenderer);
}

SPCHESS_LOAD_EVENT spLoadWindowHandleEvent(SPCHESSLoadWin* src,
		SDL_Event* event) {
	if (!src || !event)
		return SPCHESS_LOAD_INVALID_ARGUMENT;

	int numOfSlots = countSavedFiles();
	for (int i = 0; i < numOfSlots; i++)
		src->btns[i]->visible = true;

	SPCHESS_BUTTON_TYPE btn = NO_BUTTON;
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		btn = getButtonClicked(src->btns, src->numOfBtns, event, false);
		if (btn == BUTTON_LOAD_BACK)
			return SPCHESS_LOAD_BACK;

		if (btn >= BUTTON_LOAD_SLOT0 && btn <= BUTTON_LOAD_SLOT4) {

			//de-activate all slots
			src->btns[0]->active = false;
			src->btns[1]->active = false;
			src->btns[2]->active = false;
			src->btns[3]->active = false;
			src->btns[4]->active = false;

			src->btns[btn - 3]->active = true; //chosen slot is activated (assuming BUTTOM_LOAD_SLOT0 = 3)
			src->slotPicked = btn - 3;
			src->btns[6]->active = true; //change active of load btn
			return SPCHESS_LOAD_SLOT;
		}
		if (btn == BUTTON_LOAD_LOAD && src->slotPicked != -1) {
			SPGame* loaded;
			switch (src->slotPicked) {
			case 4:
				loadGame(loaded, SLOT4);
				break;
			case 3:
				loadGame(loaded, SLOT3);
				break;
			case 2:
				loadGame(loaded, SLOT2);
				break;
			case 1:
				loadGame(loaded, SLOT1);
				break;
			//SLOT0
			default:
				loadGame(loaded, SLOT0);
				break;
			}
			spGameDestroy(src->game);
			src->game = loaded;
			return SPCHESS_LOAD_LOAD;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPCHESS_LOAD_QUIT;
		break;
	default:
		return SPCHESS_LOAD_NONE;
	}
	return SPCHESS_LOAD_NONE;

}

void spLoadWindowHide(SPCHESSLoadWin* src) {
	SDL_HideWindow(src->loadWindow);
}

void spLoadWindowShow(SPCHESSLoadWin* src) {
	SDL_ShowWindow(src->loadWindow);
}

