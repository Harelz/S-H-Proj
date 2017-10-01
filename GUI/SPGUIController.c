#include "SPGUIController.h"

int MainGUIManager() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SPGUIController* controller = spControllerCreate();
	if (controller == NULL) {
		SDL_Quit();
		return 0;
	}
	SDL_Event event;
	while (true) {
		SDL_WaitEvent(&event);
		if (spControllerEventHandler(controller, &event) == SPGUI_CONTROLLER_EVENT_QUIT)
			break;
	}
	spControllerDestroy(controller);
	SDL_Quit();
	return 0;
}

SPGUIController* spControllerCreate() {
	SPGUIController* res = (SPGUIController*) malloc(sizeof(SPGUIController));
	if (!res)
		return NULL;
	res->mainWindow = spMainWindowCreate();
	if (res->mainWindow == NULL) {
		free(res);
		return NULL;
	}
	res->settingsWindow = NULL;
	res->gameWindow = NULL;
	res->loadWindow = NULL;
	res->activeWindow = SPGUI_MAIN_WINDOW;
	res->previousWindow = SPGUI_NO_WINDOW;
	return res;
}

void spControllerDestroy(SPGUIController *src) {
	if (!src)
		return;
	if (src->activeWindow == SPGUI_GAME_WINDOW)
		spGameWindowDestroy(src->gameWindow);
	else if (src->activeWindow == SPGUI_SET_WINDOW)
		spSetWindowDestroy(src->settingsWindow);
	else if (src->activeWindow == SPGUI_LOAD_WINDOW)
		spLoadWindowDestroy(src->loadWindow);
	spMainWindowDestroy(src->mainWindow);
	free(src);
}

void spControllerDraw(SPGUIController *src, SDL_Event *event) {
	if (!src)
		return;
	if (src->activeWindow == SPGUI_MAIN_WINDOW)
		spMainWindowDraw(src->mainWindow);
	else if (src->activeWindow == SPGUI_GAME_WINDOW)
		spGameWindowDraw(src->gameWindow, event);
	else if (src->activeWindow == SPGUI_SET_WINDOW)
		spSetWindowDraw(src->settingsWindow);
	else if (src->activeWindow == SPGUI_LOAD_WINDOW)
		spLoadWindowDraw(src->loadWindow);
}

SPGUI_CONTROLLER_EVENT spControllerEventHandler(SPGUIController *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_CONTROLLER_EVENT_NONE;
	if (src->activeWindow == SPGUI_MAIN_WINDOW) {
		SPGUI_MAIN_EVENT mainEvent = spMainWindowEventHandler(src->mainWindow, event);
		spControllerDraw(src, event);
		return spControllerHandleMainEvent(src, mainEvent);
	} else if (src->activeWindow == SPGUI_GAME_WINDOW) {
		SPGUI_GAME_EVENT gameEvent = spGameWindowEventHandler(src->gameWindow, event);
		spControllerDraw(src, event);
		return spControllerHandleGameEvent(src, gameEvent);
	} else if (src->activeWindow == SPGUI_LOAD_WINDOW) {
		SPGUI_LOAD_EVENT loadEvent = spLoadWindowEventHandler(src->loadWindow, event);
		spControllerDraw(src, event);
		return spControllerHandleLoadEvent(src, loadEvent);
	} else if (src->activeWindow == SPGUI_SET_WINDOW) {
		SPGUI_SET_EVENT setEvent = spSetWindowEventHandler(src->settingsWindow, event);
		spControllerDraw(src, event);
		return spControllerHandleSetEvent(src, setEvent);
	}
	return SPGUI_CONTROLLER_EVENT_NONE;
}

SPGUI_CONTROLLER_EVENT spControllerHandleMainEvent(SPGUIController *src, SPGUI_MAIN_EVENT event) {
	if (!src)
		return SPGUI_CONTROLLER_EVENT_NONE;
	if (event == SPGUI_MAIN_NEW_GAME) {
		SDL_HideWindow(src->mainWindow->mainWindow);
		src->settingsWindow = spSetWindowCreate();
		if (src->settingsWindow == NULL) {
			printf("couldn't create settings window\n");
			return SPGUI_CONTROLLER_EVENT_QUIT;
		}
		src->activeWindow = SPGUI_SET_WINDOW;
		src->previousWindow = SPGUI_MAIN_WINDOW;
	}
	if (event == SPGUI_MAIN_LOAD) {
		SDL_HideWindow(src->mainWindow->mainWindow);
		src->loadWindow = spLoadWindowCreate();
		if (src->loadWindow == NULL) {
			printf("couldn't create load window\n");
			return SPGUI_CONTROLLER_EVENT_QUIT;
		}
		src->activeWindow = SPGUI_LOAD_WINDOW;
		src->previousWindow = SPGUI_MAIN_WINDOW;
	}
	if (event == SPGUI_MAIN_EXIT || event == SPGUI_MAIN_QUIT) {
		return SPGUI_CONTROLLER_EVENT_QUIT;
	}
	return SPGUI_CONTROLLER_EVENT_NONE;
}

SPGUI_CONTROLLER_EVENT spControllerHandleGameEvent(SPGUIController *src, SPGUI_GAME_EVENT event) {
	if (!src)
		return SPGUI_CONTROLLER_EVENT_NONE;
	if (event == SPGUI_GAME_LOAD) {
		SDL_HideWindow(src->gameWindow->gameWindow);
		if (src->loadWindow != NULL)
			SDL_ShowWindow(src->loadWindow->loadWindow);
		else {
			src->loadWindow = spLoadWindowCreate();
			if (src->loadWindow == NULL) {
				printf("couldn't create load window\n");
				return SPGUI_CONTROLLER_EVENT_QUIT;
			}
		}
		src->activeWindow = SPGUI_LOAD_WINDOW;
		src->previousWindow = SPGUI_GAME_WINDOW;
	}
	if (event == SPGUI_GAME_MAIN_MENU) {
		spGameWindowDestroy(src->gameWindow);
		SDL_ShowWindow(src->mainWindow->mainWindow);
		src->activeWindow = SPGUI_MAIN_WINDOW;
		src->previousWindow = SPGUI_NO_WINDOW;
	}
	if (event == SPGUI_GAME_PLAYER_1_CHECKMATE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECKMATE!",
				"White player won the game", NULL);
		return SPGUI_CONTROLLER_EVENT_QUIT;
	}
	if (event == SPGUI_GAME_PLAYER_2_CHECKMATE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECKMATE!",
				"Black player won the game", NULL);
		return SPGUI_CONTROLLER_EVENT_QUIT;
	}
	if (event == SPGUI_GAME_TIE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TIE!",
				"The game ends with tie", NULL);
		return SPGUI_CONTROLLER_EVENT_QUIT;
	}
	if (event == SPGUI_GAME_EXIT || event == SPGUI_GAME_QUIT) {
		return SPGUI_CONTROLLER_EVENT_QUIT;
	}
	return SPGUI_CONTROLLER_EVENT_NONE;
}

SPGUI_CONTROLLER_EVENT spControllerHandleLoadEvent(SPGUIController *src, SPGUI_LOAD_EVENT event) {
	if (!src)
		return SPGUI_CONTROLLER_EVENT_NONE;
	if (event == SPGUI_LOAD_BACK) {
        SDL_HideWindow(src->loadWindow->loadWindow);
		if (src->previousWindow == SPGUI_MAIN_WINDOW) {
			src->activeWindow = SPGUI_MAIN_WINDOW;
			SDL_ShowWindow(src->mainWindow->mainWindow);
		} else if (src->previousWindow == SPGUI_GAME_WINDOW) {
			src->activeWindow = SPGUI_GAME_WINDOW;
			SDL_ShowWindow(src->gameWindow->gameWindow);
		}
		src->previousWindow = SPGUI_LOAD_WINDOW;
	}
	if (event == SPGUI_LOAD_LOAD) {
        SDL_HideWindow(src->loadWindow->loadWindow);
		src->gameWindow = spGameWindowCreate(src->loadWindow->game);
		if (src->gameWindow == NULL) {
			printf("couldn't create game window\n");
			return SPGUI_CONTROLLER_EVENT_QUIT;
		}
		src->gameWindow->isSaved = true;
		src->activeWindow = SPGUI_GAME_WINDOW;
		src->previousWindow = SPGUI_LOAD_WINDOW;
	}
	if (event == SPGUI_LOAD_QUIT)
		return SPGUI_CONTROLLER_EVENT_QUIT;
	return SPGUI_CONTROLLER_EVENT_NONE;

}

SPGUI_CONTROLLER_EVENT spControllerHandleSetEvent(SPGUIController *src, SPGUI_SET_EVENT event) {
	if (!src)
		return SPGUI_CONTROLLER_EVENT_NONE;
	if (event == SPGUI_SET_BACK) {
		SDL_HideWindow(src->settingsWindow->setWindow);
		SDL_ShowWindow(src->mainWindow->mainWindow);
		src->activeWindow = SPGUI_MAIN_WINDOW;
		src->previousWindow = SPGUI_SET_WINDOW;
	}
	if (event == SPGUI_SET_START) {
		SDL_HideWindow(src->settingsWindow->setWindow);
		src->gameWindow = spGameWindowCreate(src->settingsWindow->game);
		if (src->gameWindow == NULL) {
			printf("couldn't create game window\n");
			return SPGUI_CONTROLLER_EVENT_QUIT;
		}
		spSetWindowDestroy(src->settingsWindow);
		src->activeWindow = SPGUI_GAME_WINDOW;
		src->previousWindow = SPGUI_SET_WINDOW;
	}
	if (event == SPGUI_SET_QUIT)
		return SPGUI_CONTROLLER_EVENT_QUIT;
	return SPGUI_CONTROLLER_EVENT_NONE;
}

