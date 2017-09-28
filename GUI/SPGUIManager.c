#include "SPGUIManager.h"

int MainGUIManager() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SPGUIManager* manager = spManagerCreate();
	if (manager == NULL) {
		SDL_Quit();
		return 0;
	}
	SDL_Event event;
	while (true) {
		SDL_WaitEvent(&event);
		if (spManagerEventHandler(manager, &event) == SPGUI_MANAGER_EVENT_QUIT)
			break;
	}
	spManagerDestroy(manager);
	SDL_Quit();
	return 0;
}

SPGUIManager* spManagerCreate() {
	SPGUIManager* res = (SPGUIManager*) malloc(sizeof(SPGUIManager));
	if (!res)
		return NULL;
	res->mainWindow = spMainWindowCreate();
	if (res->mainWindow == NULL) {
		free(res);
		return NULL;
	}
	res->setWindow = NULL;
	res->gameWindow = NULL;
	res->loadWindow = NULL;
	res->activeWindow = SPGUI_MAIN_WINDOW;
	res->prevWindow = SPGUI_NO_WINDOW;
	return res;
}

void spManagerDestroy(SPGUIManager* src) {
	if (!src)
		return;
	if (src->activeWindow == SPGUI_GAME_WINDOW)
		spGameWindowDestroy(src->gameWindow);
	else if (src->activeWindow == SPGUI_SET_WINDOW)
		spSetWindowDestroy(src->setWindow);
	else if (src->activeWindow == SPGUI_LOAD_WINDOW)
		spLoadWindowDestroy(src->loadWindow);

	spMainWindowDestroy(src->mainWindow);
	free(src);
}

void spManagerDraw(SPGUIManager* src, SDL_Event* event) {
	if (!src)
		return;
	if (src->activeWindow == SPGUI_MAIN_WINDOW)
		spMainWindowDraw(src->mainWindow);
	else if (src->activeWindow == SPGUI_GAME_WINDOW)
		spGameWindowDraw(src->gameWindow, event); //passing event param because of motion-event
	else if (src->activeWindow == SPGUI_SET_WINDOW)
		spSetWindowDraw(src->setWindow);
	else if (src->activeWindow == SPGUI_LOAD_WINDOW)
		spLoadWindowDraw(src->loadWindow);
}

SPGUI_MANAGER_EVENT spManagerEventHandler(SPGUIManager *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_MANAGER_EVENT_NONE;
	if (src->activeWindow == SPGUI_MAIN_WINDOW) {
		SPGUI_MAIN_EVENT mainEvent = spMainWindowEventHandler(src->mainWindow, event);
		spManagerDraw(src, event);
		return spManagerHandleMainEvent(src, mainEvent);
	} else if (src->activeWindow == SPGUI_GAME_WINDOW) {
		SPGUI_GAME_EVENT gameEvent = spGameWindowEventHandler(src->gameWindow, event);
		spManagerDraw(src, event);
		return spManagerHandleGameEvent(src, gameEvent);
	} else if (src->activeWindow == SPGUI_LOAD_WINDOW) {
		SPGUI_LOAD_EVENT loadEvent = spLoadWindowEventHandler(src->loadWindow, event);
		spManagerDraw(src, event);
		return spManagerHandleLoadEvent(src, loadEvent);
	} else if (src->activeWindow == SPGUI_SET_WINDOW) {
		SPGUI_SET_EVENT setEvent = spSetWindowEventHandler(src->setWindow, event);
		spManagerDraw(src, event);
		return spManagerHandleSetEvent(src, setEvent);
	}
	return SPGUI_MANAGER_EVENT_NONE;
}

SPGUI_MANAGER_EVENT spManagerHandleMainEvent(SPGUIManager *src, SPGUI_MAIN_EVENT event) {
	if (!src)
		return SPGUI_MANAGER_EVENT_NONE;
	if (event == SPGUI_MAIN_NEW_GAME) {
		SDL_HideWindow(src->mainWindow->mainWindow);
		src->setWindow = spSetWindowCreate();
		if (src->setWindow == NULL) {
			printf("couldn't create set window\n");
			return SPGUI_MANAGER_EVENT_QUIT;
		}
		src->activeWindow = SPGUI_SET_WINDOW;
		src->prevWindow = SPGUI_MAIN_WINDOW;
	}
	if (event == SPGUI_MAIN_LOAD) {
		SDL_HideWindow(src->mainWindow->mainWindow);
		src->loadWindow = spLoadWindowCreate();
		if (src->loadWindow == NULL) {
			printf("couldn't create load window\n");
			return SPGUI_MANAGER_EVENT_QUIT;
		}
		src->activeWindow = SPGUI_LOAD_WINDOW;
		src->prevWindow = SPGUI_MAIN_WINDOW;
	}
	if (event == SPGUI_MAIN_EXIT || event == SPGUI_MAIN_QUIT) {
		return SPGUI_MANAGER_EVENT_QUIT;
	}
	return SPGUI_MANAGER_EVENT_NONE;
}

SPGUI_MANAGER_EVENT spManagerHandleGameEvent(SPGUIManager *src, SPGUI_GAME_EVENT event) {
	if (!src)
		return SPGUI_MANAGER_EVENT_NONE;
	if (event == SPGUI_GAME_LOAD) {
		SDL_HideWindow(src->gameWindow->gameWindow);
		if (src->loadWindow != NULL)
			SDL_ShowWindow(src->loadWindow->loadWindow);
		else {
			src->loadWindow = spLoadWindowCreate();
			if (src->loadWindow == NULL) {
				printf("couldn't create load window\n");
				return SPGUI_MANAGER_EVENT_QUIT;
			}
		}
		src->activeWindow = SPGUI_LOAD_WINDOW;
		src->prevWindow = SPGUI_GAME_WINDOW;
	}
	if (event == SPGUI_GAME_MAIN_MENU) {
		spGameWindowDestroy(src->gameWindow);
		SDL_ShowWindow(src->mainWindow->mainWindow);
		src->activeWindow = SPGUI_MAIN_WINDOW;
		src->prevWindow = SPGUI_NO_WINDOW;
	}
	if (event == SPGUI_GAME_PLAYER_1_CHECKMATE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECKMATE!",
				"White player won the game", NULL);
		return SPGUI_MANAGER_EVENT_QUIT;
	}
	if (event == SPGUI_GAME_PLAYER_2_CHECKMATE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECKMATE!",
				"Black player won the game", NULL);
		return SPGUI_MANAGER_EVENT_QUIT;
	}
	if (event == SPGUI_GAME_TIE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TIE!",
				"The game ends with tie", NULL);
		return SPGUI_MANAGER_EVENT_QUIT;
	}
	if (event == SPGUI_GAME_EXIT || event == SPGUI_GAME_QUIT) {
		return SPGUI_MANAGER_EVENT_QUIT;
	}
	return SPGUI_MANAGER_EVENT_NONE;
}

SPGUI_MANAGER_EVENT spManagerHandleLoadEvent(SPGUIManager *src, SPGUI_LOAD_EVENT event) {

	if (!src)
		return SPGUI_MANAGER_EVENT_NONE;

	if (event == SPGUI_LOAD_BACK) {
        SDL_HideWindow(src->loadWindow->loadWindow);
		if (src->prevWindow == SPGUI_MAIN_WINDOW) {
			src->activeWindow = SPGUI_MAIN_WINDOW;
			SDL_ShowWindow(src->mainWindow->mainWindow);
		} else if (src->prevWindow == SPGUI_GAME_WINDOW) {
			src->activeWindow = SPGUI_GAME_WINDOW;
			SDL_ShowWindow(src->gameWindow->gameWindow);
		}
		src->prevWindow = SPGUI_LOAD_WINDOW;
	}
	if (event == SPGUI_LOAD_LOAD) {
        SDL_HideWindow(src->loadWindow->loadWindow);
		src->gameWindow = spGameWindowCreate(src->loadWindow->game);
		if (src->gameWindow == NULL) {
			printf("couldn't create game window\n");
			return SPGUI_MANAGER_EVENT_QUIT;
		}
		src->gameWindow->isSaved = true;
		src->activeWindow = SPGUI_GAME_WINDOW;
		src->prevWindow = SPGUI_LOAD_WINDOW;
	}
	if (event == SPGUI_LOAD_QUIT)
		return SPGUI_MANAGER_EVENT_QUIT;

	return SPGUI_MANAGER_EVENT_NONE;

}

SPGUI_MANAGER_EVENT spManagerHandleSetEvent(SPGUIManager *src, SPGUI_SET_EVENT event) {
	if (!src)
		return SPGUI_MANAGER_EVENT_NONE;
	if (event == SPGUI_SET_BACK) {
		SDL_HideWindow(src->setWindow->setWindow);
		SDL_ShowWindow(src->mainWindow->mainWindow);
		src->activeWindow = SPGUI_MAIN_WINDOW;
		src->prevWindow = SPGUI_SET_WINDOW;
	}
	if (event == SPGUI_SET_START) {
		SDL_HideWindow(src->setWindow->setWindow);
		src->gameWindow = spGameWindowCreate(src->setWindow->game);
		if (src->gameWindow == NULL) {
			printf("couldn't create game window\n");
			return SPGUI_MANAGER_EVENT_QUIT;
		}
		spSetWindowDestroy(src->setWindow);
		src->activeWindow = SPGUI_GAME_WINDOW;
		src->prevWindow = SPGUI_SET_WINDOW;
	}
	if (event == SPGUI_SET_QUIT)
		return SPGUI_MANAGER_EVENT_QUIT;

	return SPGUI_MANAGER_EVENT_NONE;
}

