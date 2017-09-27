#include "SPCHESSGUIManager.h"

SPCHESSGuiManager* spManagerCreate() {
	SPCHESSGuiManager* res = (SPCHESSGuiManager*) malloc(
			sizeof(SPCHESSGuiManager));
	if (!res)
		return NULL;

	res->mainWin = spMainWindowCreate();
	if (res->mainWin == NULL) {
		free(res);
		return NULL;
	}
	res->setWin = NULL;
	res->gameWin = NULL;
	res->loadWin = NULL;
	res->activeWin = SPCHESS_MAIN_WINDOW_ACTIVE;
	res->prevWin = SPCHESS_NO_WINDOW;
	return res;
}

void spManagerDestroy(SPCHESSGuiManager* src) {
	if (!src)
		return;

	if (src->activeWin == SPCHESS_GAME_WINDOW_ACTIVE)
		spGameWindowDestroy(src->gameWin);
	else if (src->activeWin == SPCHESS_SET_WINDOW_ACTIVE)
		spSetWindowDestroy(src->setWin);
	else if (src->activeWin == SPCHESS_LOAD_WINDOW_ACTIVE)
		spLoadWindowDestroy(src->loadWin);

	spMainWindowDestroy(src->mainWin);
	free(src);
}

void spManagerDraw(SPCHESSGuiManager* src, SDL_Event* event) {
	if (!src)
		return;

	if (src->activeWin == SPCHESS_MAIN_WINDOW_ACTIVE)
		spMainWindowDraw(src->mainWin);
	else if (src->activeWin == SPCHESS_GAME_WINDOW_ACTIVE)
		spGameWindowDraw(src->gameWin, event); //passing event param because of motion-event
	else if (src->activeWin == SPCHESS_SET_WINDOW_ACTIVE)
		spSetWindowDraw(src->setWin);
	else if (src->activeWin == SPCHESS_LOAD_WINDOW_ACTIVE)
		spLoadWindowDraw(src->loadWin);
}

SPCHESS_MANAGER_EVENT handleManagerDueToMainEvent(SPCHESSGuiManager* src,
		SPCHESS_MAIN_EVENT event) {
	if (!src)
		return SPCHESS_MANAGER_NONE;

	if (event == SPCHESS_MAIN_NEW_GAME) {
		spMainWindowHide(src->mainWin);
		src->setWin = spSetWindowCreate();
		if (src->setWin == NULL) {
			printf("couldn't create set window\n");
			return SPCHESS_MANAGER_QUIT;
		}
		src->activeWin = SPCHESS_SET_WINDOW_ACTIVE;
		src->prevWin = SPCHESS_MAIN_WINDOW_ACTIVE;
	}
	if (event == SPCHESS_MAIN_LOAD) {
		spMainWindowHide(src->mainWin);
		src->loadWin = spLoadWindowCreate();
		if (src->loadWin == NULL) {
			printf("couldn't create load window\n");
			return SPCHESS_MANAGER_QUIT;
		}
		src->activeWin = SPCHESS_LOAD_WINDOW_ACTIVE;
		src->prevWin = SPCHESS_MAIN_WINDOW_ACTIVE;
	}
	if (event == SPCHESS_MAIN_EXIT || event == SPCHESS_MAIN_QUIT) {
		return SPCHESS_MANAGER_QUIT;
	}
	return SPCHESS_MANAGER_NONE;
}

SPCHESS_MANAGER_EVENT handleManagerDueToGameEvent(SPCHESSGuiManager* src,
		SPCHESS_GAME_EVENT event) {
	if (!src)
		return SPCHESS_MANAGER_NONE;

	if (event == SPCHESS_GAME_LOAD) {
		spGameWindowHide(src->gameWin);
		if (src->loadWin != NULL)
			spLoadWindowShow(src->loadWin);
		else {
			src->loadWin = spLoadWindowCreate();
			if (src->loadWin == NULL) {
				printf("couldn't create load window\n");
				return SPCHESS_MANAGER_QUIT;
			}
		}
		src->activeWin = SPCHESS_LOAD_WINDOW_ACTIVE;
		src->prevWin = SPCHESS_GAME_WINDOW_ACTIVE;
	}
	if (event == SPCHESS_GAME_MAIN_MENU) {
		spGameWindowDestroy(src->gameWin);
		spMainWindowShow(src->mainWin);
		src->activeWin = SPCHESS_MAIN_WINDOW_ACTIVE;
		src->prevWin = SPCHESS_NO_WINDOW;
	}
	if (event == SPCHESS_GAME_PLAYER_1_CHECKMATE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECKMATE!",
				"White player won the game", NULL);
		return SPCHESS_MANAGER_QUIT;
	}
	if (event == SPCHESS_GAME_PLAYER_2_CHECKMATE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECKMATE!",
				"Black player won the game", NULL);
		return SPCHESS_MANAGER_QUIT;
	}
	if (event == SPCHESS_GAME_TIE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TIE!",
				"The game ends with tie", NULL);
		return SPCHESS_MANAGER_QUIT;
	}
	if (event == SPCHESS_GAME_EXIT || event == SPCHESS_GAME_QUIT) {
		return SPCHESS_MANAGER_QUIT;
	}
	return SPCHESS_MANAGER_NONE;
}

SPCHESS_MANAGER_EVENT handleManagerDueToLoadEvent(SPCHESSGuiManager* src,
		SPCHESS_LOAD_EVENT event) {

	if (!src)
		return SPCHESS_MANAGER_NONE;

	if (event == SPCHESS_LOAD_BACK) {
		spLoadWindowHide(src->loadWin);
		if (src->prevWin == SPCHESS_MAIN_WINDOW_ACTIVE) {
			src->activeWin = SPCHESS_MAIN_WINDOW_ACTIVE;
			spMainWindowShow(src->mainWin);
		} else if (src->prevWin == SPCHESS_GAME_WINDOW_ACTIVE) {
			src->activeWin = SPCHESS_GAME_WINDOW_ACTIVE;
			spGameWindowShow(src->gameWin);
		}
		src->prevWin = SPCHESS_LOAD_WINDOW_ACTIVE;
	}
	if (event == SPCHESS_LOAD_LOAD) {
		spLoadWindowHide(src->loadWin);
		src->gameWin = spGameWindowCreate(src->loadWin->game);
		if (src->gameWin == NULL) {
			printf("couldn't create game window\n");
			return SPCHESS_MANAGER_QUIT;
		}
		src->gameWin->isSaved = true;
		src->activeWin = SPCHESS_GAME_WINDOW_ACTIVE;
		src->prevWin = SPCHESS_LOAD_WINDOW_ACTIVE;
	}
	if (event == SPCHESS_LOAD_QUIT)
		return SPCHESS_MANAGER_QUIT;

	return SPCHESS_MANAGER_NONE;

}

SPCHESS_MANAGER_EVENT handleManagerDueToSetEvent(SPCHESSGuiManager* src,
		SPCHESS_SET_EVENT event) {
	if (!src)
		return SPCHESS_MANAGER_NONE;

	if (event == SPCHESS_SET_BACK) {
		spSetWindowHide(src->setWin);
		spMainWindowShow(src->mainWin);
		src->activeWin = SPCHESS_MAIN_WINDOW_ACTIVE;
		src->prevWin = SPCHESS_SET_WINDOW_ACTIVE;
	}
	if (event == SPCHESS_SET_START) {
		spSetWindowHide(src->setWin);
		src->gameWin = spGameWindowCreate(src->setWin->game);
		if (src->gameWin == NULL) {
			printf("couldn't create game window\n");
			return SPCHESS_MANAGER_QUIT;
		}
		spSetWindowDestroy(src->setWin);
		src->activeWin = SPCHESS_GAME_WINDOW_ACTIVE;
		src->prevWin = SPCHESS_SET_WINDOW_ACTIVE;
	}
	if (event == SPCHESS_SET_QUIT)
		return SPCHESS_MANAGER_QUIT;

	return SPCHESS_MANAGER_NONE;
}

SPCHESS_MANAGER_EVENT spManagerHandleEvent(SPCHESSGuiManager* src,
		SDL_Event* event) {
	if (!src || !event)
		return SPCHESS_MANAGER_NONE;

	if (src->activeWin == SPCHESS_MAIN_WINDOW_ACTIVE) {
		SPCHESS_MAIN_EVENT mainEvent = spMainWindowHandleEvent(src->mainWin,
				event);
		spManagerDraw(src, event);
		return handleManagerDueToMainEvent(src, mainEvent);
	} else if (src->activeWin == SPCHESS_GAME_WINDOW_ACTIVE) {
		SPCHESS_GAME_EVENT gameEvent = spGameWindowHandleEvent(src->gameWin,
				event);
		spManagerDraw(src, event);
		return handleManagerDueToGameEvent(src, gameEvent);
	} else if (src->activeWin == SPCHESS_LOAD_WINDOW_ACTIVE) {
		SPCHESS_LOAD_EVENT loadEvent = spLoadWindowHandleEvent(src->loadWin,
				event);
		spManagerDraw(src, event);
		return handleManagerDueToLoadEvent(src, loadEvent);
	} else if (src->activeWin == SPCHESS_SET_WINDOW_ACTIVE) {
		SPCHESS_SET_EVENT setEvent = spSetWindowHandleEvent(src->setWin, event);
		spManagerDraw(src, event);
		return handleManagerDueToSetEvent(src, setEvent);
	}
	return SPCHESS_MANAGER_NONE;
}

