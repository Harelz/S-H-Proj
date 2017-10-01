#ifndef GRAPHICS_SPCHESSGUIBUTTON_H_
#define GRAPHICS_SPCHESSGUIBUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "../SPGame.h"
// button sizes
#define BUTTON_W 150
#define BUTTON_H 50

#define NUM_OF_SAVES 5

// files relatives paths
#ifdef _WIN32
#define ACTIVE_BMP_PATH(g) "../GUI/images/button_"#g".bmp"
#define INACTIVE_BMP_PATH(g) "../GUI/images/button_"#g"_F.bmp"
#define SAVE1 "../GUI/saved_games/save1.xml"
#define SAVE2 "../GUI/saved_games/save2.xml"
#define SAVE3 "../GUI/saved_games/save3.xml"
#define SAVE4 "../GUI/saved_games/save4.xml"
#define SAVE5 "../GUI/saved_games/save5.xml"
#elif __unix__
#define ACTIVE_BMP_PATH(g) "./GUI/images/button_"#g".bmp"
	#define INACTIVE_BMP_PATH(g) "./GUI/images/button_"#g"_F.bmp"
	#define SAVE1 "./GUI/saved_games/save1.xml"
	#define SAVE2 "./GUI/saved_games/save2.xml"
	#define SAVE3 "./GUI/saved_games/save3.xml"
	#define SAVE4 "./GUI/saved_games/save4.xml"
	#define SAVE5 "./GUI/saved_games/save5.xml"
#endif




// enum which represents button types
typedef enum {
	BUTTON_MAIN_NEW_GAME,
	BUTTON_MAIN_LOAD,
	BUTTON_MAIN_EXIT,

	BUTTON_LOAD_SLOT0,
	BUTTON_LOAD_SLOT1,
	BUTTON_LOAD_SLOT2,
	BUTTON_LOAD_SLOT3,
	BUTTON_LOAD_SLOT4,
	BUTTON_LOAD_LOAD,
	BUTTON_LOAD_BACK,

	BUTTON_SET_GAME_MODE,
	BUTTON_SET_ONE_PLAYER,
	BUTTON_SET_TWO_PLAYER,
	BUTTON_SET_DIFF,
	BUTTON_SET_NOOB_DIFF,
	BUTTON_SET_EASY_DIFF,
	BUTTON_SET_MOD_DIFF,
	BUTTON_SET_HARD_DIFF,
	BUTTON_SET_SELECT_COLOR,
	BUTTON_SET_BLACK_PLAYER,
	BUTTON_SET_WHITE_PLAYER,
	BUTTON_SET_BACK,
	BUTTON_SET_START,

	BUTTON_GAME_RESTART,
	BUTTON_GAME_SAVE,
	BUTTON_GAME_LOAD,
	BUTTON_GAME_UNDO,
	BUTTON_GAME_MAIN_MENU,
	BUTTON_GAME_EXIT,

	NO_BUTTON

} SPGUI_BUTTON_TYPE;


// button struct
typedef struct button_t {
	SDL_Renderer* windowRenderer;
	SDL_Texture* activeTexture;
	SDL_Texture* inactiveTexture;
	SDL_Rect* location;
	bool visible;
	bool active;
	SPGUI_BUTTON_TYPE type;
} Button;


/**
 * Creates a new button with the parameters given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new button instant is returned.
 */
Button* createButton(SDL_Renderer* windowRender, const char* activeImage, const char* inactiveImage,
					 SDL_Rect* location, bool visible,bool active, SPGUI_BUTTON_TYPE type);

/**
 * Creates a list of buttons with the parameters given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a list of buttons is returned.
 */
Button** createButtons(SDL_Renderer* windowRender, const char* activeImages[],const char* inactiveImages[],
		int xVals[], int yVals[], bool visible[],bool active[], SPGUI_BUTTON_TYPE types[], int numOfButtons);

/*
 * free the memory which located by the button
 */
void destroyButton(Button* src);

/*
 * free the memory which located by the buttons array
 */
void destroyButtons(Button** buttons, int numOfButtons);

/*
 * the function draws a button on the window according to the
 * button's window renderer and its SDL_Rect values
 */
void drawButton(Button* src);

/**
 * Copies and returns the SDL_Rect given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, the copied SDL_Rect is returned.
 */
SDL_Rect* copyRect(SDL_Rect *src);

/**
 * Counts the number of save files in the sev_games folder
 *
 * @return
 * Number of saves that exist.
 */
int countSavedFiles();

/*
 * 'logic shift' the saving files' order while deleting the overwriting
 * the fifth file if neccessery
 */
void promoteSlots();

/**
 * the function detects the button's type (if exists) that have been clicked in the
 * event from the buttons array. if checkActive == true, the function detects
 * only activated buttons in the array
 *
 * @return
 * The type of the pressed button or NO_BUTTON
 * if no button have been pressed in the event
 */
SPGUI_BUTTON_TYPE getPressedButtonType(Button **buttons, int numOfButtons, SDL_Event *event, bool checkActive);

#endif /* GRAPHICS_SPCHESSGUIBUTTON_H_ */
