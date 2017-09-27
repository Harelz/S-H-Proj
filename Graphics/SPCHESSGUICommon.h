#ifndef GRAPHICS_SPCHESSGUICOMMON_H_
#define GRAPHICS_SPCHESSGUICOMMON_H_

#include "SPCHESSGUIButton.h"
#include "../SPGame.h"
#include "../SPMiniMax.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//IMAGES PATH MACRO
#define ACT_IMG(g) "./graphics/images/"#g".bmp"
#define INACT_IMG(g) "./graphics/images/inactive_"#g".bmp"

//SLOTS MACRO
#define NUM_SLOTS 5
#define SLOT0 "./graphics/saved_games/slot0.xml"
#define SLOT1 "./graphics/saved_games/slot1.xml"
#define SLOT2 "./graphics/saved_games/slot2.xml"
#define SLOT3 "./graphics/saved_games/slot3.xml"
#define SLOT4 "./graphics/saved_games/slot4.xml"

/*
 * The function counts how many saved games exist (0-5)
 */
int countSavedFiles();

/*
 * The function promote the saved games: each game is promoted to it's next location.
 * In addition the last saved game is removed.
 */
void promoteSlots();

/*
 * The function returns the button's type that was clicked (in the current window).
 */
SPCHESS_BUTTON_TYPE getButtonClicked(Button** btns, int numOfBtns,
		SDL_Event* event, bool checkActive);

#endif /* GRAPHICS_SPCHESSGUICOMMON_H_ */
