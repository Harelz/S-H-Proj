#include "guimode.h"

int activegui() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SPCHESSGuiManager* manager = spManagerCreate();
	if (manager == NULL) {
		SDL_Quit();
		return 0;
	}
	SDL_Event event;
	while (true) {
		SDL_WaitEvent(&event);
		if (spManagerHandleEvent(manager, &event) == SPCHESS_MANAGER_QUIT)
			break;
	}
	spManagerDestroy(manager);
	SDL_Quit();
	return 0;
}
