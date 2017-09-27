#include "SPCHESSGUISetWin.h"

SPCHESSSetWin* spSetWindowCreate() {
	SPCHESSSetWin* res = (SPCHESSSetWin*) calloc(sizeof(SPCHESSSetWin),
			sizeof(char));
	if (res == NULL) {
		printf("Couldn't create spChessSetWin struct\n");
		return NULL;
	}
	res->setWindow = SDL_CreateWindow("CHESS GAME: Setting Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 730, 650, SDL_WINDOW_OPENGL);
	if (res->setWindow == NULL) {
		spSetWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	res->setRenderer = SDL_CreateRenderer(res->setWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->setRenderer == NULL) {
		spSetWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	res->numOfBtns = NUM_OF_SET_BUTTONS;
	const char* activeImages[NUM_OF_SET_BUTTONS] = { ACT_IMG(game_mode),
			ACT_IMG(one_player), ACT_IMG(two_players), ACT_IMG(difficulty),
			ACT_IMG(noob), ACT_IMG(easy), ACT_IMG(moderate), ACT_IMG(hard),
			ACT_IMG(select_color), ACT_IMG(black_player), ACT_IMG(
					white_player), ACT_IMG(start), ACT_IMG(back) };

	const char* inactiveImages[NUM_OF_SET_BUTTONS] = { ACT_IMG(game_mode),
			INACT_IMG(one_player), INACT_IMG(two_players), ACT_IMG(difficulty),
			INACT_IMG(noob), INACT_IMG(easy), INACT_IMG(moderate), INACT_IMG(
					hard), ACT_IMG(select_color), INACT_IMG(black_player),
					INACT_IMG(white_player), INACT_IMG(start), INACT_IMG(back) };

	int xBtns[NUM_OF_SET_BUTTONS] = { 270, 170, 360, 270, 35, 205, 375, 545,
			270, 170, 360, 460, 80 };
	int yBtns[NUM_OF_SET_BUTTONS] = { 20, 80, 80, 153, 230, 230, 230, 230,
			312, 380, 380, 500, 500 };

	bool visible[NUM_OF_SET_BUTTONS] = { true, true, true, true, true, true,
			true, true,
			true, true, true, true, true };

	bool active[NUM_OF_SET_BUTTONS] = { false, true, true, false, false, false,
			false, false,
			false, false, false, false, true };

	SPCHESS_BUTTON_TYPE types[NUM_OF_SET_BUTTONS] = { BUTTON_SET_GAME_MODE,
			BUTTON_SET_ONE_PLAYER, BUTTON_SET_TWO_PLAYER, BUTTON_SET_DIFF,
			BUTTON_SET_NOOB_DIFF, BUTTON_SET_EASY_DIFF, BUTTON_SET_MOD_DIFF,
			BUTTON_SET_HARD_DIFF, BUTTON_SET_SELECT_COLOR,
			BUTTON_SET_BLACK_PLAYER, BUTTON_SET_WHITE_PLAYER, BUTTON_SET_START,
			BUTTON_SET_BACK };

	res->btns = createButtons(res->setRenderer, activeImages, inactiveImages,
			xBtns, yBtns, visible, active, types, res->numOfBtns);

	if (res->btns == NULL) {
		SDL_DestroyRenderer(res->setRenderer);
		SDL_DestroyWindow(res->setWindow);
		free(res);
	}

	res->game = spGameCreateDef();
	return res;
}

void spSetWindowDestroy(SPCHESSSetWin* src) {
	if (!src)
		return;

	if (src->game != NULL)
		spGameDestroy(src->game);

	if (src->btns != NULL)
		destroyButtons(src->btns, src->numOfBtns);

	if (src->setRenderer != NULL)
		SDL_DestroyRenderer(src->setRenderer);

	if (src->setWindow != NULL)
		SDL_DestroyWindow(src->setWindow);

	free(src);
}

void spSetWindowDraw(SPCHESSSetWin* src) {
	if (src == NULL)
		return;

	SDL_SetRenderDrawColor(src->setRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->setRenderer);
	for (int i = 0; i < src->numOfBtns; i++)
		drawButton(src->btns[i]);

	SDL_RenderPresent(src->setRenderer);
}

SPCHESS_SET_EVENT spSetWindowHandleEvent(SPCHESSSetWin* src, SDL_Event* event) {
	if (!src || !event)
		return SPCHESS_SET_INVALID_ARGUMENT;

	SPCHESS_BUTTON_TYPE btn = NO_BUTTON;

	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		btn = getButtonClicked(src->btns, src->numOfBtns, event, true);

		if (btn == BUTTON_SET_TWO_PLAYER) {
			src->game->settings->game_mode = 2; //change the game mode to 2
			src->btns[11]->active = true; //activate start button
			//de-activate other options

			src->btns[4]->active = false;
			src->btns[5]->active = false;
			src->btns[6]->active = false;
			src->btns[7]->active = false;
			src->btns[9]->active = false;
			src->btns[10]->active = false;

			return SPCHESS_SET_GAME_MODE;
		} else if (btn == BUTTON_SET_ONE_PLAYER) {

			src->game->settings->game_mode = 1; //change the game mode to 1
			//activate possible difficulty levels
			src->btns[4]->active = true;
			src->btns[5]->active = true;
			src->btns[6]->active = true;
			src->btns[7]->active = true;
			src->btns[11]->active = false; //de-activate start btn
			return SPCHESS_SET_GAME_MODE;
		} else if (btn >= BUTTON_SET_NOOB_DIFF && btn <= BUTTON_SET_HARD_DIFF) {

			src->game->settings->difficulty = btn - 13; //set difficulty (assuming BUTTON_SET_NOOB_DIFF = 14)
			//activate color player stage
			src->btns[9]->active = true;
			src->btns[10]->active = true;
			return SPCHESS_SET_DIFF;
		} else if (btn == BUTTON_SET_WHITE_PLAYER
				|| btn == BUTTON_SET_BLACK_PLAYER) {
			src->game->settings->p1_color = btn - 19; //set user color (assuming BUTTON_SET_BLACK_PLAYER = 19)
			src->btns[11]->active = true; //activate start button
			return SPCHESS_SET_COLOR;
		} else if (btn == BUTTON_SET_BACK)
			return SPCHESS_SET_BACK;
		else if (btn == BUTTON_SET_START)
			return SPCHESS_SET_START;
		break;

	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPCHESS_SET_QUIT;
		break;
	default:
		return SPCHESS_SET_NONE;
	}
	return SPCHESS_SET_NONE;
}

void spSetWindowHide(SPCHESSSetWin* src) {
	SDL_HideWindow(src->setWindow);
}

void spSetWindowShow(SPCHESSSetWin* src) {
	SDL_ShowWindow(src->setWindow);
}

