#include "SPGUISettingsWindow.h"

SPGUISettingsWindow* spSettingsWindowCreate() {
	SPGUISettingsWindow* res = (SPGUISettingsWindow*) calloc(sizeof(SPGUISettingsWindow),
			sizeof(char));
	if (res == NULL) {
		printf("Couldn't create spChessSetWin struct\n");
		return NULL;
	}
	res->settingsWindow = SDL_CreateWindow("CHESS GAME: Setting Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 730, 650, SDL_WINDOW_OPENGL);
	if (res->settingsWindow == NULL) {
		spSettingsWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	res->settingsRenderer = SDL_CreateRenderer(res->settingsWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->settingsRenderer == NULL) {
		spSettingsWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
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

	int xVals[NUM_OF_SET_BUTTONS] = { 270, 170, 360, 270, 35, 205, 375, 545,
			270, 170, 360, 460, 80 };
	int yVals[NUM_OF_SET_BUTTONS] = { 20, 80, 80, 153, 230, 230, 230, 230,
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

	res->buttons = createButtons(res->settingsRenderer, activeImages, inactiveImages,
			xVals, yVals, visible, active, types, NUM_OF_SET_BUTTONS);

	if (res->buttons == NULL) {
		SDL_DestroyRenderer(res->settingsRenderer);
		SDL_DestroyWindow(res->settingsWindow);
		free(res);
	}

	res->game = spGameCreateDef();
	return res;
}

void spSettingsWindowDestroy(SPGUISettingsWindow *src) {
	if (!src)
		return;

	if (src->game != NULL)
		spGameDestroy(src->game);

	if (src->buttons != NULL)
		destroyButtons(src->buttons, NUM_OF_SET_BUTTONS);

	if (src->settingsRenderer != NULL)
		SDL_DestroyRenderer(src->settingsRenderer);

	if (src->settingsWindow != NULL)
		SDL_DestroyWindow(src->settingsWindow);

	free(src);
}

void spSettingsWindowDraw(SPGUISettingsWindow *src) {
	if (src == NULL)
		return;
	SDL_SetRenderDrawColor(src->settingsRenderer, 40, 40, 40, 0);
	SDL_RenderClear(src->settingsRenderer);
	for (int i = 0; i < NUM_OF_SET_BUTTONS; i++)
		drawButton(src->buttons[i]);

	SDL_RenderPresent(src->settingsRenderer);
}

SPGUI_SETTINGS_EVENT spSettingsWindowEventHandler(SPGUISettingsWindow *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_SETTINGS_INVALID_ARGUMENT;
	SPGUI_BUTTON_TYPE button;
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		button = getPressedButtonType(src->buttons, NUM_OF_SET_BUTTONS, event, true);
		if (button == BUTTON_SET_TWO_PLAYER) {
			src->game->settings->game_mode = SP_MODE_2P;
			src->buttons[11]->active = true;
			src->buttons[1]->active = true;
			src->buttons[2]->active = false;
			src->buttons[4]->active = false;
			src->buttons[5]->active = false;
			src->buttons[6]->active = false;
			src->buttons[7]->active = false;
			src->buttons[9]->active = false;
			src->buttons[10]->active = false;
			return SPGUI_SETTINGS_GAME_MODE;
		} else if (button == BUTTON_SET_ONE_PLAYER) {
			src->game->settings->game_mode = SP_MODE_1P;
			src->buttons[1]->active = false;
			src->buttons[2]->active = true;
			src->buttons[11]->active = false;
			src->buttons[4]->active = true;
			src->buttons[5]->active = true;
			src->buttons[6]->active = true;
			src->buttons[7]->active = true;
			return SPGUI_SETTINGS_GAME_MODE;
		} else if (button >= BUTTON_SET_NOOB_DIFF && button <= BUTTON_SET_HARD_DIFF) {
			int diff = (int)(button - BUTTON_SET_NOOB_DIFF);
			src->game->settings->difficulty = (SP_GAME_DIFFICULTY)(diff + 1);
			src->buttons[4+diff]->active = false;
			for(int i = 4; i<=7; i++)
				if(i != 4+diff)
					src->buttons[i]->active = true;
			src->buttons[9]->active = true;
			src->buttons[10]->active = true;
			return SPGUI_SETTINGS_DIFF;
		} else if (button == BUTTON_SET_WHITE_PLAYER
				|| button == BUTTON_SET_BLACK_PLAYER) {
			src->game->settings->p1_color = (SP_USER_COLOR)(int)(button - BUTTON_SET_BLACK_PLAYER);
			src->buttons[9 + src->game->settings->p1_color]->active = false;
			src->buttons[10 - src->game->settings->p1_color]->active = true;
			src->buttons[11]->active = true;
			return SPGUI_SETTINGS_COLOR;
		} else if (button == BUTTON_SET_BACK)
			return SPGUI_SETTINGS_BACK;
		else if (button == BUTTON_SET_START)
			return SPGUI_SETTINGS_START;
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPGUI_SETTINGS_QUIT;
		break;
	default:
		return SPGUI_SETTINGS_NONE;
	}
	return SPGUI_SETTINGS_NONE;
}
