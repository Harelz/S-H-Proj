#ifndef GRAPHICS_SPCHESSGUICOMMON_H_
#define GRAPHICS_SPCHESSGUICOMMON_H_

#include "SPGUIButton.h"
#include "../SPGame.h"
#include "../SPMiniMax.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//IMAGES PATH MACRO
#define ACT_IMG(g) "../GUI/images/"#g".bmp"
#define INACT_IMG(g) "../GUI/images/inactive_"#g".bmp"
#define NUM_OF_SAVES 5
#define SAVE1 "../GUI/saved_games/save1.xml"
#define SAVE2 "../GUI/saved_games/save2.xml"
#define SAVE3 "../GUI/saved_games/save3.xml"
#define SAVE4 "../GUI/saved_games/save4.xml"
#define SAVE5 "../GUI/saved_games/save5.xml"
//const char* savePaths[] = {SAVE1 , SAVE2 , SAVE3 , SAVE4 , SAVE5};
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
SPGUI_BUTTON_TYPE getButtonClicked(Button** buttons, int numOfButtons,
		SDL_Event* event, bool checkActive);

#endif /* GRAPHICS_SPCHESSGUICOMMON_H_ */
