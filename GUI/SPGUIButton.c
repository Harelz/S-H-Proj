#include "SPGUIButton.h"

SDL_Rect* spCopyRect(SDL_Rect* src) {
	if (src == NULL)
		return NULL;

	SDL_Rect* res = malloc(sizeof(SDL_Rect));
	if (res == NULL)
		return NULL;

	res->h = src->h;
	res->w = src->w;
	res->x = src->x;
	res->y = src->y;
	return res;
}

Button* createButton(SDL_Renderer* windowRender, const char* activeImage,
		const char* inactiveImage, SDL_Rect* location, bool visible,
		bool active, SPGUI_BUTTON_TYPE type) {
	if (windowRender == NULL || location == NULL || activeImage == NULL
			|| inactiveImage == NULL)
		return NULL;
	Button* res = (Button*) malloc(sizeof(Button));
	SDL_Surface* loadingSurface = SDL_LoadBMP(activeImage); //We use the surface as a temp var
	SDL_Texture* activeTexture = SDL_CreateTextureFromSurface(windowRender, loadingSurface);
	//loading active image
	if (res == NULL || loadingSurface == NULL || activeTexture == NULL) {
		printf("Could not create a surface: %s\n", activeImage);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(activeTexture);
		free(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	//loading inactive image
	SDL_Surface* loadingSurface2 = SDL_LoadBMP(inactiveImage); //We use the surface as a temp var
	SDL_Texture* inactiveTexture = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface2);
	if (res == NULL || loadingSurface2 == NULL || inactiveTexture == NULL) {
		printf("Could not create a surface: %s\n", inactiveImage);
		SDL_FreeSurface(loadingSurface2);
		SDL_DestroyTexture(inactiveTexture);
		SDL_DestroyTexture(activeTexture);
		free(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface2); //Surface is not actually needed after texture is created
	res->windowRenderer = windowRender;
	res->activeTexture = activeTexture;
	res->inactiveTexture = inactiveTexture;
	res->location = spCopyRect(location);
	res->visible = visible;
	res->active = active;
	res->type = type;
	return res;
}

void destroyButton(Button* src) {
	if (!src)
		return;
	SDL_DestroyTexture(src->activeTexture);
	SDL_DestroyTexture(src->inactiveTexture);
	SDL_DestroyRenderer(src->windowRenderer);
	free(src->location);
	free(src);
}

void drawButton(Button* src) {
	if (!src)
		return;
	if (src->visible) {
		if (src->active) {
			SDL_RenderCopy(src->windowRenderer, src->activeTexture, NULL,
					src->location);
		} else
			SDL_RenderCopy(src->windowRenderer, src->inactiveTexture, NULL,
					src->location);
	}
}

Button** createButtons(SDL_Renderer* windowRender, const char* activeImages[],
		const char* inactiveImages[], int xBtns[], int yBtns[], bool visible[],
		bool active[], SPGUI_BUTTON_TYPE types[], int numOfBtns) {

	//maybe more
	if (windowRender == NULL)
		return NULL;

	Button** btns = (Button**) malloc(sizeof(Button*) * numOfBtns);
	if (btns == NULL)
		return NULL;

	for (int i = 0; i < numOfBtns; i++) {
		SDL_Rect rec = { .x = xBtns[i], .y = yBtns[i], .h = BUTTON_H, .w = BUTTON_W};
		btns[i] = createButton(windowRender, activeImages[i], inactiveImages[i],
							   &rec, visible[i], active[i], types[i]);
		if (btns[i] == NULL) {
			for (int k = 0; k < i; k++) {
				destroyButton(btns[k]);
				free(btns);
			}
			return NULL;
		}
	}
	return btns;
}


void destroyButtons(Button** btns, int numOfBtns) {
	if(btns == NULL)
		return;

	for(int i = 0; i < numOfBtns; i++)
		destroyButton(btns[i]);

}



