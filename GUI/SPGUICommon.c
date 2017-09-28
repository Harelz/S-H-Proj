#include "SPGUICommon.h"

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
	/*
	int numOfSavedSlots = countSavedFiles();
	int ind = numOfSavedSlots - 1;
	if (numOfSavedSlots == NUM_OF_SAVES) {
		remove(SAVE5);
		ind--;
	}
	if (ind >= 3) {
		rename(SAVE4, SAVE5);
		ind--;
	}
	if (ind >= 2) {
		rename(SAVE3, SAVE4);
		ind--;
	}
	if (ind >= 1) {
		rename(SAVE2, SAVE3);
		ind--;
	}
	if (ind >= 0) {
		rename(SAVE1, SAVE2);
		ind--;
	}*/
}
SPGUI_BUTTON_TYPE getButtonClicked(Button** buttons, int numOfButtons, SDL_Event* event, bool checkActive) {
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
