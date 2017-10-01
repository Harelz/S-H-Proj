#include "SPGUISetWindow.h"

SPGUISetWindow* spSetWindowCreate() {
	SPGUISetWindow* res = (SPGUISetWindow*) calloc(sizeof(SPGUISetWindow),
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
	const char* activeImages[NUM_OF_SET_BUTTONS] = { ACTIVE_BMP_PATH(game_mode),
			ACTIVE_BMP_PATH(one_player), ACTIVE_BMP_PATH(two_players), ACTIVE_BMP_PATH(difficulty),
			ACTIVE_BMP_PATH(noob), ACTIVE_BMP_PATH(easy), ACTIVE_BMP_PATH(moderate), ACTIVE_BMP_PATH(hard),
			ACTIVE_BMP_PATH(select_color), ACTIVE_BMP_PATH(black_player), ACTIVE_BMP_PATH(
					white_player), ACTIVE_BMP_PATH(start), ACTIVE_BMP_PATH(back) };

	const char* inactiveImages[NUM_OF_SET_BUTTONS] = { ACTIVE_BMP_PATH(game_mode),
			INACTIVE_BMP_PATH(one_player), INACTIVE_BMP_PATH(two_players), ACTIVE_BMP_PATH(difficulty),
			INACTIVE_BMP_PATH(noob), INACTIVE_BMP_PATH(easy), INACTIVE_BMP_PATH(moderate), INACTIVE_BMP_PATH(
					hard), ACTIVE_BMP_PATH(select_color), INACTIVE_BMP_PATH(black_player),
					INACTIVE_BMP_PATH(white_player), INACTIVE_BMP_PATH(start), INACTIVE_BMP_PATH(back) };

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

	SPGUI_BUTTON_TYPE types[NUM_OF_SET_BUTTONS] = { BUTTON_SET_GAME_MODE,
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

void spSetWindowDestroy(SPGUISetWindow* src) {
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

void spSetWindowDraw(SPGUISetWindow* src) {
	if (src == NULL)
		return;

	SDL_SetRenderDrawColor(src->setRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->setRenderer);
	for (int i = 0; i < src->numOfBtns; i++)
		drawButton(src->btns[i]);

	SDL_RenderPresent(src->setRenderer);
}

SPGUI_SET_EVENT spSetWindowEventHandler(SPGUISetWindow *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_SET_INVALID_ARGUMENT;
	SPGUI_BUTTON_TYPE btn;
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		btn = getClickedButtonType(src->btns, src->numOfBtns, event, true);
		if (btn == BUTTON_SET_TWO_PLAYER) {
			src->game->settings->game_mode = SP_MODE_2P;
			src->btns[11]->active = true; //activate start button
			src->btns[1]->active = true; // de-select one player
			src->btns[2]->active = false; // select two players
			src->btns[4]->active = false;
			src->btns[5]->active = false;
			src->btns[6]->active = false;
			src->btns[7]->active = false;
			src->btns[9]->active = false;
			src->btns[10]->active = false;
			return SPGUI_SET_GAME_MODE;
		} else if (btn == BUTTON_SET_ONE_PLAYER) {
			src->game->settings->game_mode = SP_MODE_1P;
			src->btns[1]->active = false; // select one player
			src->btns[2]->active = true; // de-select two players
			src->btns[11]->active = false; //de-activate start btn
			//activate possible difficulty levels
			src->btns[4]->active = true;
			src->btns[5]->active = true;
			src->btns[6]->active = true;
			src->btns[7]->active = true;
			return SPGUI_SET_GAME_MODE;
		} else if (btn >= BUTTON_SET_NOOB_DIFF && btn <= BUTTON_SET_HARD_DIFF) {
			int diff = (int)(btn - BUTTON_SET_NOOB_DIFF);
			src->game->settings->difficulty = (SP_GAME_DIFFICULTY)(diff + 1);
			//activate color player stage
			src->btns[4+diff]->active = false; // deactivate level chosen
			for(int i = 4; i<=7; i++)
				if(i != 4+diff)
					src->btns[i]->active = true;
			src->btns[9]->active = true;
			src->btns[10]->active = true;
			return SPGUI_SET_DIFF;
		} else if (btn == BUTTON_SET_WHITE_PLAYER
				|| btn == BUTTON_SET_BLACK_PLAYER) {
			src->game->settings->p1_color = (SP_USER_COLOR)(int)(btn - BUTTON_SET_BLACK_PLAYER);
			src->btns[9 + src->game->settings->p1_color]->active = false;
			src->btns[10 - src->game->settings->p1_color]->active = true;
			src->btns[11]->active = true; //activate start button
			return SPGUI_SET_COLOR;
		} else if (btn == BUTTON_SET_BACK)
			return SPGUI_SET_BACK;
		else if (btn == BUTTON_SET_START)
			return SPGUI_SET_START;
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPGUI_SET_QUIT;
		break;
	default:
		return SPGUI_SET_NONE;
	}
	return SPGUI_SET_NONE;
}
