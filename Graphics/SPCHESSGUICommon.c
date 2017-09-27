#include "SPCHESSGUICommon.h"

int countSavedFiles() {
	int cnt = 0;

	if (access(SLOT0, F_OK) != -1)
		cnt++;
	else
		return cnt;
	if (access(SLOT1, F_OK) != -1)
		cnt++;
	else
		return cnt;
	if (access(SLOT2, F_OK) != -1)
		cnt++;
	else
		return cnt;
	if (access(SLOT3, F_OK) != -1)
		cnt++;
	else
		return cnt;
	if (access(SLOT4, F_OK) != -1)
		cnt++;
	else
		return cnt;

	return cnt;
}

void promoteSlots() {
	int numOfSavedSlots = countSavedFiles();
	int ind = numOfSavedSlots - 1;
	if (numOfSavedSlots == NUM_SLOTS) {
		remove(SLOT4);
		ind--;
	}
	if (ind >= 3) {
		rename(SLOT3, SLOT4);
		ind--;
	}
	if (ind >= 2) {
		rename(SLOT2, SLOT3);
		ind--;
	}
	if (ind >= 1) {
		rename(SLOT1, SLOT2);
		ind--;
	}
	if (ind >= 0) {
		rename(SLOT0, SLOT1);
		ind--;
	}
}

SPCHESS_BUTTON_TYPE getButtonClicked(Button** btns, int numOfBtns,
		SDL_Event* event, bool checkActive) {
	for (int i = 0; i < numOfBtns; i++) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, btns[i]->location) && btns[i]->visible) {
			if (checkActive) {
				if (btns[i]->active)
					return btns[i]->type;
			} else
				return btns[i]->type;
		}
	}
	return NO_BUTTON;
}
