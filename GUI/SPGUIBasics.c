#include "SPGUIBasics.h"

SDL_Rect* copyRect(SDL_Rect *src) {
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
	SDL_Surface* loadingSurface = SDL_LoadBMP(activeImage);
	SDL_Texture* activeTexture = SDL_CreateTextureFromSurface(windowRender, loadingSurface);
	if (res == NULL || loadingSurface == NULL || activeTexture == NULL) {
		printf("Could not create a surface: %s\n", activeImage);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(activeTexture);
		free(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	SDL_Surface* loadingSurface2 = SDL_LoadBMP(inactiveImage);
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
	SDL_FreeSurface(loadingSurface2);
	res->windowRenderer = windowRender;
	res->activeTexture = activeTexture;
	res->inactiveTexture = inactiveTexture;
	res->location = copyRect(location);
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
		const char* inactiveImages[], int xVals[], int yVals[], bool visible[],
		bool active[], SPGUI_BUTTON_TYPE types[], int numOfButtons) {
	if (windowRender == NULL)
		return NULL;
	Button** buttons = (Button**) malloc(sizeof(Button*) * numOfButtons);
	if (buttons == NULL)
		return NULL;
	for (int i = 0; i < numOfButtons; i++) {
		SDL_Rect rec = { .x = xVals[i], .y = yVals[i], .h = BUTTON_H, .w = BUTTON_W};
		buttons[i] = createButton(windowRender, activeImages[i], inactiveImages[i],
							   &rec, visible[i], active[i], types[i]);
		if (buttons[i] == NULL) {
			for (int k = 0; k < i; k++) {
				destroyButton(buttons[k]);
				free(buttons);
			}
			return NULL;
		}
	}
	return buttons;
}

void destroyButtons(Button** buttons, int numOfButtons) {
	if(buttons == NULL)
		return;
	for(int i = 0; i < numOfButtons; i++)
		destroyButton(buttons[i]);
}


int countSavedFiles() {
	const char* savePaths[] = {SAVE1 , SAVE2 , SAVE3 , SAVE4 , SAVE5};
	int cnt = 0;
	for(int i = 0; i<NUM_OF_SAVES ; i++){
		if (access(savePaths[i], F_OK) != -1)
			cnt++;
		else
			return cnt;
	}
	return cnt;
}

void promoteSlots() {
	const char* savePaths[] = {SAVE1 , SAVE2 , SAVE3 , SAVE4 , SAVE5};
	int numOfSaves = countSavedFiles();
	if (access(savePaths[numOfSaves-1], F_OK) != -1)
		remove(SAVE5);
	for(int i = numOfSaves-2; i>=0 ; i--)
		rename(savePaths[i], savePaths[i+1]);
}
SPGUI_BUTTON_TYPE getPressedButtonType(Button **buttons, int numOfButtons, SDL_Event *event, bool checkActive) {
	SDL_Point point;
	point.x = event->button.x;
	point.y = event->button.y;
	for (int i = 0; i < numOfButtons; i++) {
		if (SDL_PointInRect(&point, buttons[i]->location) && buttons[i]->visible) {
			if (!checkActive) return buttons[i]->type;
			else if(buttons[i]->active) return buttons[i]->type;
		}
	}
	return NO_BUTTON;
}

